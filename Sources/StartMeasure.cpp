#include "StartMeasure.h"
#include "Multimetr.h"

StartMeasure::StartMeasure(Multimetr* const multimetr):
    multimetr(multimetr)
{

}

void StartMeasure::execute(const std::string& nameChannel)
{
    auto channel = multimetr->findChannel(nameChannel);

    if(channel->checkStatusActivity())
        if(channel->checkVoltage()){
            multimetr->runChannel(channel);
            channel->setStatus(Status::RUN);
            message = std::string("Ok: Channel - '" + nameChannel + "' is running");
        }else
            message = std::string("fail: Set range of voltage before run a channel - '" + nameChannel + "'");
    else
        message = std::string("fail: The channel - '" + nameChannel + "' already running.");

}

