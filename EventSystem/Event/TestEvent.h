#pragma once
#include "Event.h"

class TestEvent : public Event
{
public:
    TestEvent() { currentTestEvent = testEventID++; }

    TestEvent* GetDerivedPointerToThisEvent() override;

    int GetCurrentTestEventID() const { return currentTestEvent; }

private:
    static int testEventID;
    int currentTestEvent = 0;
};

