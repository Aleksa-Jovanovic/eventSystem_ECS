#include "ECSChannel.h"

ECSChannel& ECSChannel::GetChannel()
{
    static ECSChannel channel("ECSChannel", ChannelType::ECS_CHANNEL);
    return channel;
}