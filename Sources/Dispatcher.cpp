#include "Dispatcher.h"
#include "System.h"
#include <signal.h>
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

    std::string message = "Server: Failed request a operation 'op' for client - " + std::to_string(socket_fd) + "\n";
    if(techicalSupport.isOpeartionSuccessfully(result, message))
        std::cout << "Server: Successfully registration file of description.\n" << std::endl;
}

int Dispatcher::waitForIncomingCalls()
{
    int result = epoll_wait(event_fd, incomingCalls.data(), MAXCALLS, -1);

    return result;
}

int Dispatcher::callingOfClient(const int index)
{
    return incomingCalls.at(index).data.fd;
}
