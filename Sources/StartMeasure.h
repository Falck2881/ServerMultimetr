#ifndef STARTMEASURE_H
#define STARTMEASURE_H

#include "Command.h"

class StartMeasure: public Command
{
    public:
        StartMeasure(Multimetr* const multimetr);
        void execute(const std::string& nameChannel) override;
    private:
        Multimetr* const multimetr;

};

#endif // STARTMEASURE_H
