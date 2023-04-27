#include "GetResult.h"
#include "Multimetr.h"

GetResult::GetResult(Multimetr* const multimetr):
    multimetr(multimetr)
{
}

void GetResult::execute(const std::string &name)
{
    auto channel = multimetr->findChannel(name);

    if(channel->checkStatusActivity())
        message = std::string("Ok: Resulte V = '" + channel->getVoltage() + "'.\n");
    else
        message = std::string("fail: To get the result voltage, pause the channel '" + channel->getName() + "'.\n");
}
