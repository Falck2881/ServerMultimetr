#ifndef GETSTATUS_H
#define GETSTATUS_H

#include "Command.h"

class GetStatus: public Command
{
    public:
        GetStatus(Multimetr* const multimetr);
        void execute(const std::string &nameChannel) override;
    private:
        Multimetr* const multimetr;
};

#endif // GETSTATUS_H
