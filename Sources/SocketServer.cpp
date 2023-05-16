#include "SocketServer.h"
#include <iostream>

Socket* Socket::socketServer = nullptr;

Socket::Socket():
    socket_fd(0),
    pathToAddress("/tmp/socketForMultimetr")
{
    removeOldSession();
    initialize();
}

void Socket::removeOldSession()
{
    int result = remove(pathToAddress.data());

    if(result == 0)
        std::cout << "Server: Successfully removing old session.\n" << std::endl;

}

void Socket::initialize()
{
    create();
    bindSocketToTheDomain();
    readyListen();
}

void Socket::create()
{
    socket_fd = Sys::createSocket(AF_UNIX, SOCK_STREAM, 0);

    if(technicalSupport.isOpeartionSuccessfully(socket_fd, "Server: Failed to create socket.\n"))
    {
        memset(&domain, 0, sizeof(domain));
        domain.sun_family = AF_UNIX;
        strncpy(domain.sun_path, pathToAddress.data(), sizeof(domain.sun_path)-1);
        std::cout << "Server: Socket successfully create.\n" << std::endl;
    }
}

void Socket::bindSocketToTheDomain()
{
    const int result{bind(socket_fd,reinterpret_cast<const struct sockaddr*>(&domain),sizeof(domain))};
    if(technicalSupport.isOpeartionSuccessfully(result,"Server: Failed to bind the socket to domain.\n"))
        std::cout << "Server: Socket successfully bind to domain.\n" << std::endl;

}

void Socket::readyListen()
{
    const int maxBackLog{100};
    const int result = listen(socket_fd, maxBackLog);
    if(technicalSupport.isOpeartionSuccessfully(result,"Server: Failed to make passive socket listening.\n"))
        std::cout << "Server: Socket ready to listen.\n" << std::endl;
}

int Socket::get() const
{
    return socket_fd;
}

std::shared_ptr<Multimetr> Socket::findClient(const int socketClient) const
{
    auto pair = notebookOfClients.find(socketClient);

    if(pair != notebookOfClients.end())
        return pair->second;
    else{
        std::cout << "Server: failed find socket of client - " << socketClient << ".\n" <<std::endl;
        exit(0);
    }
}

void Socket::addClient(const int socketClient)
{
    if(notebookOfClients.empty()){
        notebookOfClients.insert(std::make_pair(socketClient,std::make_shared<Multimetr>()));
        hintForNextClient = std::end(notebookOfClients);
    }
    else
        hintForNextClient = notebookOfClients.insert(hintForNextClient,std::make_pair(socketClient,std::make_shared<Multimetr>()));
}

void Socket::close()
{
    completeWorkClient();
}

void Socket::completeWorkClient()
{
    if(!notebookOfClients.empty())
    {
        auto beginBook = notebookOfClients.begin();
        auto endBook = notebookOfClients.end();
        while(beginBook != endBook)
        {
            auto socketClient = beginBook->first;
            disconnect(socketClient);
            auto multimetr = beginBook->second;
            multimetr->completeExecutionAllChannels();
            ++beginBook;
        }
        notebookOfClients.clear();
    }
}

void Socket::notifyClient(const int socket_fd, const std::string& message)
{
    int result = send(socket_fd, message.data(), message.capacity(), MSG_NOSIGNAL);

    if(technicalSupport.isClientDisconnected(result)){
        std::cout << "Server: Failed to send a message about the accepted request.\n" << std::endl;
        disconnectClient(socket_fd);
    }
}

void Socket::disconnectClient(const int socketClient)
{
    if(!notebookOfClients.empty())
    {
        auto multimetr = findClient(socketClient);
        multimetr->completeExecutionAllChannels();
        disconnect(socketClient);
        notebookOfClients.erase(socketClient);
    }
}

void Socket::disconnect(const int socket)
{
    const int result{Sys::closeSocket(socket)};

    if(result == 0)
        std::cout << "Server: Client - ' " << socket << " ' were disabled.\n" << std::endl;
    else
        std::cout << "Server: Error - " << errno << " at closing socket " << socket << ".\n" << std::endl;
}

Socket* Socket::instance()
{
    if(socketServer == nullptr)
        socketServer = new Socket();

    return socketServer;
}
