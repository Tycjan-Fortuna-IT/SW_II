#include "Scene.hpp"

#include <entt.hpp>

#include "Core/ECS/Components.hpp"
#include "Core/Renderer/Camera.hpp"
#include "../Renderer/Renderer2D.hpp"

namespace SW {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity entity = { EntityRegistry::GetRegistry().create() };

		entity.AddComponent<IDComponent>();
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		EntityRegistry::GetRegistry().destroy(entity);
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

}
