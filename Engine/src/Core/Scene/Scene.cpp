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

	void Scene::OnUpdate(Timestep dt, const SceneCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		const auto& group = EntityRegistry::Get().group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group) {
			const auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

			Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
		}

		Renderer2D::EndScene();
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
