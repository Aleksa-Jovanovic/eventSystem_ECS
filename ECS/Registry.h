#ifndef REGISTRY__H__
#define  REGISTRY__H__

#include <stdint.h>
#include <queue>

#include "Entity.h"
#include "Component/Component.h"
#include "Component/ComponentArray.h"

#define gameRegistry Registry::GetRegistry()

class Registry
{
public:
    static Registry& GetRegistry();

    Entity CreateEnity();
    void DestroyEntity(Entity entity);

    template<typename ComponentType>
    void AddComponentToEntity(Entity entity);
    template<typename ComponentType>
    void AttachComponentToEntity(Entity entity, ComponentType component);
    template<typename ComponentType>
    void DetachComponentFromEntity(Entity entity);
    template<typename ComponentType>
    ComponentType& GetComponentForEntity(Entity entity);

    template<typename ComponentType>
    ComponentArray<ComponentType>& GetComponentArray()
    {
        static ComponentArray<ComponentType> s_ComponentArray{};
        return s_ComponentArray;
    }

private:
    Registry();

    // Queue of unused entities
    std::queue<Entity> m_AvailableEntities{};
    uint32_t m_LiveEntityCount;
};

template<typename ComponentType>
void Registry::AddComponentToEntity(Entity entity)
{
    ComponentType newComponent;
    GetComponentArray<ComponentType>().BindComponentToEntity(entity, newComponent);
}

template<typename ComponentType>
void Registry::AttachComponentToEntity(Entity entity, ComponentType component)
{
    GetComponentArray<ComponentType>().BindComponentToEntity(entity, component);
}

template<typename ComponentType>
void Registry::DetachComponentFromEntity(Entity entity)
{
    GetComponentArray<ComponentType>().UnBindComponent(entity);
}

template<typename ComponentType>
ComponentType& Registry::GetComponentForEntity(Entity entity)
{
    return GetComponentArray<ComponentType>().GetComponent(entity);
}

#endif // REGISTRY__H__