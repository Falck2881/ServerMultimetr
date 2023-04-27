#include "SetRange.h"
#include "Multimetr.h"
#include "RangeVoltage.h"
#include <regex>
#include <utility>
#include <iostream>
#include <iomanip>

std::string getChannel(const std::string& parameters);
Voltage getRangeVoltage(const std::string& parameters);
Voltage determinateRangeVoltage(const std::string& range);

SetRange::SetRange(Multimetr* const multimetr):
    multimetr(multimetr)
{

}

void SetRange::execute(const std::string &parameters)
{
    auto nameChannel = getChannel(parameters);
    auto channel = multimetr->findChannel(nameChannel);

    if(channel->checkStatusActivity()){
        Voltage voltage{getRangeVoltage(parameters)};
        channel->setVoltage(voltage);
        message = std::string("Ok: In channel '" + nameChannel + "' set range [" +
                              voltage.min_str() + " ... " + voltage.max_str() +"].\n");
    }
    else
        message = std::string("fail: Unable set range the channel '" + nameChannel + "'. Now channel is running.\n");

}

std::string getChannel(const std::string& parameters)
{
    std::regex regExp("channel\\d{1}");
    std::smatch match;
    std::string nameChannel;

    if(std::regex_search(parameters,match,regExp))
        nameChannel = match[0];

    return nameChannel;
}

Voltage getRangeVoltage(const std::string& parameters)
{
    std::regex regExp("range[0-4]{1}");
    std::smatch match;
    Voltage range;

    if(std::regex_search(parameters, match,regExp))
        range = determinateRangeVoltage(match[0]);

    return range;
}

Voltage determinateRangeVoltage(const std::string& range)
{
    if(range == "range0")
        return Voltage(0.0000001f,0.001f, 120);
    else if(range == "range1")
        return Voltage(0.001f, 1.0f, 12);
    else if(range == "range2")
        return Voltage(1.0f, 1000.0f ,14);
    else
        return Voltage(1000.0f, 1000000.0f, 8);
}
