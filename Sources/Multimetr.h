#ifndef MULTIMETR_H
#define MULTIMETR_H

#include <map>
#include <string>
#include <memory>
#include <functional>
#include <thread>
#include "Command.h"
#include "Channel.h"

#define MAX_COUNT_CHANNEL 5

enum class TypeCommand{START_OF_MEASURE, SET_RANGE, STOP_OF_MEASURE,
                        STATUS_OF_DEVICE, RESULT_OF_MEASURE, NO_COMMAND};

using HintToInsertChannel = std::map<std::string, std::shared_ptr<Channel>>::iterator;
using HintToInsertTaskChannel = std::map<std::string, std::thread>::iterator;

class Multimetr
{
    public:
        Multimetr();
        void completeExecutionAllChannels();
        std::string executeCommand(std::string& name);
        std::shared_ptr<Channel> findChannel(const std::string& name);
        void runChannel(std::shared_ptr<Channel> channel);
        void setRangeCahnnel(const std::string& name, std::pair<float,float> range);
    private:
        void initialeChannels();
        void addChannel(std::unique_ptr<Channel> channel);
        void addChannelIntoSeparateThread(std::shared_ptr<Channel> channel);
        bool findChannelInThread(const std::string& channel);
        std::string getParameters(std::string& name);
        TypeCommand determineCommand(const std::string& name);
    private:
        std::map<std::string, std::shared_ptr<Channel>> channels;
        std::map<std::string, std::thread> tasksChannels;
        HintToInsertChannel hintToInsertChannel;
        HintToInsertTaskChannel hintToInsertTask;
        std::unique_ptr<Command> startMeasure;
        std::unique_ptr<Command> setRange;
        std::unique_ptr<Command> stopMeasure;
        std::unique_ptr<Command> status;
        std::unique_ptr<Command> resultMeasure;
};

#endif // MULTIMETR_H
