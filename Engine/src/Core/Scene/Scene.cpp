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
#include "Core/Scripting/ScriptingCore.hpp"
#include "Asset/Prefab.hpp"
#include "Asset/AssetManager.hpp"
#include "Audio/AudioEngine.hpp"
#include "Audio/SoundListener.hpp"

namespace SW {

// WELP
#define CopyReferencedEntities(T) \
	{ \
		if (src.HasComponent<T>()) { \
			T& component = src.GetComponent<T>(); \
			if (component.ConnectedEntityID) { \
				Entity connectedEntity = GetEntityByID(component.ConnectedEntityID); \
				Entity duplicatedConnectedEntity = DuplicateEntity(connectedEntity, duplicatedEntities); \
				T& newComponent = dst.GetComponent<T>(); \
				newComponent.ConnectedEntityID = duplicatedConnectedEntity.GetID(); \
			} \
		} \
	}

	Scene::Scene()
		: m_Registry(this)
	{
		//entt::registry& reg = m_Registry.GetRegistryHandle();

		// This enables creating entities in runtime
		//reg.on_construct<RigidBody2DComponent>().connect<&Scene::OnRigidBody2DComponentCreated>(this);
	}
	
	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tag /*= "Entity"*/)
	{
		return CreateEntityWithID(Random::CreateID(), tag);
	}

	Entity Scene::CreateEntityWithID(u64 id, const std::string& tag /*= "Entity"*/)
	{
		Entity entity = { m_Registry.GetRegistryHandle().create(), this };

		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<RelationshipComponent>();

		m_EntityMap[id] = entity;

		return entity;
	}

	template <typename T>
	static void RemoveReferencedConnections(entt::registry& registry, u64 id)
	{
		for (entt::entity handle : registry.view<T>()) {
			T& component = registry.get<T>(handle);

			if (component.ConnectedEntityID == id)
				component.ConnectedEntityID = 0;
		}
	}

	void Scene::DestroyEntity(Entity entity)
	{
		entity.RemoveParent();

		RelationshipComponent& rc = entity.GetRelations();

		const std::vector<u64> children = rc.ChildrenIDs;
		
		for (u64 childId : children) {
			Entity childToDelete = GetEntityByID(childId);

			DestroyEntity(childToDelete);
		}

		u64 id = entity.GetID();

		entt::registry& registry = m_Registry.GetRegistryHandle();

		RemoveReferencedConnections<DistanceJoint2DComponent>(registry, id);
		RemoveReferencedConnections<RevolutionJoint2DComponent>(registry, id);
		RemoveReferencedConnections<PrismaticJoint2DComponent>(registry, id);
		RemoveReferencedConnections<SpringJoint2DComponent>(registry, id);
		RemoveReferencedConnections<WheelJoint2DComponent>(registry, id);

		if (IsPlaying() &&entity.HasComponent<RigidBody2DComponent>()) {
			b2Body* body = (b2Body*)entity.GetComponent<RigidBody2DComponent>().Handle;

			m_PhysicsWorld2D->DestroyBody(body);
		}

		if (entity.HasComponent<ScriptComponent>()) {
			ScriptComponent& sc = entity.GetComponent<ScriptComponent>();

			if (sc.ScriptID) {
				if (sc.Instance.IsValid())
					sc.Instance.Invoke("OnDestroy");

				m_ScriptStorage.ShutdownEntityStorage(sc.ScriptID, id);
			}
		}

		m_EntityMap.erase(id);
		m_Registry.DestroyEntity(entity);

		SortEntities();
	}

	void Scene::DestroyEntityInRuntime(u64 id)
	{
		m_EntitiesToDelete.emplace(id);
	}

	void Scene::DestroyAllEntities()
    {
		m_Registry.DestroyAllEntities();

		m_EntityMap.clear();
    }

	void Scene::SortEntities()
	{
		m_Registry.GetRegistryHandle().sort<IDComponent>([&](const auto lhs, const auto rhs) {
			auto lhsEntity = m_EntityMap.find(lhs.ID);
			auto rhsEntity = m_EntityMap.find(rhs.ID);

			return static_cast<u32>(lhsEntity->second) < static_cast<u32>(rhsEntity->second);
		});
	}

	Entity Scene::InstantiatePrefab(
		const Prefab* prefab, const glm::vec3* position /*= nullptr*/, const glm::vec3* rotation /*= nullptr*/,
		const glm::vec3* scale /*= nullptr*/
	) {
		std::unordered_map<u64, Entity> duplicatedEntities;

		return CreatePrefabricatedEntity(prefab->GetRootEntity(), duplicatedEntities, position, rotation, scale);
	}

