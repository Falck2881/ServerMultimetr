#include "CommunicationCenter.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

CommunicationCenter::CommunicationCenter():attemptsToConnection(1)
{

}

bool CommunicationCenter::isOpeartionSuccessfully(const int result, const std::string message)
{
    if(result != -1)
        return true;
    else{
        std::cout << message << std::endl;
        exit(1);
    }
}

std::string CommunicationCenter::errorInConnection()
{
    std::string message ("Attempt: " + std::to_string(attemptsToConnection) +
                         "|Failed connecting to server, check activity the server.\n");
    std::this_thread::sleep_for(2s);
    ++attemptsToConnection;

    return message;
}


