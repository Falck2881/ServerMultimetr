#ifndef COMMUNICATIONCENTER_H
#define COMMUNICATIONCENTER_H

#include <string>

class CommunicationCenter
{
    public:
        CommunicationCenter();
        std::string errorInConnection();
        bool isOpeartionSuccessfully(const int result, const std::string message);
    private:
        unsigned int attemptsToConnection;
};

#endif // COMMUNICATIONCENTER_H
