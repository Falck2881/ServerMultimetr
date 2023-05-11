#ifndef SYSTEM_H
#define SYSTEM_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/un.h>

namespace Sys {
    int connectWithServer(const int socket_fd, const struct sockaddr_un activeSocket);
    int createSocket(const int domain, const int type, const int protocol);
    int sendRequest(const int fileDescription, const char* buffer,const int lengthBuffer, const int flags);
    int reciveAnswer(const int fileDescription, const char* buffer, const size_t sizeBuffer);
    bool errorToSend(const int result);
    bool errorToRead(const int result);
    void closeSocket(int socket_fd);
    int createEventPoll(const int flag);
}

#endif // SYSTEM_H
