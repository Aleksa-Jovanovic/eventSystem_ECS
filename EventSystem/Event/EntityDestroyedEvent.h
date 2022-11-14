#pragma once
#include <stdint.h>
#include "Event.h"

class EntityDestroyedEvent : public Event
{
public:
    EntityDestroyedEvent(uint32_t destroyedEntity) { m_DestroyedEntity = destroyedEntity; }

    EntityDestroyedEvent* GetDerivedPointerToThisEvent() override;

    uint32_t GetDestroyedEntity() const { return m_DestroyedEntity; }
private:
    uint32_t m_DestroyedEntity;
};
