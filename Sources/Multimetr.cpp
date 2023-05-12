#include "Multimetr.h"
#include "StartMeasure.h"
#include "SetRange.h"
#include "StopMeasure.h"
#include "GetResult.h"
#include "GetStatus.h"
#include <iostream>
#include <pthread.h>
#include <regex>

std::string getNameChannel(const int number);

Multimetr::Multimetr():
    startMeasure{std::make_unique<StartMeasure>(this)},
    setRange{std::make_unique<SetRange>(this)},
    stopMeasure{std::make_unique<StopMeasure>(this)},
    status{std::make_unique<GetStatus>(this)},
    resultMeasure{std::make_unique<GetResult>(this)}
{
    while(true)
    {
        if(MAX_COUNT_CHANNEL >= 2){
            initialeChannels();
            break;
        }
        else std::cout << "Server: Check given count a channels !" << std::endl;
    }
}

void Multimetr::initialeChannels()
{
    for(int i{0}; i < MAX_COUNT_CHANNEL; ++i)
        addChannel(std::make_unique<Channel>(getNameChannel(i)));
}

std::string getNameChannel(const int number)
{
    return std::string("channel" + std::to_string(number));
}

void Multimetr::addChannel(std::unique_ptr<Channel> channel)
{
    if(channels.empty()){
        channels.insert(std::make_pair(channel->getName(),std::move(channel)));
        hintToInsertChannel = std::end(channels);
    }else
        hintToInsertChannel = channels.insert(hintToInsertChannel,std::make_pair(channel->getName(),std::move(channel)));
}

void Multimetr::runChannel(std::shared_ptr<Channel> channel)
{

    if(tasksChannels.empty()){
        tasksChannels.insert(std::make_pair(channel->getName(),std::thread(&Channel::start, channel)));
        hintToInsertTask = std::end(tasksChannels);
    }
    else
        addChannelIntoSeparateThread(channel);
}

void Multimetr::addChannelIntoSeparateThread(std::shared_ptr<Channel> channel)
{
    if(findChannelInThread(channel->getName()))
        hintToInsertTask = tasksChannels.insert(hintToInsertTask, std::make_pair(channel->getName(),std::thread(&Channel::start,channel)));
}

bool Multimetr::findChannelInThread(const std::string &channel)
{
    return tasksChannels.find(channel) == tasksChannels.end();
}

std::shared_ptr<Channel> Multimetr::findChannel(const std::string &name)
{
    return channels.find(name)->second;
}

std::string Multimetr::executeCommand(std::string &name)
{
    std::string resultExecute;

    switch(determineCommand(name))
    {
        case TypeCommand::START_OF_MEASURE:
            startMeasure->execute(getParameters(name));
            resultExecute = startMeasure->answer();
            break;
        case TypeCommand::SET_RANGE:
            setRange->execute(getParameters(name));
            resultExecute = setRange->answer();
            break;
        case TypeCommand::STOP_OF_MEASURE:
            stopMeasure->execute(getParameters(name));
            resultExecute = stopMeasure->answer();
            break;
        case TypeCommand::STATUS_OF_DEVICE:
            status->execute(getParameters(name));
            resultExecute = status->answer();
            break;
        case TypeCommand::RESULT_OF_MEASURE:
            resultMeasure->execute(getParameters(name));
            resultExecute = resultMeasure->answer();
            break;
        default:
            break;
    }

    return resultExecute;
}

TypeCommand Multimetr::determineCommand(const std::string& name)
{

    if(name.find("start_measure") != std::string::npos)
        return TypeCommand::START_OF_MEASURE;
    else if(name.find("set_range") != std::string::npos)
        return TypeCommand::SET_RANGE;
    else if(name.find("stop_measure") != std::string::npos)
        return TypeCommand::STOP_OF_MEASURE;
    else if(name.find("get_status") != std::string::npos)
        return TypeCommand::STATUS_OF_DEVICE;
    else if(name.find("get_result") != std::string::npos)
        return TypeCommand::RESULT_OF_MEASURE;
    else
        return TypeCommand::NO_COMMAND;

}

std::string Multimetr::getParameters(std::string &name)
{
    std::string _template("\\w+\\s+(channel\\d{1}|channel\\d{1},+\\s+range[0-4]{1})?");

    std::regex regExp(_template.data());
    std::smatch match;
    std::string str;
    if(std::regex_match(name,match,regExp))
        str = match[1].first.base();
    else
        str = "";

    return str;
}

void Multimetr::completeExecutionAllChannels()
{
    if(!tasksChannels.empty())
    {
        for(auto& [nameChannel, threadChannel] : tasksChannels)
        {
            auto channel = channels.find(nameChannel);

            if(channel != channels.end()){
                channel->second->setStatus(Status::EXIT);
                threadChannel.join();
            }
        }
    }
}
