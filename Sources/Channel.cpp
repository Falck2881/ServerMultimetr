#include "Channel.h"
#include <cmath>
#include <regex>
#include <strstream>
#include <iomanip>



Channel::Channel(std::string name):
    currentVoltageInChannel(0.0f),
    name(name),
    status{Status::NO_RUN}
{

}

std::string Channel::getName()
{
    std::unique_lock<std::mutex> lock_mute(mutex);
    return name;
}

void Channel::setVoltage(Voltage newVoltage)
{
    std::unique_lock<std::mutex> lock_mutex(mutex);
    voltage = newVoltage;
}

void Channel::start()
{
    while(true)
    {
        std::unique_lock lock_mutex(mutex);
        event.wait(lock_mutex,[&](){return status == Status::RUN || status == Status::EXIT;});

        if(status == Status::EXIT) break;

        currentVoltageInChannel = voltage.calculate();
    }
}

bool Channel::checkStatusActivity()
{
    std::unique_lock lock_mutex(mutex);
    return status == Status::NO_RUN;
}

bool Channel::checkVoltage()
{
    std::unique_lock lock_mutex(mutex);
    return voltage.min() != fabs(0.0f) && voltage.max() != fabs(0.0f);
}

void Channel::setStatus(Status newStatus)
{
    std::unique_lock lock_mutex(mutex);
    status = newStatus;
    event.notify_one();
}

std::string Channel::getVoltage()
{
    std::unique_lock lock_mutex(mutex);
    std::stringstream stream;
    stream << std::fixed << std::setprecision(8) << currentVoltageInChannel;
    std::string str_CurrentVoltage = stream.str();
    std::regex regExp("(0\\.\\d+[^0]|\\d+\\.0)");
    std::smatch match;

    if(std::regex_search(str_CurrentVoltage,match,regExp))
        str_CurrentVoltage = match[0];
    else
        str_CurrentVoltage = "empty: check regular expresion.\n";

    return str_CurrentVoltage;
}
