#ifndef SERVER_H
#define SERVER_H

#include <signal.h>
#include <map>
#include <string>
#include <memory>
#include "Multimetr.h"
#include "Dispatcher.h"
#include "SocketServer.h"


class Server
{
    public:
        Server();
        void run();
    private:
        bool incomingRequestExist(const int countIncomingCalls) const;
        void workWithRequest(const int countIncommingCalls);
        bool isNoClientReserved(const int socket_fd) const;
        void registerClient();
        std::string executeRequest(const int socket_fd) const;
    private:
        struct sigaction signal;
        Socket* admin;
        std::unique_ptr<Dispatcher> dispatcher;
};

#endif // SERVER_H
