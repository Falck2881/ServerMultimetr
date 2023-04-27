#include "Server.h"
#include <iostream>
#include <string>

Server::Server():
    pathToAddres("/tmp/socketForMultimetr"),
    mainAddressSocket_fd(0)
{
    removeOldSassion();
    initialize();
}

void Server::removeOldSassion()
{
    int result = remove(pathToAddres.data());

    if(result != -1)
        std::cout << "Server: Old session successfully remove.\n" << std::endl;
}

void Server::initialize()
{
    createSocket();
    bindSocketToTheAddres();
    makePassiveSocketListening();
    startObserver();
}

void Server::createSocket()
{
    mainAddressSocket_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(mainAddressSocket_fd == -1)
    {
        std::cout << "Server: Failed to create socket of passive.\n" << std::endl;
        exit(1);
    }
}

void Server::bindSocketToTheAddres()
{
    memset(&localSocket, 0, sizeof(localSocket));
    localSocket.sun_family = AF_UNIX;
    strncpy(localSocket.sun_path, pathToAddres.data(), sizeof(localSocket.sun_path)-1);

    if(bind(mainAddressSocket_fd,reinterpret_cast<const struct sockaddr*>(&localSocket),sizeof(localSocket)) == -1)
    {
        std::cout << "Server: Failed to bind the socket to addres.\n" << std::endl;
        exit(1);
    }
}

void Server::makePassiveSocketListening()
{
    const int maxBackLog{100};
    const int result = listen(mainAddressSocket_fd, maxBackLog);
    if(result == -1)
    {
        std::cout <<  "Server: Failed to make passive socket listening.\n" << std::endl;
        exit(1);
    }
}

void Server::startObserver()
{
    observerForIncoming_fd = epoll_create1(0);

    if(observerForIncoming_fd == -1){
        std::cout << "Server: Failed to create instance 'epoll'.\n" << std::endl;
        exit(1);
    }

    addSocketForObserving(mainAddressSocket_fd);
}

void Server::addSocketForObserving(const int socket_fd)
{
    observerAtClients.events = EPOLLIN | EPOLLET;
    observerAtClients.data.fd = socket_fd;

    if(epoll_ctl(observerForIncoming_fd, EPOLL_CTL_ADD, socket_fd, &observerAtClients) == -1)
    {
        std::cout << "Failed request a operation 'op' for file of description\n" << std::endl;
        exit(-1);
    }
}

void Server::run()
{
    while(true)
    {
        int countIncomingCalls{waitForIncomingCalls()};

        if(incomingExist(countIncomingCalls))
            workWithClints(countIncomingCalls);

    }
}

int Server::waitForIncomingCalls()
{
    return epoll_wait(observerForIncoming_fd, incomingCallsFromClients.data(), MAXCALLS, -1);
}

bool Server::incomingExist(const int countIncomingCalls) const
{
    return countIncomingCalls != -1 && countIncomingCalls > 0;
}

void Server::workWithClints(const int countIncomingCalls)
{
    for(int i{0}; i < countIncomingCalls; ++i)
    {
        if(isNoClientReserved(incomingCallsFromClients.at(i).data.fd))
            registerClient();
        else{
            std::string answer = executeRequestClient(incomingCallsFromClients.at(i).data.fd);
            notifyClient(incomingCallsFromClients.at(i).data.fd, answer);
            checkPresenceClient(incomingCallsFromClients.at(i).data.fd);
        }
    }
}

bool Server::isNoClientReserved(const int socket_fd) const
{
    return socket_fd == mainAddressSocket_fd;
}

void Server::registerClient()
{
    const int newClient_fd{accept(mainAddressSocket_fd,NULL,NULL)};

    if(newClient_fd == -1){
        std::cout << "Server: Failed links with client.\n" << std::endl;
        exit(1);
    }
    addSocketForObserving(newClient_fd);
    addClientIntoNotebook(newClient_fd);
}

void Server::addClientIntoNotebook(const int socket_fd)
{
    if(notebookOfClients.empty()){
        notebookOfClients.insert(std::make_pair(socket_fd,std::make_shared<Multimetr>()));
        hintForNextClient = std::end(notebookOfClients);
    }
    else
        hintForNextClient = notebookOfClients.insert(hintForNextClient,std::make_pair(socket_fd,std::make_shared<Multimetr>()));

}

std::string readRequestClient(const int socket_fd)
{
    char buffer[256];
    memset(buffer,'\0',sizeof(buffer));
    std::string request;
    int sizeReadByte = read(socket_fd, buffer, sizeof(buffer));

    request = buffer;

    if(sizeReadByte != -1 && sizeReadByte > 0 && request != "")
        std::cout << "Server: Request of client successfully are read.\n" << std::endl;

    return request;
}

std::string Server::executeRequestClient(const int socket_fd) const
{
    std::string request = readRequestClient(socket_fd);
    auto multimetr = findMultimetrOfClient(socket_fd);
    std::string answer = multimetr->executeCommand(request);
    return answer;
}

std::shared_ptr<Multimetr> Server::findMultimetrOfClient(const int socket_fd) const
{
    auto multimetr = notebookOfClients.find(socket_fd);
    return multimetr->second;
}

void Server::notifyClient(const int socket_fd, const std::string &message) const
{
    int result = send(socket_fd, message.data(), message.capacity(), MSG_NOSIGNAL);
    if(result == -1 && wereClientDisable())
        std::cout << "Server: Failed to send a message about the accepted request.\n" << std::endl;
}

bool Server::wereClientDisable() const
{
    return errno == EPIPE;
}

void Server::checkPresenceClient(const int socket_fd)
{
    if(wereClientDisable())
        resetConnectionWithClient(socket_fd);
}

void Server::resetConnectionWithClient(const int socket_fd)
{
    if(!notebookOfClients.empty()){
        auto multimetr = notebookOfClients.find(socket_fd)->second;
        multimetr->completeExecutionAllChannels();
        notebookOfClients.erase(socket_fd);
        std::cout << "Server: Client - ' " << socket_fd << " ' were disabled.\n" << std::endl;
        errno = ECONNRESET;
    }
}
