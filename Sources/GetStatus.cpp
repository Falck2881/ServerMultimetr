#include "GetStatus.h"
#include "Multimetr.h"
#include <random>

bool isExistError();

GetStatus::GetStatus(Multimetr* const multimetr):
    multimetr(multimetr)
{

}

void GetStatus::execute(const std::string &name)
{
    auto channel = multimetr->findChannel(name);

    if(isExistError()){
        message = std::string("error_state: failed to get state of channel '" + channel->getName() + "', repeat attempt.\n");
        return;
    }

    if(channel->checkStatusActivity())
        message = std::string("idle_state:\n Range of Voltage: [" + channel->getVoltage() +"].\n"
                                            "State: '" + channel->getName() + "' paused.\n");
    else
        message = std::string("busy_state:\n State:" + channel->getName() + " paused.\n");

}

bool isExistError()
{
    std::random_device randDev;
    std::mt19937 mersenne(randDev());
    std::uniform_int_distribution dist(1,10);

    unsigned int number = dist(mersenne);

    if(number % 2 == 0)
        return true;
    else
        return false;
}
