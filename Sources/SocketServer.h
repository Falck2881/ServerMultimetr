#ifndef SOKETSERVER_H
#define SOKETSERVER_H

#include "System.h"
#include "CommunicationCenter.h"

class Socket
{
    public:
        Socket();
        int get() const;
    private:
        void removeOldSession();
        void initialize();
        void create();
        void bindSocketToTheDomain();
        void readyListen();
    private:
        struct sockaddr_un domain;
        int socket_fd;
        const std::string pathToAddress;
        CommunicationCenter technicalSupport;

};

#endif // SOKETSERVER_H
