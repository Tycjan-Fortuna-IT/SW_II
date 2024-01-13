#include "Scene.hpp"

#include <entt.hpp>

#include "Core/ECS/Components.hpp"

namespace SW {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity entity = { EntityRegistry::Get().create() };

		entity.AddComponent<IDComponent>();
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		EntityRegistry::Get().destroy(entity);
	}

	void Scene::OnUpdate(f32 dt)
	{

	}

	void Scene::OnViewportResize(u32 width, u32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = EntityRegistry::Get().view<CameraComponent>();

		for (auto entity : view) {
			CameraComponent& cameraComponent = view.get<CameraComponent>(entity);

			cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

}
