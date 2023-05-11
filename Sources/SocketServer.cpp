#include "SocketServer.h"
#include <iostream>

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
