#include "Scene.hpp"

#include <entt.hpp>

#include "Core/ECS/Components.hpp"
#include "Core/Renderer/Camera.hpp"
#include "Core/Renderer/Renderer2D.hpp"
#include "Core/Utils/Random.hpp"

namespace SW {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tag /*= "Entity"*/)
	{
		Entity entity = { EntityRegistry::GetRegistryHandle().create() };

		u64 id = CreateID();

		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();

		m_EntityMap[id] = (entt::entity)entity;

		return entity;
	}

	Entity Scene::CreateEntityWithID(u64 id, const std::string& tag /*= "Entity"*/)
	{
		Entity entity = { EntityRegistry::GetRegistryHandle().create() };

		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();

		m_EntityMap[id] = (entt::entity)entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		EntityRegistry::GetRegistryHandle().destroy(entity);
	}

    void Scene::DestroyAllEntities()
    {
		EntityRegistry::DestroyAllEntities();

		m_EntityMap.clear();
    }

    void Scene::OnUpdate(Timestep dt, const SceneCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		for (auto&& [entity, tc, sc] : EntityRegistry::GetEntitiesWith<TransformComponent, SpriteComponent>().each()) {
			Renderer2D::DrawQuad(tc.GetTransform(), sc);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(u32 width, u32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		for (auto&& [entity, cc] : EntityRegistry::GetEntitiesWith<CameraComponent>().each()) {
			cc.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetEntityByID(u64 id) const
	{
		if (m_EntityMap.find(id) != m_EntityMap.end())
			return { m_EntityMap.at(id) };

		ASSERT(false, "Entity not found!"); // to do allow in ASSERTS to format strings

		return {};
	}

}
