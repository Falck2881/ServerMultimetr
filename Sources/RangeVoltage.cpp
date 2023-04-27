#include "RangeVoltage.h"
#include <strstream>
#include <iomanip>
#include <regex>


Voltage::Voltage(Voltage&& oldObj)
{
    this->minRange = oldObj.minRange;
    this->maxRange = oldObj.maxRange;
    this->voltageGain = oldObj.voltageGain;
    this->factoryVoltage = oldObj.factoryVoltage;
}

Voltage::Voltage(Voltage& oldObj)
{
    this->minRange = oldObj.minRange;
    this->maxRange = oldObj.maxRange;
    this->voltageGain = oldObj.voltageGain;
    this->factoryVoltage = oldObj.factoryVoltage;

}

Voltage::Voltage(float min, float max, int gain):
    minRange(min),maxRange(max),
    voltageGain(gain),factoryVoltage(gain)
{

}

Voltage& Voltage::operator=(Voltage && oldObj)
{
    if(this == &oldObj)
        return *this;

    this->minRange = oldObj.minRange;
    this->maxRange = oldObj.maxRange;
    this->voltageGain = oldObj.voltageGain;
    this->factoryVoltage = oldObj.factoryVoltage;

    return *this;
}

Voltage& Voltage::operator=(Voltage &oldObj)
{
    if(this == &oldObj)
        return *this;

    this->minRange = oldObj.minRange;
    this->maxRange = oldObj.maxRange;
    this->voltageGain = oldObj.voltageGain;
    this->factoryVoltage = oldObj.factoryVoltage;

    return *this;
}

float Voltage::min() const
{
    return minRange;
}

float Voltage::max() const
{
    return maxRange;
}

float Voltage::calculate()
{
    float currentVoltage = minRange * voltageGain;

    if(minRange <= currentVoltage && currentVoltage <= maxRange){
        voltageGain+=voltageGain;
        return currentVoltage;
    }
    else{
        voltageGain = factoryVoltage;
        currentVoltage = minRange * voltageGain;
        return currentVoltage;
    }
}

std::string Voltage::min_str() const
{
    std::strstream stream;
    stream << std::fixed << std::setprecision(8) << minRange;
    std::string minRange_str = stream.str();
    std::regex regExp("(1000000\\.0|0\\.0000001|0\\.001|1000\\.0|1\\.0)");
    std::smatch match;

    if(std::regex_search(minRange_str,match,regExp))
        minRange_str = match[0];
    else
        minRange_str = "Empty Voltage: check regular expresion in Voltage::min_str().\n";

    return minRange_str;
}

std::string Voltage::max_str() const
{
    std::strstream stream;
    stream << std::fixed << std::setprecision(8) << maxRange;
    std::string maxRange_str = stream.str();
    std::regex regExp("(1000000\\.0|0\\.0000001|0\\.001|1000\\.0|1\\.0)");
    std::smatch match;

    if(std::regex_search(maxRange_str,match,regExp))
        maxRange_str = match[0];
    else
        maxRange_str = "Empty Voltage: check regular expresion in Voltage::max_str().\n";

    return maxRange_str;
}
