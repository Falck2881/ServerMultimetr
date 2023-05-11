#ifndef SERVER_H
#define SERVER_H

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
        bool incomingExist(const int countIncomingCalls) const;
        void workWithClints(const int countIncommingCalls);
        bool isNoClientReserved(const int socket_fd) const;
        void registerClient();
        void addClientIntoNotebook(const int socket_fd);
        std::string executeRequestClient(const int socket_fd) const;
        std::shared_ptr<Multimetr> findMultimetrOfClient(const int socket_fd) const;
        void notifyClient(const int socket_fd, const std::string& message) const;
        bool wereClientDisable() const;
        void checkPresenceClient(const int socket_fd);
        void resetConnectionWithClient(const int socket_fd);
    private:
        std::unique_ptr<Socket> socketServer;
        std::unique_ptr<Dispatcher> dispatcher;
        std::map<int,std::shared_ptr<Multimetr>> notebookOfClients;
        std::map<int,std::shared_ptr<Multimetr>>::iterator hintForNextClient;
};

#endif // SERVER_H
