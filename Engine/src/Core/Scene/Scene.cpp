#include "Scene.hpp"

#include <entt.hpp>
#include <box2d/box2d.h>

#include "Core/ECS/Components.hpp"
#include "Core/Renderer/Camera.hpp"
#include "Core/Renderer/Renderer2D.hpp"
#include "Core/Utils/Random.hpp"
#include "Core/ECS/Entity.hpp"

namespace SW {

	Scene::Scene()
		: m_Registry(this) {}
	
	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tag /*= "Entity"*/)
	{
		Entity entity = { m_Registry.GetRegistryHandle().create(), this };

		u64 id = CreateID();

		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();

		m_EntityMap[id] = (entt::entity)entity;

		return entity;
	}

	Entity Scene::CreateEntityWithID(u64 id, const std::string& tag /*= "Entity"*/)
	{
		Entity entity = { m_Registry.GetRegistryHandle().create(), this };

		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();

		m_EntityMap[id] = (entt::entity)entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.DestroyEntity(entity);
	}

    void Scene::DestroyAllEntities()
    {
		m_Registry.DestroyAllEntities();

		m_EntityMap.clear();
    }

	bool Scene::BeginRendering(SceneCamera* camera)
	{
		switch (m_SceneState) {
			case SW::SceneState::Edit:
				Renderer2D::BeginScene(*camera); break;
			case SW::SceneState::Play: {
				glm::mat4 cameraTransform;
				SceneCamera* mainCamera = nullptr;

				for (auto&& [handle, tc, cc] : m_Registry.GetEntitiesWith<TransformComponent, CameraComponent>().each()) {
					if (cc.Primary) {
						mainCamera = &cc.Camera;
						cameraTransform = tc.GetTransform();

						break;
					}
				}

				if (!mainCamera)
					return false;

				Renderer2D::BeginScene(*mainCamera, cameraTransform);

				break;
			}
			case SW::SceneState::Pause:
				SW_FATAL("Not yet implemented!"); break;
			case SW::SceneState::Simulate:
				SW_FATAL("Not yet implemented!"); break;
		}

		return true;
	}

	void Scene::EndRendering()
	{
		Renderer2D::EndScene();
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsFrameAccumulator = 0.0f;

		m_PhysicsWorld2D = new b2World({ m_Gravity.x, m_Gravity.y });

		for (auto&& [handle, tc, rbc] : m_Registry.GetEntitiesWith<TransformComponent, RigidBody2DComponent>().each()) {
			CreateRigidbody2D({ handle, this }, tc, rbc);
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld2D;
	}

	void Scene::OnUpdate(Timestep dt, const SceneCamera& camera)
	{
		if (m_SceneState == SceneState::Play)
			this->OnUpdateRuntime(dt);
		else
			this->OnUpdateEditor(dt, camera);
	}

    void Scene::OnUpdateEditor(Timestep dt, const SceneCamera& camera)
    {
		for (auto&& [handle, tc, sc] : m_Registry.GetEntitiesWith<TransformComponent, SpriteComponent>().each()) {
			Renderer2D::DrawQuad(tc.GetTransform(), sc, (int)handle);
		}
	}

	void Scene::OnUpdateRuntime(Timestep dt)
	{
#pragma region Physics
		constexpr f32 physicsStepRate = 50.0f;
		constexpr f32 physicsTs = 1.0f / physicsStepRate;

		m_PhysicsFrameAccumulator += dt;

		while (m_PhysicsFrameAccumulator >= physicsTs) {
			m_PhysicsWorld2D->Step(physicsTs, static_cast<int32_t>(m_VelocityIterations), static_cast<int32_t>(m_PositionIterations));

			m_PhysicsFrameAccumulator -= physicsTs;
		}

		for (auto&& [handle, tc, rbc] : m_Registry.GetEntitiesWith<TransformComponent, RigidBody2DComponent>().each()) {
			const b2Body* body = static_cast<b2Body*>(rbc.Handle);

			if (!body->IsAwake())
				continue;

			const b2Vec2 position = body->GetPosition();

			tc.Position.x = position.x;
			tc.Position.y = position.y;
			tc.Rotation.z = body->GetAngle();
		}

#pragma endregion

		for (auto&& [entity, tc, sc] : m_Registry.GetEntitiesWith<TransformComponent, SpriteComponent>().each()) {
			Renderer2D::DrawQuad(tc.GetTransform(), sc, (int)entity);
		}
	}

	void Scene::OnViewportResize(u32 width, u32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		for (auto&& [entity, cc] : m_Registry.GetEntitiesWith<CameraComponent>().each()) {
			cc.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetEntityByID(u64 id)
	{
		if (m_EntityMap.find(id) != m_EntityMap.end())
			return { m_EntityMap.at(id), this };

		ASSERT(false, "Entity not found!"); // to do allow in ASSERTS to format strings

		return {};
	}

    Scene* Scene::DeepCopy()
    {
		Scene* copy = new Scene();

		for (auto&& [handle, idc, tc] : m_Registry.GetEntitiesWith<IDComponent, TagComponent>().each()) {
			copy->CreateEntityWithID(idc.ID, tc.Tag);
		}

		entt::registry& registry = copy->GetRegistry().GetRegistryHandle();

		// TODO use some more generic way
		for (auto&& [handle, idc, tc] : m_Registry.GetEntitiesWith<IDComponent, TransformComponent>().each()) {
			TransformComponent& componentToCopy = GetEntityByID(idc.ID).GetComponent<TransformComponent>();
			Entity destination = GetEntityByID(idc.ID);

			registry.emplace_or_replace<TransformComponent>(destination, componentToCopy);
		}

		for (auto&& [handle, idc, tc] : m_Registry.GetEntitiesWith<IDComponent, SpriteComponent>().each()) {
			SpriteComponent& componentToCopy = GetEntityByID(idc.ID).GetComponent<SpriteComponent>();
			Entity destination = GetEntityByID(idc.ID);

			registry.emplace_or_replace<SpriteComponent>(destination, componentToCopy);
		}

		for (auto&& [handle, idc, tc] : m_Registry.GetEntitiesWith<IDComponent, CameraComponent>().each()) {
			CameraComponent& componentToCopy = GetEntityByID(idc.ID).GetComponent<CameraComponent>();
			Entity destination = GetEntityByID(idc.ID);

			registry.emplace_or_replace<CameraComponent>(destination, componentToCopy);
		}

		for (auto&& [handle, idc, tc] : m_Registry.GetEntitiesWith<IDComponent, RigidBody2DComponent>().each()) {
			RigidBody2DComponent& componentToCopy = GetEntityByID(idc.ID).GetComponent<RigidBody2DComponent>();
			Entity destination = GetEntityByID(idc.ID);

			registry.emplace_or_replace<RigidBody2DComponent>(destination, componentToCopy);
		}

		for (auto&& [handle, idc, tc] : m_Registry.GetEntitiesWith<IDComponent, BoxCollider2DComponent>().each()) {
			BoxCollider2DComponent& componentToCopy = GetEntityByID(idc.ID).GetComponent<BoxCollider2DComponent>();
			Entity destination = GetEntityByID(idc.ID);

			registry.emplace_or_replace<BoxCollider2DComponent>(destination, componentToCopy);
		}

		return copy;
    }

    void Scene::CreateRigidbody2D(Entity entity, const TransformComponent& tc, RigidBody2DComponent& rbc) const
	{
		b2BodyDef definition;
		definition.type = static_cast<b2BodyType>(rbc.Type);
		definition.fixedRotation = false;
		definition.allowSleep = rbc.AllowSleep;
		definition.gravityScale = rbc.GravityScale;
		definition.position.Set(tc.Position.x, tc.Position.y);
		definition.angle = tc.Rotation.z;

		b2Body* rb = m_PhysicsWorld2D->CreateBody(&definition);

		b2MassData massData = rb->GetMassData();
		massData.mass = 1.f;
		
		rb->SetMassData(&massData);
		rbc.Handle = rb;

		if (entity.HasComponent<BoxCollider2DComponent>()) {
			CreateBoxCollider2D(entity, tc, rbc, entity.GetComponent<BoxCollider2DComponent>());
		}
	}

	void Scene::CreateBoxCollider2D(Entity entity, const TransformComponent& tc, const RigidBody2DComponent& rbc, BoxCollider2DComponent& bcc) const
	{
		b2PolygonShape boxShape;
		boxShape.SetAsBox(bcc.Size.x * tc.Scale.x, bcc.Size.y * tc.Scale.y, { bcc.Offset.x, bcc.Offset.y }, 0.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.userData.pointer = static_cast<u32>(entity);
		fixtureDef.density = 1.f;
		fixtureDef.friction = 0.5f;
		fixtureDef.restitution = 0.f;
		fixtureDef.restitutionThreshold = 0.5f;

		b2Body* body = static_cast<b2Body*>(rbc.Handle);
		b2Fixture* fixture = body->CreateFixture(&fixtureDef);

		bcc.Handle = fixture;
	}

}
