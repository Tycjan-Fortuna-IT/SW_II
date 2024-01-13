#include "EntityRegistry.hpp"

#include "Components.hpp"
#include "Entity.hpp"

namespace SW {

    entt::registry EntityRegistry::s_EnttHandle = {};

    Entity EntityRegistry::CreateEntity()
	{
        Entity entity{ s_EnttHandle.create() };

        entity.AddComponent<IDComponent>();

        return entity;
    }

    void EntityRegistry::DestroyEntity(Entity entity)
	{
        s_EnttHandle.destroy(entity);
    }

}
