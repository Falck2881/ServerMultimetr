#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <map>
#include <array>
#include <string>
#include <memory>
#include "Multimetr.h"

#define MAXCALLS 100

class Server
{
    public:
        Server();
        void run();
    private:
        void initialize();
        void removeOldSassion();
        void createSocket();
        void bindSocketToTheAddres();
        void makePassiveSocketListening();
        void startObserver();
        void addSocketForObserving(const int socket_fd);
        void addClientIntoNotebook(const int socket_fd);
        std::shared_ptr<Multimetr> findMultimetrOfClient(const int socket_fd) const;
        bool isNoClientReserved(const int socket_fd) const;
        bool incomingExist(const int countIncomingCalls) const;
        void registerClient();
        int waitForIncomingCalls();
        void workWithClints(const int countIncommingCalls);
        void notifyClient(const int socket_fd, const std::string& message) const;
        void checkPresenceClient(const int socket_fd);
        bool wereClientDisable() const;
        void resetConnectionWithClient(const int socket_fd);
        std::string executeRequestClient(const int socket_fd) const;
    private:
        const std::string pathToAddres;
        struct sockaddr_un localSocket;
        struct epoll_event observerAtClients;
        std::array<struct epoll_event,MAXCALLS> incomingCallsFromClients;
        std::map<int,std::shared_ptr<Multimetr>> notebookOfClients;
        std::map<int,std::shared_ptr<Multimetr>>::iterator hintForNextClient;
        int observerForIncoming_fd;
        int mainAddressSocket_fd;
};

#endif // SERVER_H
