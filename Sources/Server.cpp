#include "Server.h"
#include <iostream>
#include <string>

Server::Server():
    socketServer{std::make_unique<Socket>()},
    dispatcher{std::make_unique<Dispatcher>(socketServer->get())}
{

}

void Server::run()
{
    while(true)
    {
        int countIncomingCalls{dispatcher->waitForIncomingCalls()};

        if(incomingExist(countIncomingCalls))
            workWithClints(countIncomingCalls);

    }
}

bool Server::incomingExist(const int countIncomingCalls) const
{
    return countIncomingCalls != -1 && countIncomingCalls > 0;
}

void Server::workWithClints(const int countIncomingCalls)
{
    for(int i{0}; i < countIncomingCalls; ++i)
    {
        if(isNoClientReserved(dispatcher->callingOfClient(i)))
            registerClient();
        else{
            std::string answer = executeRequestClient(dispatcher->callingOfClient(i));
            notifyClient(dispatcher->callingOfClient(i), answer);
            checkPresenceClient(dispatcher->callingOfClient(i));
        }
    }
}

bool Server::isNoClientReserved(const int socketClient_fd) const
{
    return socketClient_fd == socketServer->get();
}

void Server::registerClient()
{
    const int newClient_fd{accept(socketServer->get(),NULL,NULL)};

    if(newClient_fd == -1){
        std::cout << "Server: Failed links with client.\n" << std::endl;
        exit(1);
    }
    dispatcher->addSocketForObserving(newClient_fd);
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
