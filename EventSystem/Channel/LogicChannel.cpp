#include "LogicChannel.h"

LogicChannel& LogicChannel::GetChannel()
{
    static LogicChannel channel("LogicChannel", ChannelType::FRAME_DEPENDENT);
    return channel;
}