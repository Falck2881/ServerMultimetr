#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>
#include <utility>
#include <shared_mutex>
#include <condition_variable>
#include "RangeVoltage.h"

enum class Status{RUN,NO_RUN,EXIT};

class Channel
{
    public:
        Channel(std::string name);
        std::string getName();
        void setStatus(Status newStatus);
        void setVoltage(Voltage newVoltage);
        void start();
        bool checkStatusActivity();
        bool checkVoltage();
        std::string getVoltage();
    private:
        float currentVoltageInChannel;
        std::string name;
        Voltage voltage;
        Status status;
        std::mutex mutex;
        std::condition_variable event;
};

#endif // CHANNEL_H
