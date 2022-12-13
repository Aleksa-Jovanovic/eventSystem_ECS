#pragma once
//#include "../Entity.h"

#include <array>
#include <vector>
#include <algorithm>

#include "../Entity.h"
#include "../../EventSystem/Event/EntityDestroyedEvent.h"
#include "../../EventSystem/Channel/ECSChannel.h"

#define EXPAND_SIZE MAX_ENTITIES / 10

template<typename ComponentType>
class ComponentArray
{
public:
    ComponentArray();
    ~ComponentArray();

    void BindComponentToEntity(Entity entity, ComponentType component);
    void UnBindComponent(Entity entity);

    ComponentType* GetComponent(Entity entity);
 
private:
    // When entity gets removed all of its components need to be removed as well
    void HandleEntityDestroyedEvent(EntityDestroyedEvent& event)
    {
        if (std::find(m_EnityIndexMapping.begin(), m_EnityIndexMapping.end(), event.GetDestroyedEntity()) != m_EnityIndexMapping.end())
        {
            UnBindComponent(event.GetDestroyedEntity());
        }
    }
    EventCallback<ComponentArray<ComponentType>, EntityDestroyedEvent> m_EntityDestroyedEventCallback;

    std::array<ComponentType, MAX_ENTITIES> m_ComponentArray;
    std::vector<Entity> m_EnityIndexMapping;
    uint32_t m_ValidEntriesCount;
};

template<typename ComponentType>
ComponentArray<ComponentType>::ComponentArray()
{
    m_ValidEntriesCount = 0;
    m_EnityIndexMapping.reserve(EXPAND_SIZE);

    m_EntityDestroyedEventCallback.Init(this, &ComponentArray<ComponentType>::HandleEntityDestroyedEvent);
    ECSChannel::GetChannel().SubscribeEventHandler(&m_EntityDestroyedEventCallback);
}

template<typename ComponentType>
ComponentArray<ComponentType>::~ComponentArray()
{
    ECSChannel::GetChannel().UnSubscribeEventHandler(&m_EntityDestroyedEventCallback);
}

template<typename ComponentType>
void ComponentArray<ComponentType>::BindComponentToEntity(Entity entity, ComponentType component)
{
    auto mapperItr = find(m_EnityIndexMapping.begin(), m_EnityIndexMapping.end(), entity);
    if (mapperItr != m_EnityIndexMapping.end())
    {
        //Logger::Warning("Component for entity (", entity, ") already exist", " <-> ", __FUNCTION__);
    }

    // If vector is full we need to increase the size;
    uint32_t newEntityIndex = m_ValidEntriesCount;
    if (m_EnityIndexMapping.size() <= newEntityIndex + 1)
    {
        uint32_t mapperSize = m_EnityIndexMapping.size();
        mapperSize += EXPAND_SIZE;
        m_EnityIndexMapping.resize(mapperSize);
    }

    m_EnityIndexMapping[newEntityIndex] = entity;
    m_ComponentArray[newEntityIndex] = component;
    m_ValidEntriesCount++;
}

template<typename ComponentType>
void ComponentArray<ComponentType>::UnBindComponent(Entity entity)
{
    auto mapperItr = find(m_EnityIndexMapping.begin(), m_EnityIndexMapping.end(), entity);
    if (mapperItr == m_EnityIndexMapping.end())
    {
        // Log component doesn't exist
        return;
    }

    uint32_t index = mapperItr - m_EnityIndexMapping.begin();
    m_EnityIndexMapping[index] = m_EnityIndexMapping[m_ValidEntriesCount - 1];
    m_EnityIndexMapping.pop_back();

    m_ComponentArray[index] = m_ComponentArray[m_ValidEntriesCount - 1];
    m_ValidEntriesCount--;
}

template<typename ComponentType>
ComponentType* ComponentArray<ComponentType>::GetComponent(Entity entity)
{
    std::vector<Entity>::iterator mapperItr = find(m_EnityIndexMapping.begin(), m_EnityIndexMapping.end(), entity);
    if (mapperItr == m_EnityIndexMapping.end())
    {
        // Log component doesn't exist
        assert
        return nullptr;
    }

    uint32_t componentIndex = (*mapperItr);
    return &(m_ComponentArray[componentIndex]);
}