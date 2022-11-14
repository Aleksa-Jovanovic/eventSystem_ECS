#include "Channel.h"
#include "../Event/Event.h"

Channel::Channel(std::string channelName, ChannelType channelType) : m_Name(channelName), m_ChannelType(channelType)
{
    m_ThreadPool.Start();
}

Channel& Channel::GetChannel()
{
    static Channel channel("Channel", ChannelType::BASIC_CHANNEL);
    return channel;
}

void Channel::DispatchEvent(Event* dispatchedEvent)
{
    m_DispatchedEventList.push_back(dispatchedEvent);
}

void Channel::ProcessEvents()
{
    if (m_DispatchedEventList.size() == 0)
    {
        return;
    }

    std::list<Event*> eventsToRemove;

    for (Event* event : m_DispatchedEventList)
    {
        InvokeEvent((event->GetDerivedPointerToThisEvent()));
        eventsToRemove.push_back(event);
    }

    for (Event* event : eventsToRemove)
    {
        m_DispatchedEventList.remove(event);
    }
}