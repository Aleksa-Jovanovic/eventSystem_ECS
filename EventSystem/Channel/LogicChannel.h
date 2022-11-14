#pragma once

#include "Channel.h"

class LogicChannel : public Channel
{
public:
    static LogicChannel& GetChannel();

private:
    LogicChannel(std::string channelName, ChannelType channelType) : Channel(channelName, channelType) {};
};