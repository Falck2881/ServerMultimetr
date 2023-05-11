#include "Dispatcher.h"
#include "System.h"
#include <iostream>

Dispatcher::Dispatcher(const int socketServer)
{
    event_fd = Sys::createEventPoll(0);

    if(techicalSupport.isOpeartionSuccessfully(event_fd, "Server: Failed to create instance 'epoll'.\n"))
        std::cout << "Server: Successfully create instance of Eventpoll.\n" << std::endl;

    addSocketForObserving(socketServer);
}


void Dispatcher::addSocketForObserving(const int socket_fd)
{
    observeAtIncoming.events = EPOLLIN | EPOLLET;
    observeAtIncoming.data.fd = socket_fd;

    const int result{epoll_ctl(event_fd, EPOLL_CTL_ADD, socket_fd, &observeAtIncoming)};

    if(techicalSupport.isOpeartionSuccessfully(result,"Server: Failed request a operation 'op' for file of description\n"))
        std::cout << "Server: Successfully registration file of description.\n" << std::endl;
}

int Dispatcher::waitForIncomingCalls()
{
    return epoll_wait(event_fd, incomingCalls.data(), MAXCALLS, -1);
}

int Dispatcher::callingOfClient(const int index)
{
    return incomingCalls.at(index).data.fd;
}
