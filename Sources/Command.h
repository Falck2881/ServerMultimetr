#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Multimetr;

class Command
{
    public:
        virtual ~Command() = default;
        virtual void execute(const std::string& parameters) = 0;
        std::string answer()const{return message;}
    protected:
        Command() = default;
        std::string message;
};

#endif // COMMAND_H
