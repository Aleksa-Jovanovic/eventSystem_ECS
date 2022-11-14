#pragma once

#include "Channel.h"

class ECSChannel : public Channel
{
public:
    static ECSChannel& GetChannel();

private:
    ECSChannel(std::string channelName, ChannelType channelType) : Channel(channelName, channelType) {};
};