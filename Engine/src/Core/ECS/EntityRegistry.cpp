#include "EntityRegistry.hpp"

#include "Components.hpp"
#include "Entity.hpp"
#include "Core/Scene/Scene.hpp"

namespace SW {

	EntityRegistry::EntityRegistry(Scene* scene)
		: m_Scene(scene) {}

	Entity EntityRegistry::CreateEntity()
	{
        Entity entity{ s_EnttHandle.create(), m_Scene };

        entity.AddComponent<IDComponent>();

        return entity;
    }

    void EntityRegistry::DestroyEntity(Entity entity)
	{
        s_EnttHandle.destroy(entity);
    }

    void EntityRegistry::DestroyAllEntities()
    {
		s_EnttHandle.clear();
    }

}
