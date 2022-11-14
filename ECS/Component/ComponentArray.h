#pragma once
//#include "../Entity.h"

#include <array>
#include <vector>

#include "../Entity.h"
#include "../../EventSystem/Event/EntityDestroyedEvent.h"
#include "../../EventSystem/Channel/ECSChannel.h"

template<typename ComponentType>
class ComponentArray
{
public:
    ComponentArray();
    ~ComponentArray();

    void BindComponentToEntity(Entity entity, ComponentType component);
    void UnBindComponent(Entity entity);

    ComponentType& GetComponent(Entity entity);
 
private:
    // When entity gets removed all of its components need to be removed as well
    void HandleEntityDestroyedEvent(EntityDestroyedEvent& event);
    EventCallback<ComponentArray<ComponentType>, EntityDestroyedEvent> m_EntityDestroyedEventCallback;

    std::array<ComponentType, MAX_ENTITIES> m_ComponentArray;
    std::vector<Entity> m_EnityIndexMapping;
    uint32_t m_ValidEntriesCount;
};