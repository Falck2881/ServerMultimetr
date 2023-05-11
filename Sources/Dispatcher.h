#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "CommunicationCenter.h"
#include "System.h"
#include "Multimetr.h"
#include <map>

#define MAXCALLS 100

class Dispatcher
{
    public:
        Dispatcher(const int socketServer);
        void addSocketForObserving(const int socket_fd);
        int waitForIncomingCalls();
        int callingOfClient(const int index);

    private:
        int event_fd;
        struct epoll_event observeAtIncoming;
        std::array<struct epoll_event,MAXCALLS> incomingCalls;
        CommunicationCenter techicalSupport;
};

#endif // DISPATCHER_H