#define CopyReferencedEntitiesPref(T) \
{ \
	if (src.HasComponent<T>()) { \
		T& component = src.GetComponent<T>(); \
		T& newComponent = dst.GetComponent<T>(); \
		if (component.ConnectedEntityID) { \
			if (duplicatedEntities.contains(component.ConnectedEntityID)) { \
				newComponent.ConnectedEntityID = duplicatedEntities.at(component.ConnectedEntityID).GetID(); \
			} else { \
				Entity connectedEntity = srcScene->GetEntityByID(component.ConnectedEntityID); \
				Entity duplicatedConnectedEntity = CreatePrefabricatedEntity(connectedEntity, duplicatedEntities); \
				newComponent.ConnectedEntityID = duplicatedConnectedEntity.GetID(); \
				duplicatedEntities[component.ConnectedEntityID] = duplicatedConnectedEntity; \
			} \
		} \
	} \
}

	Entity Scene::CreatePrefabricatedEntity(
		Entity src, std::unordered_map<u64, Entity>& duplicatedEntities, const glm::vec3* position /*= nullptr*/,
		const glm::vec3* rotation /*= nullptr*/, const glm::vec3* scale /*= nullptr*/
	) {
		if (duplicatedEntities.count(src.GetID()) > 0) {
			return duplicatedEntities[src.GetID()];
		}

		Scene* srcScene = src.GetScene();

		entt::registry& destReg = GetRegistry().GetRegistryHandle();

		Entity dst = CreateEntity();

		srcScene->CopyComponentIfExists<TagComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<TransformComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<SpriteComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<AnimatedSpriteComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<CircleComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<TextComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<ScriptComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<CameraComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<RigidBody2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<BoxCollider2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<CircleCollider2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<PolygonCollider2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<BuoyancyEffector2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<DistanceJoint2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<RevolutionJoint2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<PrismaticJoint2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<SpringJoint2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<WheelJoint2DComponent>(dst, destReg, src);

		CopyReferencedEntitiesPref(DistanceJoint2DComponent);
		CopyReferencedEntitiesPref(RevolutionJoint2DComponent);
		CopyReferencedEntitiesPref(PrismaticJoint2DComponent);
		CopyReferencedEntitiesPref(SpringJoint2DComponent);
		CopyReferencedEntitiesPref(WheelJoint2DComponent);

		if (position)
			dst.GetTransform().Position = *position;

		if (rotation)
			dst.GetTransform().Rotation = *rotation;
		
		if (scale)
			dst.GetTransform().Scale = *scale;

		if (IsPlaying()) {
			if (src.HasComponent<RigidBody2DComponent>()) {
				RigidBody2DComponent& rbc = dst.GetComponent<RigidBody2DComponent>();

				CreateRigidbody2D(dst, dst.GetWorldSpaceTransform(), rbc);
			}
		}

		if (src.HasComponent<ScriptComponent>()) {
			ScriptComponent& sc = dst.GetComponent<ScriptComponent>();

			ASSERT(ScriptingCore::Get().IsValidScript(sc.ScriptID), "Prefab's script ID is invalid!");

			m_ScriptStorage.InitializeEntityStorage(sc.ScriptID, dst.GetID());
			srcScene->GetScriptStorage().CopyEntityStorage(src.GetID(), dst.GetID(), m_ScriptStorage);
			ScriptingCore& scriptEngine = ScriptingCore::Get();

			if (IsPlaying() && scriptEngine.IsValidScript(sc.ScriptID)) {
				sc.Instance = scriptEngine.Instantiate(dst.GetID(), m_ScriptStorage, u64(dst.GetID()));
				sc.Instance.Invoke("OnCreate");
			}
		}

		duplicatedEntities[src.GetID()] = dst;

		std::vector<u64> childIds = src.GetRelations().ChildrenIDs;

		for (u64 childId : childIds) {
			Entity childDuplicate = CreatePrefabricatedEntity(srcScene->GetEntityByID(childId), duplicatedEntities);

			childDuplicate.SetParent(dst);
		}

		return dst;
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsFrameAccumulator = 0.0f;

		m_PhysicsWorld2D = new b2World({ Gravity.x, Gravity.y });
		m_PhysicsContactListener2D = new Physics2DContactListener(this);
		m_PhysicsWorld2D->SetContactListener(m_PhysicsContactListener2D);

		for (auto&& [handle, id, sc] : m_Registry.GetEntitiesWith<IDComponent, ScriptComponent>().each()) {
			Entity entity = { handle, this };

			if (!sc.ScriptID)
				continue;

			sc.Instance = ScriptingCore::Get().Instantiate<u64>(id.ID, m_ScriptStorage, (u64)id.ID);
			sc.Instance.Invoke("OnCreate");
		}

		for (auto&& [handle, rbc] : m_Registry.GetEntitiesWith<RigidBody2DComponent>().each()) {
			Entity entity = { handle, this };

			CreateRigidbody2D(entity, entity.GetWorldSpaceTransform(), rbc);
		}

		for (auto&& [handle, rbc, djc] : m_Registry.GetEntitiesWith<RigidBody2DComponent, DistanceJoint2DComponent>().each()) {
			CreateDistanceJoint2D(rbc, djc);
		}

		for (auto&& [handle, rbc, rjc] : m_Registry.GetEntitiesWith<RigidBody2DComponent, RevolutionJoint2DComponent>().each()) {
			CreateRevolutionJoint2D(rbc, rjc);
		}

		for (auto&& [handle, rbc, pjc] : m_Registry.GetEntitiesWith<RigidBody2DComponent, PrismaticJoint2DComponent>().each()) {
			CreatePrismaticJoint2D(rbc, pjc);
		}

		for (auto&& [handle, rbc, sjc] : m_Registry.GetEntitiesWith<RigidBody2DComponent, SpringJoint2DComponent>().each()) {
			CreateSpringJoint2D(rbc, sjc);
		}

		for (auto&& [handle, rbc, wjc] : m_Registry.GetEntitiesWith<RigidBody2DComponent, WheelJoint2DComponent>().each()) {
			CreateWheelJoint2D(rbc, wjc);
		}

		for (auto&& [handle, tc, asc] : m_Registry.GetEntitiesWith<TransformComponent, AudioSourceComponent>().each()) {
			if (!asc.Handle)
				continue;

			if (asc.PlayOnCreate) {
				Sound* sound = *AssetManager::GetAssetRaw<Sound>(asc.Handle);

				SoundSpecification spec;
				spec.Sound = sound;
				spec.Pitch = asc.Pitch;
				spec.Volume = asc.Volume;

				if (asc.Is3D) {
					spec.Looping = asc.Looping;
					spec.Is3D = asc.Is3D;
					spec.Attenuation = asc.Attenuation;
					spec.RollOff = asc.RollOff;
					spec.MinGain = asc.MinGain;
					spec.MaxGain = asc.MaxGain;
					spec.MinDistance = asc.MinDistance;
					spec.MaxDistance = asc.MaxDistance;
					spec.ConeInnerAngle = asc.ConeInnerAngle;
					spec.ConeOuterAngle = asc.ConeOuterAngle;
					spec.ConeOuterGain = asc.ConeOuterGain;
					spec.DopplerFactor = asc.DopplerFactor;

					const glm::mat4 invertedTransform = glm::inverse(Entity(handle, this).GetWorldSpaceTransformMatrix());
					const glm::vec3 forward = glm::normalize(glm::vec3(invertedTransform * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));

					asc.Instance = AudioEngine::PlaySound3D(spec, tc.Position, forward);
				} else {
					asc.Instance = AudioEngine::PlaySound(spec);
				}
			}
		}

		for (auto&& [handle, tc, alc] : m_Registry.GetEntitiesWith<TransformComponent, AudioListenerComponent>().each()) {
			SoundListener* listener = new SoundListener(alc.ConeInnerAngle, alc.ConeOuterAngle, alc.ConeOuterGain);

			const glm::mat4 invertedTransform = glm::inverse(Entity(handle, this).GetWorldSpaceTransformMatrix());
			const glm::vec3 forward = glm::normalize(glm::vec3(invertedTransform * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));

			listener->SetPosition(tc.Position);
			listener->SetDirection(-forward);

			alc.Listener = listener;
		}

		for (auto&& [handle, tc, alc] : m_Registry.GetEntitiesWith<TransformComponent, AudioListenerComponent>().each()) {
			if (alc.Listener) {
				delete alc.Listener;
				alc.Listener = nullptr;
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		for (auto&& [handle, id, sc] : m_Registry.GetEntitiesWith<IDComponent, ScriptComponent>().each()) {
			Entity entity = { handle, this };

			if (!sc.ScriptID)
				continue;

			sc.Instance.Invoke("OnCleanup");
			ScriptingCore::Get().DestroyInstance(id.ID, m_ScriptStorage);
		}

		for (auto&& [handle, tc, asc] : m_Registry.GetEntitiesWith<TransformComponent, AudioSourceComponent>().each()) {
			if (!asc.Handle)
				continue;

			if (asc.Instance && *asc.Instance)
				(*asc.Instance)->Stop();
		}

		delete m_PhysicsWorld2D;
		m_PhysicsWorld2D = nullptr;

		delete m_PhysicsContactListener2D;
		m_PhysicsContactListener2D = nullptr;
	}

    void Scene::OnUpdateEditor(Timestep dt, EditorCamera* camera)
    {
		PROFILE_FUNCTION();

		m_AnimationTime += dt;

		Renderer2D::BeginScene(camera);

		for (auto&& [handle, sc] : m_Registry.GetEntitiesWith<SpriteComponent>().each()) {
			Entity entity = { handle, this };

			Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), sc, (int)handle);
		}

		for (auto&& [handle, asc] : m_Registry.GetEntitiesWith<AnimatedSpriteComponent>().each()) {
			Entity entity = { handle, this };

			if (!asc.CurrentAnimation)
				continue;

			Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), asc, m_AnimationTime, (int)handle);
		}

		for (auto&& [handle, cc] : m_Registry.GetEntitiesWith<CircleComponent>().each()) {
			Entity entity = { handle, this };

			Renderer2D::DrawCircle(entity.GetWorldSpaceTransformMatrix(), cc, (int)handle);
		}

		for (auto&& [handle, tc] : m_Registry.GetEntitiesWith<TextComponent>().each()) {
			Entity entity = { handle, this };

			if (!tc.Handle)
				continue;

			Renderer2D::DrawString(entity.GetWorldSpaceTransformMatrix(), tc, (int)handle);
		}
	}

	void Scene::OnUpdateRuntime(Timestep dt)
	{
		PROFILE_FUNCTION();

		m_AnimationTime += dt;

#pragma region Physics
		if (m_SceneState != SceneState::Pause) {
			PROFILE_SCOPE("Scene::OnUpdateRuntime() - Physics update");

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

				Entity entity = { handle, this };

				tc.Position.x = position.x;
				tc.Position.y = position.y;
				tc.Rotation.z = body->GetAngle();

				entity.ConvertToLocalSpace();
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
		}

		while (!m_EntitiesToDelete.empty()) {
			u64 id = m_EntitiesToDelete.front();
			Entity toDelete = TryGetEntityByID(id);

			if (toDelete) {
				DestroyEntity(toDelete);
			} else {
				SW_WARN("Entity could not be found {}", id);
			}

			m_EntitiesToDelete.pop();
		}

#pragma endregion

		{
			PROFILE_SCOPE("Scene::OnUpdate - C# OnUpdate");

			for (auto&& [handle, sc] : m_Registry.GetEntitiesWith<ScriptComponent>().each()) {
				sc.Instance.Invoke<f32>("OnUpdate", dt);
			}
		}

		{
			PROFILE_SCOPE("Scene::OnUpdate - C# OnLateUpdate");

			for (auto&& [handle, sc] : m_Registry.GetEntitiesWith<ScriptComponent>().each()) {
				sc.Instance.Invoke<f32>("OnLateUpdate", dt);
			}
		}

		glm::mat4 cameraTransform;
		SceneCamera* mainCamera = nullptr;

		for (auto&& [handle, cc] : m_Registry.GetEntitiesWith<CameraComponent>().each()) {
			Entity entity = { handle, this };

			if (cc.Primary) {
				mainCamera = &cc.Camera;

				glm::mat4 worldTransform = entity.GetWorldSpaceTransformMatrix();

				glm::vec3 position;
				glm::vec3 rotation;

				Math::DecomposeTransformForTranslationAndRotation(worldTransform, position, rotation);

				cameraTransform = glm::translate(glm::mat4(1.0f), position) // do not apply scale to the camera
					* glm::toMat4(glm::quat(rotation));

				break;
			}
		}

		if (!mainCamera) {
			Renderer2D::StartBatch(); // fix to a bug where camera is not present, but leftovers from edit state were present.
			return;
		}

		Renderer2D::BeginScene(*mainCamera, cameraTransform);

		for (auto&& [handle, sc] : m_Registry.GetEntitiesWith<SpriteComponent>().each()) {
			Entity entity = { handle, this };

			Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), sc, (int)handle);
		}

		for (auto&& [handle, asc] : m_Registry.GetEntitiesWith<AnimatedSpriteComponent>().each()) {
			Entity entity = { handle, this };

			if (!asc.CurrentAnimation)
				continue;

			Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), asc, m_AnimationTime, (int)handle);
		}

		for (auto&& [handle, cc] : m_Registry.GetEntitiesWith<CircleComponent>().each()) {
			Entity entity = { handle, this };

			Renderer2D::DrawCircle(entity.GetWorldSpaceTransformMatrix(), cc, (int)handle);
		}

		for (auto&& [handle, tc] : m_Registry.GetEntitiesWith<TextComponent>().each()) {
			Entity entity = { handle, this };

			if (!tc.Handle)
				continue;

			Renderer2D::DrawString(entity.GetWorldSpaceTransformMatrix(), tc, (int)handle);
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
			return m_EntityMap.at(id);

		ASSERT(false, "Entity with ID: {} does not exist!", id);

		return {};
	}

	Entity Scene::TryGetEntityByID(u64 id)
	{
		if (const auto iter = m_EntityMap.find(id); iter != m_EntityMap.end())
			return iter->second;

		return {};
	}

    Entity Scene::GetEntityByTag(const std::string& tag)
    {
		for (auto&& [handle, tc] : m_Registry.GetEntitiesWith<TagComponent>().each()) {
			if (tc.Tag == tag) {
				return { handle, this };
			}
		}

		ASSERT(false, "Entity with tag: {} does not exist!", tag);

		return {};
    }

	Entity Scene::TryGetEntityByTag(const std::string& tag)
	{
		for (auto&& [handle, tc] : m_Registry.GetEntitiesWith<TagComponent>().each()) {
			if (tc.Tag == tag) {
				return { handle, this };
			}
		}

		return {};
	}

	template<typename T>
	inline static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<u64, entt::entity>& enttMap)
	{
		for (entt::entity srcEntity : srcRegistry.view<T>()) {
			entt::entity destEntity = enttMap.at(srcRegistry.get<IDComponent>(srcEntity).ID);

			T& srcComponent = srcRegistry.get<T>(srcEntity);
			dstRegistry.emplace_or_replace<T>(destEntity, srcComponent);
		}
	}

    Scene* Scene::DeepCopy()
    {
		Scene* copy = new Scene();
		copy->SetHandle(this->GetHandle());

		std::unordered_map<u64, entt::entity> enttMap;

		entt::registry& currentRegistry = m_Registry.GetRegistryHandle();
		entt::registry& copyRegistry = copy->GetRegistry().GetRegistryHandle();

		for (auto entity : currentRegistry.view<IDComponent>()) {
			u64 uuid = currentRegistry.get<IDComponent>(entity).ID;
			std::string& name = currentRegistry.get<TagComponent>(entity).Tag;

			enttMap[uuid] = copy->CreateEntityWithID(uuid, name);
		}

		CopyComponent<TransformComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<SpriteComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<AnimatedSpriteComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<CircleComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<RelationshipComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<ScriptComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<TextComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<CameraComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<RigidBody2DComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<CircleCollider2DComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<PolygonCollider2DComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<BuoyancyEffector2DComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<DistanceJoint2DComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<RevolutionJoint2DComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<PrismaticJoint2DComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<SpringJoint2DComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<WheelJoint2DComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<AudioSourceComponent>(copyRegistry, currentRegistry, enttMap);
		CopyComponent<AudioListenerComponent>(copyRegistry, currentRegistry, enttMap);

		m_ScriptStorage.CopyTo(copy->m_ScriptStorage);

		return copy;
    }

	Entity Scene::DuplicateEntity(Entity src, std::unordered_map<u64, Entity>& duplicatedEntities)
	{
		if (duplicatedEntities.count(src.GetID()) > 0) {
			return duplicatedEntities[src.GetID()];
		}

		entt::registry& currentRegistry = m_Registry.GetRegistryHandle();

		Entity dst = CreateEntity();

		CopyComponentIfExists<TagComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<TransformComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<SpriteComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<AnimatedSpriteComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<CircleComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<TextComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<ScriptComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<CameraComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<RigidBody2DComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<BoxCollider2DComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<CircleCollider2DComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<PolygonCollider2DComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<BuoyancyEffector2DComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<DistanceJoint2DComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<RevolutionJoint2DComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<PrismaticJoint2DComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<SpringJoint2DComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<WheelJoint2DComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<AudioSourceComponent>(dst, currentRegistry, src);
		CopyComponentIfExists<AudioListenerComponent>(dst, currentRegistry, src);

		CopyReferencedEntities(DistanceJoint2DComponent);
		CopyReferencedEntities(RevolutionJoint2DComponent);
		CopyReferencedEntities(PrismaticJoint2DComponent);
		CopyReferencedEntities(SpringJoint2DComponent);
		CopyReferencedEntities(WheelJoint2DComponent);

		if (dst.HasComponent<ScriptComponent>()) {
			const ScriptComponent& sc = dst.GetComponent<ScriptComponent>();

			m_ScriptStorage.InitializeEntityStorage(sc.ScriptID, dst.GetID());
			m_ScriptStorage.CopyEntityStorage(src.GetID(), dst.GetID(), m_ScriptStorage);
		}

		duplicatedEntities[src.GetID()] = dst;

		std::vector<u64> childIds = src.GetRelations().ChildrenIDs;

		if (Entity parent = src.GetParent()) {
			dst.SetParent(parent);
		}

		for (u64 childId : childIds) {
			Entity childDuplicate = DuplicateEntity(GetEntityByID(childId), duplicatedEntities);
			
			childDuplicate.SetParent(dst);
		}

		return dst;
	}

	void Scene::CreateRigidbody2D(Entity entity, const TransformComponent& tc, RigidBody2DComponent& rbc)
	{
		b2BodyDef definition;
		definition.type = static_cast<b2BodyType>(rbc.Type);
		definition.allowSleep = rbc.AllowSleep;
		definition.awake = rbc.InitiallyAwake;
		definition.fixedRotation = rbc.FixedRotation;
		definition.bullet = rbc.IsBullet;
		definition.gravityScale = rbc.GravityScale;
		definition.position.Set(tc.Position.x, tc.Position.y);
		definition.angle = tc.Rotation.z;
		definition.linearDamping = rbc.LinearDamping;
		definition.angularDamping = rbc.AngularDamping;

		b2Body* rb = m_PhysicsWorld2D->CreateBody(&definition);

		if (!rbc.AutoMass && rbc.Mass > 0.01f) {
			b2MassData massData = rb->GetMassData();
			massData.mass = rbc.Mass;
			rb->SetMassData(&massData);
		}

		rbc.Handle = rb;

		if (entity.HasComponent<BoxCollider2DComponent>()) {
			CreateBoxCollider2D(entity, tc, rbc, entity.GetComponent<BoxCollider2DComponent>());
		}

		if (entity.HasComponent<CircleCollider2DComponent>()) {
			CreateCircleCollider2D(entity, tc, rbc, entity.GetComponent<CircleCollider2DComponent>());
		}

		if (entity.HasComponent<PolygonCollider2DComponent>()) {
			CreatePolygonCollider2D(entity, rbc, entity.GetComponent<PolygonCollider2DComponent>());
		}
	}

	void Scene::CreateBoxCollider2D(Entity entity, const TransformComponent& tc, const RigidBody2DComponent& rbc, BoxCollider2DComponent& bcc)
	{
		b2PolygonShape boxShape;
		boxShape.SetAsBox(tc.Scale.x * bcc.Size.x, tc.Scale.y * bcc.Size.y, b2Vec2(tc.Scale.x * bcc.Offset.x, tc.Scale.y * bcc.Offset.y), 0.0f);

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
		circleShape.m_p.Set(ccc.Offset.x, ccc.Offset.y);

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

	void Scene::CreatePolygonCollider2D(Entity entity, const RigidBody2DComponent& rbc, PolygonCollider2DComponent& pcc)
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

	void Scene::CreateDistanceJoint2D(const RigidBody2DComponent& rbc, DistanceJoint2DComponent& djc)
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

	void Scene::CreateRevolutionJoint2D(const RigidBody2DComponent& rbc, RevolutionJoint2DComponent& rjc)
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

	void Scene::CreatePrismaticJoint2D(const RigidBody2DComponent& rbc, PrismaticJoint2DComponent& pjc)
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

	void Scene::CreateSpringJoint2D(const RigidBody2DComponent& rbc, SpringJoint2DComponent& sjc)
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

	void Scene::CreateWheelJoint2D(const RigidBody2DComponent& rbc, WheelJoint2DComponent& wjc)
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

	//void Scene::OnRigidBody2DComponentCreated(entt::registry& registry, entt::entity handle)
	//{
	//	if (!IsPlaying())
	//		return;

	//	Entity entity = { handle, this };

	//	RigidBody2DComponent& rbc = entity.GetComponent<RigidBody2DComponent>();

	//	CreateRigidbody2D(entity, entity.GetWorldSpaceTransform(), rbc);
	//}

}
