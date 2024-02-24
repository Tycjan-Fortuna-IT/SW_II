#include "Scene.hpp"

#include <entt.hpp>
#include <box2d/box2d.h>

#include "Core/ECS/Components.hpp"
#include "Core/Renderer/Camera.hpp"
#include "Core/Renderer/Renderer2D.hpp"
#include "Core/Utils/Random.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Editor/EditorCamera.hpp"
#include "Core/Physics/Physics2DContactListener.hpp"

namespace SW {

	Scene::Scene(const std::string& filepath)
		: m_Registry(this), m_FilePath(filepath) {}
	
	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tag /*= "Entity"*/)
	{
		return CreateEntityWithID(CreateID(), tag);
	}

	Entity Scene::CreateEntityWithID(u64 id, const std::string& tag /*= "Entity"*/)
	{
		Entity entity = { m_Registry.GetRegistryHandle().create(), this };

		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<RelationshipComponent>();

		m_EntityMap[id] = (entt::entity)entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		entity.RemoveParent();

		for (u64 childId : entity.GetRelations().ChildrenIDs) {
			Entity childToDelete = GetEntityByID(childId);

			DestroyEntity(childToDelete);
		}

		m_EntityMap.erase(entity.GetID());
		m_Registry.DestroyEntity(entity);
	}

    void Scene::DestroyAllEntities()
    {
		m_Registry.DestroyAllEntities();

		m_EntityMap.clear();
    }

	bool Scene::BeginRendering(EditorCamera* camera)
	{
		switch (m_SceneState) {
			case SW::SceneState::Edit:
				Renderer2D::BeginScene(camera); break;
			case SW::SceneState::Play: {
				glm::mat4 cameraTransform;
				SceneCamera* mainCamera = nullptr;

				for (auto&& [handle, tc, cc] : m_Registry.GetEntitiesWith<TransformComponent, CameraComponent>().each()) {
					Entity entity = { handle, this };

					if (cc.Primary) {
						mainCamera = &cc.Camera;
						cameraTransform = entity.GetWorldSpaceTransformMatrix();

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

		m_PhysicsWorld2D = new b2World({ Gravity.x, Gravity.y });
		m_PhysicsContactListener2D = new Physics2DContactListener(this);
		m_PhysicsWorld2D->SetContactListener(m_PhysicsContactListener2D);

		for (auto&& [handle, rbc] : m_Registry.GetEntitiesWith<RigidBody2DComponent>().each()) {
			Entity entity = { handle, this };

			CreateRigidbody2D(entity, entity.GetWorldSpaceTransform(), rbc);
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld2D;
		delete m_PhysicsContactListener2D;
	}

	void Scene::OnUpdate(Timestep dt)
	{
		if (m_SceneState == SceneState::Play)
			this->OnUpdateRuntime(dt);
		else
			this->OnUpdateEditor(dt);
	}

    void Scene::OnUpdateEditor(Timestep dt)
    {
		for (auto&& [handle, sc] : m_Registry.GetEntitiesWith<SpriteComponent>().each()) {
			Entity entity = { handle, this };

			Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), sc, (int)handle);
		}

		for (auto&& [handle, cc] : m_Registry.GetEntitiesWith<CircleComponent>().each()) {
			Entity entity = { handle, this };

			Renderer2D::DrawCircle(entity.GetWorldSpaceTransformMatrix(), cc, (int)handle);
		}
	}

	void Scene::OnUpdateRuntime(Timestep dt)
	{
#pragma region Physics
		constexpr f32 physicsStepRate = 50.0f;
		constexpr f32 physicsTs = 1.0f / physicsStepRate;

		m_PhysicsFrameAccumulator += dt;

		while (m_PhysicsFrameAccumulator >= physicsTs) {
			m_PhysicsContactListener2D->Step(physicsTs);
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

		for (auto&& [handle, sc] : m_Registry.GetEntitiesWith<SpriteComponent>().each()) {
			Entity entity = { handle, this };

			Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), sc, (int)handle);
		}

		for (auto&& [handle, cc] : m_Registry.GetEntitiesWith<CircleComponent>().each()) {
			Entity entity = { handle, this };

			Renderer2D::DrawCircle(entity.GetWorldSpaceTransformMatrix(), cc, (int)handle);
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

	template <typename C>
	inline static void CopyComponent(Scene* scene, EntityRegistry& sourceRegistry, entt::registry& destinationRegistryHandle)
	{
		for (auto&& [handle, idc, component] : sourceRegistry.GetEntitiesWith<IDComponent, C>().each()) {
			C& componentToCopy = scene->GetEntityByID(idc.ID).GetComponent<C>();
			Entity destination = scene->GetEntityByID(idc.ID);

			destinationRegistryHandle.emplace_or_replace<C>(destination, componentToCopy);
		}
	}

    Scene* Scene::DeepCopy()
    {
		Scene* copy = new Scene(m_FilePath);

		entt::registry& copyRegistry = copy->GetRegistry().GetRegistryHandle();

		for (auto&& [handle, idc, tc] : m_Registry.GetEntitiesWith<IDComponent, TagComponent>().each()) {
			Entity current = { handle, this };
			RelationshipComponent& currentRelations = current.GetRelations();

			Entity copied = copy->CreateEntityWithID(idc.ID, tc.Tag);
			RelationshipComponent& copiedRelations = copied.GetRelations();

			copiedRelations.ParentID = currentRelations.ParentID;

			for (u64 childId : currentRelations.ChildrenIDs) {
				copiedRelations.ChildrenIDs.emplace_back(childId);
			}

			copyRegistry.emplace_or_replace<RelationshipComponent>(copied, copiedRelations);
		}

		CopyComponent<TransformComponent>(this, m_Registry, copyRegistry);
		CopyComponent<SpriteComponent>(this, m_Registry, copyRegistry);
		CopyComponent<CircleComponent>(this, m_Registry, copyRegistry);
		CopyComponent<CameraComponent>(this, m_Registry, copyRegistry);
		CopyComponent<RigidBody2DComponent>(this, m_Registry, copyRegistry);
		CopyComponent<BoxCollider2DComponent>(this, m_Registry, copyRegistry);
		CopyComponent<CircleCollider2DComponent>(this, m_Registry, copyRegistry);
		CopyComponent<BuoyancyEffector2DComponent>(this, m_Registry, copyRegistry);

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

		if (entity.HasComponent<CircleCollider2DComponent>()) {
			CreateCircleCollider2D(entity, tc, rbc, entity.GetComponent<CircleCollider2DComponent>());
		}
	}

	void Scene::CreateBoxCollider2D(Entity entity, const TransformComponent& tc, const RigidBody2DComponent& rbc, BoxCollider2DComponent& bcc) const
	{
		b2PolygonShape boxShape;
		boxShape.SetAsBox(bcc.Size.x * tc.Scale.x, bcc.Size.y * tc.Scale.y, { bcc.Offset.x, bcc.Offset.y }, 0.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.userData.pointer = static_cast<u32>(entity);
		fixtureDef.density = bcc.Density;
		fixtureDef.friction = rbc.Friction;
		fixtureDef.restitution = rbc.Restitution;
		fixtureDef.restitutionThreshold = rbc.RestitutionThreshold;
		fixtureDef.isSensor = bcc.IsSensor;

		b2Body* body = static_cast<b2Body*>(rbc.Handle);
		b2Fixture* fixture = body->CreateFixture(&fixtureDef);

		bcc.Handle = fixture;
	}

	void Scene::CreateCircleCollider2D(Entity entity, const TransformComponent& tc, const RigidBody2DComponent& rbc, CircleCollider2DComponent& ccc) const
	{
		b2CircleShape circleShape;
		circleShape.m_radius = ccc.Radius * glm::max(tc.Scale.x, tc.Scale.y);
		circleShape.m_p = { ccc.Offset.x, ccc.Offset.y };

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circleShape;
		fixtureDef.userData.pointer = static_cast<u32>(entity);
		fixtureDef.density = ccc.Density;
		fixtureDef.friction = rbc.Friction;
		fixtureDef.restitution = rbc.Restitution;
		fixtureDef.restitutionThreshold = rbc.RestitutionThreshold;
		fixtureDef.isSensor = ccc.IsSensor;

		b2Body* body = static_cast<b2Body*>(rbc.Handle);
		b2Fixture* fixture = body->CreateFixture(&fixtureDef);

		ccc.Handle = fixture;
	}

}
