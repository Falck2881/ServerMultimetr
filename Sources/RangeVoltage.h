#ifndef RANGEVOLTAGE_H
#define RANGEVOLTAGE_H

#include <string>

struct Voltage
{
    public:
        Voltage(Voltage&&);
        Voltage(Voltage&);
        Voltage(float min = 0.0f, float max = 0.0f, int gain = 0);
        Voltage& operator=(Voltage& oldObj);
        Voltage& operator=(Voltage&& oldObj);
        float min() const;
        float max() const;
        float calculate();
        std::string min_str() const;
        std::string max_str() const;
    private:
        float minRange;
        float maxRange;
        int voltageGain;
        int factoryVoltage;
};
#endif // RANGEVOLTAGE_H
