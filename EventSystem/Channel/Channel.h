#pragma once

// One Channel can have more then one event using it
// In hashMap we will store for which events what objects listen
#include <algorithm>
#include <string> 
#include <vector>
#include <list>
#include <memory> 

#include "../EventHandler.h"
#include "../EventThreadPool.h"

class Event;
class EventThreadPool;

enum class ChannelType
{
    BASIC_CHANNEL,
    UI_CHANNEL,
    ECS_CHANNEL,
    FRAME_DEPENDENT,
    FRAME_INDEPENDENT
};

template<typename EventClass>
using Ptr = EventHandler<EventClass>*;

class Channel 
{
public:
    Channel() = delete;
    Channel(const Channel&) = delete;
    Channel(Channel&&) = delete;

    static Channel& GetChannel();

    /* Creating eventHandler vectors for each EventClass used */
    template<typename EventClass>
    std::vector<Ptr<EventClass>>& GetEventHandler();

    /* Subscribe eventHandler */
    template<typename EventClass>
    void SubscribeEventHandler(EventHandler<EventClass>* eventHandler);

    /* UnSubscribe eventHandler */
    template<typename EventClass>
    bool UnSubscribeEventHandler(EventHandler<EventClass>* eventHandlerToRemove);

    /* Immediately resolve dispatched event */
    template<typename EventClass>
    void DispatchEventImmediately(EventClass* dispatchedEvent);

    /* Currently not working */
    void ProcessEvents(); // PROBLEM
    /* Currently not working */
    void DispatchEvent(Event* dispatchedEvent); // PROBLEM

protected:
    Channel(std::string channelName, ChannelType channelType);
    
private:
    /* Template method to INVOKE single event, you don't need to specify which eventClass it is,
        it can deduce that from event param */
    template<typename EventClass>
    void InvokeEvent(EventClass* event);

    std::string m_Name;
    ChannelType m_ChannelType;

    /* Currently not used */
    std::list<Event*> m_DispatchedEventList;

    EventThreadPool m_ThreadPool;
};



template<typename EventClass>
std::vector<Ptr<EventClass>>& Channel::GetEventHandler()
{
    static std::vector<Ptr<EventClass>> s_EventHandlers{};
    return s_EventHandlers;
}

template<typename EventClass>
void Channel::SubscribeEventHandler(EventHandler<EventClass>* eventHandler)
{
    GetEventHandler<EventClass>().push_back(Ptr<EventClass>(eventHandler));
}

template<typename EventClass>
bool Channel::UnSubscribeEventHandler(EventHandler<EventClass>* eventHandlerToRemove)
{
    std::vector<Ptr<EventClass>>& handlerVector = GetEventHandler<EventClass>();
    std::vector<Ptr<EventClass>>::iterator vectorItr = handlerVector.end();

    vectorItr = find_if(handlerVector.begin(), handlerVector.end(),
        [&](Ptr<EventClass>& uniquePtr) {return uniquePtr->GetEventHandlerID() == eventHandlerToRemove->GetEventHandlerID(); });

    if (vectorItr != handlerVector.end())
    {
        handlerVector.erase(vectorItr);
        return true;
    }

    return false;
}

template<typename EventClass>
void Channel::DispatchEventImmediately(EventClass* dispatchedEvent)
{
    InvokeEvent(dispatchedEvent);
}

template<typename EventClass>
void Channel::InvokeEvent(EventClass* event)
{
    //std::vector<std::unique_ptr<EventHandler<EventClass>>>& a = GetEventHandler<EventClass>();
    for (auto& eventHandler : GetEventHandler<EventClass>())
    {
        eventHandler->Call(event);
    }

    delete event;
}