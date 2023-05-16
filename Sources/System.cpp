#include "System.h"
#include <iostream>


int Sys::connectWithServer(const int socket_fd, const struct sockaddr_un activeSocket){
    return connect(socket_fd, reinterpret_cast<const struct sockaddr*>(&activeSocket), sizeof(activeSocket));
}

int Sys::createSocket(const int domain, const int type, const int protocol)
{
    return socket(domain, type, protocol);
}

int Sys::sendRequest(const int fileDescription, const char* buffer, const int lengthBuffer, const int flags)
{
    return send(fileDescription, buffer, lengthBuffer, flags);
}

int Sys::reciveAnswer(const int fileDescription,const char* buffer, const size_t sizeBuffer)
{
    return read(fileDescription, const_cast<char*>( buffer), sizeBuffer);
}

bool Sys::errorToSend(const int result)
{
    return result == -1 && errno == EPIPE ? true : false;
}

bool Sys::errorToRead(const int result)
{
    return result == -1 || result == 0 ? true : false;
}

int Sys::closeSocket(int socket_fd)
{
    return close(socket_fd);
}

int Sys::createEventPoll(const int flag)
{
    return epoll_create1(0);
}
