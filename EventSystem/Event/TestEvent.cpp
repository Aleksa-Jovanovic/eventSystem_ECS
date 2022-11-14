#include "TestEvent.h"

int TestEvent::testEventID = 0;

TestEvent* TestEvent::GetDerivedPointerToThisEvent()
{ 
    return this; 
}
