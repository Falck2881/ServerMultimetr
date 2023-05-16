#ifndef SOKETSERVER_H
#define SOKETSERVER_H

#include "Multimetr.h"
#include "System.h"
#include "CommunicationCenter.h"
#include <map>

class Socket
{

    public:
        static Socket* instance();
        int get() const;
        std::shared_ptr<Multimetr> findClient(const int socketClient) const;
        void addClient(const int socketClient);
        void removeOldSession();
        void notifyClient(const int socket_fd, const std::string& message);
        void close();
    private:
        void initialize();
        void create();
        void bindSocketToTheDomain();
        void readyListen();
        bool isClientDisconnected(const int result) const;
        void disconnectClient(const int socketClient);
        void disconnect(const int socketClient);
        void completeWorkClient();
    private:
        static Socket* socketServer;
        struct sockaddr_un domain;
        int socket_fd;
        const std::string pathToAddress;
        CommunicationCenter technicalSupport;
        std::map<int,std::shared_ptr<Multimetr>> notebookOfClients;
        std::map<int,std::shared_ptr<Multimetr>>::iterator hintForNextClient;
    protected:
        Socket();

};

#endif // SOKETSERVER_H
