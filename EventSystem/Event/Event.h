#pragma once

//#include "EventEnum.h"

class Channel;

class Event
{
public:
    Event() = default;

    // Override it to return your EventClass*
    virtual Event* GetDerivedPointerToThisEvent() = 0;

    //void SendTo(IEventHandler* target);
};