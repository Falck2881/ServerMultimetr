#include "Server.h"
#include <iostream>
#include <string>
#include <signal.h>

void quit(const int)
{
    Socket* admin = Socket::instance();
    if(Sys::closeSocket(admin->get()) == 0){
        admin->close();
        admin->removeOldSession();
        std::cout << "Server: COMPLETION OF WORK.\n" << std::endl;
        exit(0);
    }
    else std::cout << "Server: failed close socket of server." << std::endl;
}

Server::Server()
{
    admin = Socket::instance();
    dispatcher = std::make_unique<Dispatcher>(admin->get());
    signal.sa_handler = quit;

}

void Server::run()
{
    while(true)
    {
        sigaction(SIGTSTP, &signal, NULL);
        sigaction(SIGINT, &signal, NULL);
        sigaction(SIGQUIT, &signal, NULL);
        int countIncomingCalls{dispatcher->waitForIncomingCalls()};

        if(incomingRequestExist(countIncomingCalls))
            workWithRequest(countIncomingCalls);
    }
}

bool Server::incomingRequestExist(const int countIncomingCalls) const
{
    return countIncomingCalls != -1 && countIncomingCalls > 0;
}

void Server::workWithRequest(const int countIncommingCalls)
{
    for(int i{0}; i < countIncommingCalls; ++i)
    {
        if(isNoClientReserved(dispatcher->callingOfClient(i)))
            registerClient();
        else{
            std::string answer = executeRequest(dispatcher->callingOfClient(i));
            admin->notifyClient(dispatcher->callingOfClient(i), answer);
        }
    }
}

bool Server::isNoClientReserved(const int socketClient_fd) const
{
    return socketClient_fd == admin->get();
}

void Server::registerClient()
{
    const int newClient_fd{accept(admin->get(),NULL,NULL)};

    if(newClient_fd == -1){
        std::cout << "Server: Failed links with client.\n" << std::endl;
        exit(1);
    }
    dispatcher->addSocketForObserving(newClient_fd);
    admin->addClient(newClient_fd);
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

std::string Server::executeRequest(const int socket_fd) const
{
    std::string request = readRequestClient(socket_fd);
    auto multimetr = admin->findClient(socket_fd);
    std::string answer = multimetr->executeCommand(request);
    return answer;
}
