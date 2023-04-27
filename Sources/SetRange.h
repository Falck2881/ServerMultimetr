#ifndef SETRANGE_H
#define SETRANGE_H

#include "Command.h"
#include "RangeVoltage.h"

class SetRange: public Command
{
    public:
        SetRange(Multimetr* const multimetr);
        void execute(const std::string &parameters) override;
    private:
        Multimetr* const multimetr;
};

#endif // SETRANGE_H
