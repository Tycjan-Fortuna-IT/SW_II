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

		for (auto&& [handle, rbc, djc] : m_Registry.GetEntitiesWith<RigidBody2DComponent, DistanceJoint2DComponent>().each()) {
			Entity entity = { handle, this };

			CreateDistanceJoint2D(entity, rbc, djc);
		}

		for (auto&& [handle, rbc, rjc] : m_Registry.GetEntitiesWith<RigidBody2DComponent, RevolutionJoint2DComponent>().each()) {
			Entity entity = { handle, this };

			CreateRevolutionJoint2D(entity, rbc, rjc);
		}

		for (auto&& [handle, rbc, pjc] : m_Registry.GetEntitiesWith<RigidBody2DComponent, PrismaticJoint2DComponent>().each()) {
			Entity entity = { handle, this };

			CreatePrismaticJoint2D(entity, rbc, pjc);
		}

		for (auto&& [handle, rbc, sjc] : m_Registry.GetEntitiesWith<RigidBody2DComponent, SpringJoint2DComponent>().each()) {
			Entity entity = { handle, this };

			CreateSpringJoint2D(entity, rbc, sjc);
		}

		for (auto&& [handle, rbc, wjc] : m_Registry.GetEntitiesWith<RigidBody2DComponent, WheelJoint2DComponent>().each()) {
			Entity entity = { handle, this };

			CreateWheelJoint2D(entity, rbc, wjc);
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

		for (auto&& [handle, djc] : m_Registry.GetEntitiesWith<DistanceJoint2DComponent>().each()) {
			if (djc.RuntimeJoint) {
				b2Joint* joint = (b2Joint*)(djc.RuntimeJoint);

				if (joint->GetReactionForce(physicsStepRate).LengthSquared() > djc.BreakingForce * djc.BreakingForce) {
					m_PhysicsWorld2D->DestroyJoint(joint);

					djc.RuntimeJoint = nullptr;
				}
			}
		}

		for (auto&& [handle, rjc] : m_Registry.GetEntitiesWith<RevolutionJoint2DComponent>().each()) {
			if (rjc.RuntimeJoint) {
				b2Joint* joint = (b2Joint*)(rjc.RuntimeJoint);

				if (
					joint->GetReactionForce(physicsStepRate).LengthSquared() > rjc.BreakingForce * rjc.BreakingForce ||
					joint->GetReactionTorque(physicsStepRate) > rjc.BreakingTorque
				) {
					m_PhysicsWorld2D->DestroyJoint(joint);

					rjc.RuntimeJoint = nullptr;
				}
			}
		}

		for (auto&& [handle, pjc] : m_Registry.GetEntitiesWith<PrismaticJoint2DComponent>().each()) {
			if (pjc.RuntimeJoint) {
				b2Joint* joint = (b2Joint*)(pjc.RuntimeJoint);

				if (
					joint->GetReactionForce(physicsStepRate).LengthSquared() > pjc.BreakingForce * pjc.BreakingForce ||
					joint->GetReactionTorque(physicsStepRate) > pjc.BreakingTorque
				) {
					m_PhysicsWorld2D->DestroyJoint(joint);

					pjc.RuntimeJoint = nullptr;
				}
			}
		}

		for (auto&& [handle, sjc] : m_Registry.GetEntitiesWith<SpringJoint2DComponent>().each()) {
			if (sjc.RuntimeJoint) {
				b2Joint* joint = (b2Joint*)(sjc.RuntimeJoint);

				if (joint->GetReactionForce(physicsStepRate).LengthSquared() > sjc.BreakingForce * sjc.BreakingForce) {
					m_PhysicsWorld2D->DestroyJoint(joint);

					sjc.RuntimeJoint = nullptr;
				}
			}
		}

		for (auto&& [handle, wjc] : m_Registry.GetEntitiesWith<WheelJoint2DComponent>().each()) {
			if (wjc.RuntimeJoint) {
				b2Joint* joint = (b2Joint*)(wjc.RuntimeJoint);

				if (
					joint->GetReactionForce(physicsStepRate).LengthSquared() > wjc.BreakingForce * wjc.BreakingForce ||
					joint->GetReactionTorque(physicsStepRate) > wjc.BreakingTorque
				) {
					m_PhysicsWorld2D->DestroyJoint(joint);

					wjc.RuntimeJoint = nullptr;
				}
			}
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
		CopyComponent<PolygonCollider2DComponent>(this, m_Registry, copyRegistry);
		CopyComponent<BuoyancyEffector2DComponent>(this, m_Registry, copyRegistry);
		CopyComponent<DistanceJoint2DComponent>(this, m_Registry, copyRegistry);
		CopyComponent<RevolutionJoint2DComponent>(this, m_Registry, copyRegistry);
		CopyComponent<PrismaticJoint2DComponent>(this, m_Registry, copyRegistry);
		CopyComponent<SpringJoint2DComponent>(this, m_Registry, copyRegistry);
		CopyComponent<WheelJoint2DComponent>(this, m_Registry, copyRegistry);

		return copy;
    }

    void Scene::CreateRigidbody2D(Entity entity, const TransformComponent& tc, RigidBody2DComponent& rbc)
	{
		b2BodyDef definition;
		definition.type = static_cast<b2BodyType>(rbc.Type);
		definition.fixedRotation = false;
		definition.allowSleep = rbc.AllowSleep;
		definition.gravityScale = rbc.GravityScale;
		definition.position.Set(tc.Position.x, tc.Position.y);
		definition.angle = tc.Rotation.z;
		//definition.linearDamping = 1.f;
		//definition.angularDamping = 10.f;

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

		if (entity.HasComponent<PolygonCollider2DComponent>()) {
			CreatePolygonCollider2D(entity, tc, rbc, entity.GetComponent<PolygonCollider2DComponent>());
		}
	}

	void Scene::CreateBoxCollider2D(Entity entity, const TransformComponent& tc, const RigidBody2DComponent& rbc, BoxCollider2DComponent& bcc)
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

	void Scene::CreateCircleCollider2D(Entity entity, const TransformComponent& tc, const RigidBody2DComponent& rbc, CircleCollider2DComponent& ccc)
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

	void Scene::CreatePolygonCollider2D(Entity entity, const TransformComponent& tc, const RigidBody2DComponent& rbc, PolygonCollider2DComponent& pcc)
	{
		if (pcc.Vertices.size() < 3) {
			SW_ERROR("Cannot create polygon collider with less than 3 vertices! Currently: {}", pcc.Vertices.size());
			return;
		}

		for (u64 i = 0; i < pcc.Vertices.size(); ++i) {
			pcc.Vertices[i] += pcc.Offset;
		}

		b2PolygonShape polygonShape;
		polygonShape.Set((const b2Vec2*)pcc.Vertices.data(), (i32)pcc.Vertices.size());

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		fixtureDef.userData.pointer = static_cast<u32>(entity);
		fixtureDef.density = pcc.Density;
		fixtureDef.friction = rbc.Friction;
		fixtureDef.restitution = rbc.Restitution;
		fixtureDef.restitutionThreshold = rbc.RestitutionThreshold;
		fixtureDef.isSensor = pcc.IsSensor;

		b2Body* body = static_cast<b2Body*>(rbc.Handle);
		b2Fixture* fixture = body->CreateFixture(&fixtureDef);

		pcc.Handle = fixture;
	}

	void Scene::CreateDistanceJoint2D(Entity entity, const RigidBody2DComponent& rbc, DistanceJoint2DComponent& djc)
	{
		if (!djc.ConnectedEntityID)
			return;

		Entity connectedEntity = GetEntityByID(djc.ConnectedEntityID);

		if (!connectedEntity.HasComponent<RigidBody2DComponent>())
			return;

		b2Body* originBody = (b2Body*)rbc.Handle;
		b2Body* connectedBody = (b2Body*)connectedEntity.GetComponent<RigidBody2DComponent>().Handle;

		b2Vec2 originAnchor = originBody->GetWorldPoint({ djc.OriginAnchor.x, djc.OriginAnchor.y });
		b2Vec2 connectedAnchor = connectedBody->GetWorldPoint({ djc.ConnectedAnchor.x, djc.ConnectedAnchor.y });

		b2DistanceJointDef jointDef;
		jointDef.Initialize(originBody, connectedBody, originAnchor, connectedAnchor);
		jointDef.collideConnected = djc.EnableCollision;
		if (!djc.AutoLength)
			jointDef.length = djc.Length;
		jointDef.minLength = glm::min(jointDef.length, djc.MinLength);
		jointDef.maxLength = jointDef.length + glm::max(djc.MaxLength, 0.0f);

		djc.RuntimeJoint = m_PhysicsWorld2D->CreateJoint(&jointDef);
	}

	void Scene::CreateRevolutionJoint2D(Entity entity, const RigidBody2DComponent& rbc, RevolutionJoint2DComponent& rjc)
	{
		if (!rjc.ConnectedEntityID)
			return;

		Entity connectedEntity = GetEntityByID(rjc.ConnectedEntityID);

		if (!connectedEntity.HasComponent<RigidBody2DComponent>())
			return;

		b2Body* originBody = (b2Body*)rbc.Handle;
		b2Body* connectedBody = (b2Body*)connectedEntity.GetComponent<RigidBody2DComponent>().Handle;

		b2Vec2 originAnchor = originBody->GetWorldPoint({ rjc.OriginAnchor.x, rjc.OriginAnchor.y });

		b2RevoluteJointDef jointDef;
		jointDef.Initialize(originBody, connectedBody, originAnchor);
		jointDef.collideConnected = rjc.EnableCollision;
		jointDef.enableLimit = rjc.EnableLimit;
		jointDef.lowerAngle = rjc.LowerAngle;
		jointDef.upperAngle = rjc.UpperAngle;
		jointDef.enableMotor = rjc.EnableMotor;
		jointDef.motorSpeed = rjc.MotorSpeed;
		jointDef.maxMotorTorque = rjc.MaxMotorTorque;

		rjc.RuntimeJoint = m_PhysicsWorld2D->CreateJoint(&jointDef);
	}

	void Scene::CreatePrismaticJoint2D(Entity entity, const RigidBody2DComponent& rbc, PrismaticJoint2DComponent& pjc)
	{
		if (!pjc.ConnectedEntityID)
			return;

		Entity connectedEntity = GetEntityByID(pjc.ConnectedEntityID);

		if (!connectedEntity.HasComponent<RigidBody2DComponent>())
			return;

		b2Body* originBody = (b2Body*)rbc.Handle;
		b2Body* connectedBody = (b2Body*)connectedEntity.GetComponent<RigidBody2DComponent>().Handle;

		b2Vec2 worldAxis(1.0f, 0.0f);

		b2PrismaticJointDef jointDef;
		jointDef.Initialize(originBody, connectedBody, originBody->GetWorldPoint({pjc.OriginAnchor.x, pjc.OriginAnchor.y}), worldAxis);
		jointDef.collideConnected = pjc.EnableCollision;
		jointDef.referenceAngle = pjc.Angle;
		jointDef.enableLimit = pjc.EnableLimit;
		jointDef.lowerTranslation = pjc.LowerTranslation;
		jointDef.upperTranslation = pjc.UpperTranslation;
		jointDef.enableMotor = pjc.EnableMotor;
		jointDef.motorSpeed = pjc.MotorSpeed;
		jointDef.maxMotorForce = pjc.MaxMotorForce;

		pjc.RuntimeJoint = m_PhysicsWorld2D->CreateJoint(&jointDef);
	}

	void Scene::CreateSpringJoint2D(Entity entity, const RigidBody2DComponent& rbc, SpringJoint2DComponent& sjc)
	{
		if (!sjc.ConnectedEntityID)
			return;

		Entity connectedEntity = GetEntityByID(sjc.ConnectedEntityID);

		if (!connectedEntity.HasComponent<RigidBody2DComponent>())
			return;

		b2Body* originBody = (b2Body*)rbc.Handle;
		b2Body* connectedBody = (b2Body*)connectedEntity.GetComponent<RigidBody2DComponent>().Handle;

		b2Vec2 originAnchor = originBody->GetWorldPoint({ sjc.OriginAnchor.x, sjc.OriginAnchor.y });
		b2Vec2 connectedAnchor = connectedBody->GetWorldPoint({ sjc.ConnectedAnchor.x, sjc.ConnectedAnchor.y });

		b2DistanceJointDef jointDef;
		jointDef.Initialize(originBody, connectedBody, originAnchor, connectedAnchor);
		jointDef.collideConnected = sjc.EnableCollision;
		if (!sjc.AutoLength)
			jointDef.length = sjc.Length;
		jointDef.minLength = glm::min(jointDef.length, sjc.MinLength);
		jointDef.maxLength = jointDef.length + glm::max(sjc.MaxLength, 0.0f);

		b2LinearStiffness(jointDef.stiffness, jointDef.damping, sjc.Frequency, sjc.DampingRatio, originBody, connectedBody);

		sjc.RuntimeJoint = m_PhysicsWorld2D->CreateJoint(&jointDef);
	}

	void Scene::CreateWheelJoint2D(Entity entity, const RigidBody2DComponent& rbc, WheelJoint2DComponent& wjc)
	{
		if (!wjc.ConnectedEntityID)
			return;

		Entity connectedEntity = GetEntityByID(wjc.ConnectedEntityID);

		if (!connectedEntity.HasComponent<RigidBody2DComponent>())
			return;

		b2Body* originBody = (b2Body*)rbc.Handle;
		b2Body* connectedBody = (b2Body*)connectedEntity.GetComponent<RigidBody2DComponent>().Handle;

		b2Vec2 axis(0.0f, 1.0f);

		f32 mass = originBody->GetMass();
		f32 omega = 2.0f * b2_pi * wjc.Frequency;

		b2WheelJointDef jointDef;
		jointDef.Initialize(originBody, connectedBody, originBody->GetWorldPoint({ wjc.OriginAnchor.x, wjc.OriginAnchor.y }), axis);
		jointDef.collideConnected = wjc.EnableCollision;
		jointDef.stiffness = mass * omega * omega;
		jointDef.damping = 2.0f * mass * wjc.DampingRatio * omega;
		jointDef.enableMotor = wjc.EnableMotor;
		jointDef.motorSpeed = wjc.MotorSpeed;
		jointDef.maxMotorTorque = wjc.MaxMotorTorque;
		jointDef.enableLimit = wjc.EnableLimit;
		jointDef.lowerTranslation = wjc.LowerTranslation;
		jointDef.upperTranslation = wjc.UpperTranslation;

		wjc.RuntimeJoint = m_PhysicsWorld2D->CreateJoint(&jointDef);
	}

}
