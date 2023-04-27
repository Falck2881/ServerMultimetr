#ifndef GETRESULT_H
#define GETRESULT_H

#include "Command.h"

class GetResult: public Command
{
    public:
        GetResult(Multimetr* const multimetr);
        void execute(const std::string &parameters) override;
    private:
        Multimetr* const multimetr;
};

#endif // GETRESULT_H
