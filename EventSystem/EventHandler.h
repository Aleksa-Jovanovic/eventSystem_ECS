#pragma once

class Event;

//Base EventHnadler class so I can put it in HashMap
class IEventHandler
{
public:
    int GetEventHandlerID() const { return m_EventHandlerID; }

private:
    static int s_EventHandlerID;
    int m_EventHandlerID = ++s_EventHandlerID;
};

template<class EVENT_CLASS>
class EventHandler : public IEventHandler
{
public:
    virtual void Call(Event* event) = 0;
    virtual bool IsInitialized() const = 0;
};

template<class LISTENER_CLASS, class EVENT_CLASS>
class EventCallback : public EventHandler<EVENT_CLASS>
{
public:
    typedef void(LISTENER_CLASS::*CallbackPtr)(EVENT_CLASS&);

    EventCallback(LISTENER_CLASS* listener = nullptr) :
        m_Listener(listener){};

    void Init(LISTENER_CLASS* listener, CallbackPtr function)
    {
        this->m_Listener = listener;
        this->m_Callback = function;
    }

    bool IsInitialized() const override { return this->m_Listener != nullptr && m_Callback != nullptr; }

    void Call(Event* event) override
    {
        if (IsInitialized())
        {
            (this->m_Listener->*m_Callback)(*(static_cast<EVENT_CLASS*>(event)));
        }
    }

private:
    LISTENER_CLASS* m_Listener;
    CallbackPtr m_Callback;
};