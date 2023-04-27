#ifndef STOPMEASURE_H
#define STOPMEASURE_H

#include "Command.h"

class StopMeasure: public Command
{
    public:
        StopMeasure(Multimetr* const multimetr);
        void execute(const std::string &parameters) override;
    private:
        Multimetr* const multimetr;
};

#endif // STOPMEASURE_H
