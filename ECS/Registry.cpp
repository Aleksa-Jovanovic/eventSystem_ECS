#include "Registry.h"
#include "../EventSystem/Event/EntityDestroyedEvent.h"
#include "../EventSystem/Channel/ECSChannel.h"

#include <Windows.h>
#include <iostream>
#include <sstream>

Registry& Registry::GetRegistry()
{
    static Registry s_Registry{};
    return s_Registry;
}

Registry::Registry()
{
    for (Entity newEntityID = 1; newEntityID <= MAX_ENTITIES; newEntityID++)
    {
        m_AvailableEntities.push(newEntityID);
    }

    m_LiveEntityCount = 0;
}

Entity Registry::CreateEnity()
{
    if (m_LiveEntityCount == MAX_ENTITIES)
    {
        std::wostringstream os_;
        os_ << "[ ALERT ] => Max entityCount reached "<<std::endl;
        OutputDebugStringW(os_.str().c_str());

        return MAX_ENTITIES;
    }

    Entity newEntityID = m_AvailableEntities.front();
    m_AvailableEntities.pop();
    m_LiveEntityCount++;

    return newEntityID;
}

void Registry::DestroyEntity(Entity entity)
{
    if (entity >= MAX_ENTITIES)
    {
        std::wostringstream os_;
        os_ << "[ ALERT ] => EntityID out of scope " << std::endl;
        OutputDebugStringW(os_.str().c_str());
        return;
    }

    m_AvailableEntities.push(entity);
    m_LiveEntityCount--;

    EntityDestroyedEvent* event = new EntityDestroyedEvent(entity);
    ECSChannel::GetChannel().DispatchEventImmediately(event);
}