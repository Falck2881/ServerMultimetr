#include "StopMeasure.h"
#include "Multimetr.h"

StopMeasure::StopMeasure(Multimetr* const multimetr):
    multimetr(multimetr)
{

}

void StopMeasure::execute(const std::string &name)
{
    auto channel = multimetr->findChannel(name);

    if(channel->checkStatusActivity()){
        channel->setStatus(Status::RUN);
        message = std::string("OK: Channel '" + channel->getName() + "' is running.\n");
    }
    else{
        channel->setStatus(Status::NO_RUN);
        message = std::string("OK: Channel '" + channel->getName() + "' is paused.\n");
    }
}

