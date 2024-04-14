/**
 * @file Scene.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.2.0
 * @date 2024-04-13
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Scene/SceneCamera.hpp"
#include "Core/Timestep.hpp"
#include "Core/Math/Vector2.hpp"
#include "Core/ECS/EntityRegistry.hpp"
#include "Core/ECS/Components.hpp"
#include "Core/Scripting/ScriptStorage.hpp"
#include "Asset/Asset.hpp"

class b2World;

namespace SW {

	class Entity;
	class EditorCamera;
	class Physics2DContactListener;
	class Prefab;

	/**
	 * @brief Represents the state of the scene.
	 */
	enum class SceneState {
		Edit = 0,		//< The scene is in edit mode. Entities can be added, removed and modified.
		Play = 1,		//< The scene is in play mode. The scene is being simulated.
		Pause = 2,		//< The scene is in pause mode. The scene is not being simulated.
		Simulate = 3	//< The scene is in simulate mode. The scene is being simulated, but the editor is still usable.
	};

	/**
	 * @brief Represents a scene in the game engine.
	 */
	class Scene final : public Asset
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		Scene();

		/**
		 * @brief Destructor.
		 */
		~Scene();

		static AssetType GetStaticType() { return AssetType::Scene; }
		AssetType GetAssetType() const override { return AssetType::Scene; }

		/**
		 * @brief Creates a new entity in the scene with an optional tag.
		 * @param tag The tag to assign to the entity (default is "Entity").
		 * @return The created entity.
		 */
		Entity CreateEntity(const std::string& tag = "Entity");

		/**
		 * @brief Creates a new entity in the scene with id and an optional tag.
		 * @param id The id to assign to the entity.
		 * @param tag The tag to assign to the entity (default is "Entity").
		 * @return The created entity.
		 */
		Entity CreateEntityWithID(u64 id, const std::string& tag = "Entity");

		/**
		 * @brief Destroys the specified entity in the scene.
		 * @param entity The entity to destroy.
		 */
		void DestroyEntity(Entity entity);

		/**
		 * @brief Destroys all present entities in the scene.
		 */
		void DestroyAllEntities();

		/**
		 * @brief This function is called when the scene is started (the state has changed)
		 */
		void OnRuntimeStart();

		/**
		 * @brief This function is called when the scene is stopped (the state has changed)
		 */
		void OnRuntimeStop();

		/**
		 * @brief Updates the scene in edit mode with the specified timestep and camera.
		 * @param dt The timestep since the last update.
		 * @param camera The camera to use for rendering the scene.
		 */
		void OnUpdateEditor(Timestep dt, EditorCamera* camera);

		/**
		 * @brief Updates the scene in runtime mode with the specified timestep.
		 * @param dt The timestep since the last update.
		 */
		void OnUpdateRuntime(Timestep dt);

		/**
		 * @brief Handles viewport resize events.
		 * @param width The new width of the viewport.
		 * @param height The new height of the viewport.
		 */
		void OnViewportResize(u32 width, u32 height);

		/**
		 * @brief Retrieves the entity registry of the scene.
		 * @return The entity registry.
		 */
		EntityRegistry& GetRegistry() { return m_Registry; }

		/**
		 * @brief Retrieves the entity with the specified ID.
		 * @param id The ID of the entity.
		 * @return The entity with the specified ID.
		 * 
		 * @warning Entity is expected to exist (runtime error if it doesn't)!
		 */
		Entity GetEntityByID(u64 id);

		/**
		 * @brief Retrieves the entity with the specified ID.
		 * @param id The ID of the entity.
		 * @return The entity with the specified ID, or an invalid entity if not found.
		 *
		 * @warning Entity is not expected to exist (empty returned if it doesn't) - caller must check!
		 */
		Entity TryGetEntityByID(u64 id);

		/**
		 * @brief Retrieves the entity with the specified tag.
		 * @param tag The tag of the entity.
		 * @return The entity with the specified tag.
		 *
		 * @warning Entity is expected to exist (runtime error if it doesn't)!
					If multiple entities have the same tag - the first found is returned.
		 */
		Entity GetEntityByTag(const std::string& tag);

		/**
		 * @brief Retrieves the entity with the specified tag.
		 * @param tag The tag of the entity.
		 * @return The entity with the specified tag.
		 *
		 * @warning Entity is not expected to exist (empty returned if it doesn't) - caller must check!
		 *			If multiple entities have the same tag - the first found is returned.
		 */
		Entity TryGetEntityByTag(const std::string& tag);

		/**
		 * @brief Retrieves current state of the scene.
		 * 
		 * @return SceneState The current state of the scene.
		 */
		SceneState GetCurrentState() const { return m_SceneState; }

		/**
		 * @brief Sets new state of the scene.
		 * @note Remember that the state will impact the way the scene is updated.
		 * 
		 * @param state The new state of the scene.
		 */
		void SetNewState(SceneState state) { m_SceneState = state; }

		/**
		 * @brief Set the position of the viewport.
		 * 
		 * @param position The new position of the viewport.
		 */
		void SetViewportPosition(glm::vec2 position) { m_ViewportPosition = position; }

		/**
		 * @brief Retrieves the position of the viewport.
		 * 
		 * @warning This is only the initial position of the viewport. Does not change during runtime.
		 * 
		 * @return glm::vec2 The position of the viewport.
		 */
		const glm::vec2& GetViewportPosition() const { return m_ViewportPosition; }

		/**
		 * @brief Copies the scene. (deep copy with all components and entities)
		 * 
		 * @return Scene* The copy of the scene.
		 */
		Scene* DeepCopy();

		/**
		 * @brief Duplicates the entity. (deep copy with all components and children)
		 * 
		 * @param entity The entity to duplicate.
		 * @param duplicatedEntities The map of already duplicated entities.
		 * 
		 * @return Entity The duplicated entity.
		 */
		Entity DuplicateEntity(Entity src, std::unordered_map<u64, Entity>& duplicatedEntities);

		/**
		 * @brief Get the width of the viewport.
		 * 
		 * @return u32 The width of the viewport.
		 */
		u32 GetViewportWidth() const { return m_ViewportWidth; }

		/**
		 * @brief Get the height of the viewport.
		 * 
		 * @return u32 The height of the viewport.
		 */
		u32 GetViewportHeight() const { return m_ViewportHeight; }

		/**
		 * @brief Sort entities by their IDs.
		 */
		void SortEntities();

		/**
		 * Copies a component from one entity to another if it exists.
		 * 
		 * @tparam T The type of the component to copy.
		 * @param dst The destination entity to copy the component to.
		 * @param dstRegistry The registry of the destination entity.
		 * @param src The source entity to copy the component from.
		 */
		template<typename T>
		inline void CopyComponentIfExists(entt::entity dst, entt::registry& dstRegistry, entt::entity src)
		{
			entt::registry& registry = m_Registry.GetRegistryHandle();

			if (registry.all_of<T>(src)) {
				T& srcComponent = registry.get<T>(src);

				dstRegistry.emplace_or_replace<T>(dst, srcComponent);
			}
		}

		/**
		 * Copies a component from one entity to another entity in a scene.
		 * 
		 * @tparam T The type of the component to copy.
		 * @param destination The destination scene where the component will be copied to.
		 * @param from The entity from which the component will be copied.
		 * @param to The entity to which the component will be copied.
		 */
		template<typename T>
		inline void CopyComponentIntoScene(Scene* destination, Entity from, Entity to)
		{
			entt::registry& destRegistry = destination->GetRegistry().GetRegistryHandle();	

			CopyComponentIfExists<T>(to, destRegistry, from);
		}

		/**
		 * @brief Whether the scene is currently playing. 
		 */
		bool IsPlaying() const { return m_SceneState != SceneState::Edit; }

		/**
		 * @brief Gets the script storage associated with the scene.
		 * 
		 * @return A reference to the script storage.
		 */
		ScriptStorage& GetScriptStorage() { return m_ScriptStorage; }

		const ScriptStorage& GetScriptStorageC() const { return m_ScriptStorage; }

		Entity InstantiatePrefab(const Prefab* prefab, const glm::vec3* position = nullptr, const glm::vec3* rotation = nullptr,
			const glm::vec3* scale = nullptr);

	private:
		Entity CreatePrefabricatedEntity(Entity src, std::unordered_map<u64, Entity>& duplicatedEntities, const glm::vec3* position = nullptr,
			const glm::vec3* rotation = nullptr, const glm::vec3* scale = nullptr);

	public:
		glm::vec2 Gravity = { 0.0f, -9.80665f };	/**< The gravity of the scene. */

	private:
		EntityRegistry m_Registry; /**< The entity registry of the scene. */

		std::unordered_map<u64, Entity> m_EntityMap = {}; /**< Map of entity IDs to entt::entity handles. (cache) */

		u32 m_ViewportWidth = 0; /**< The width of the viewport. */
		u32 m_ViewportHeight = 0; /**< The height of the viewport. */

		glm::vec2 m_ViewportPosition = glm::vec2(0.f); /**< The initial position of the viewport. */

		SceneState m_SceneState = SceneState::Edit;	/**< The current state of the scene. */

		b2World* m_PhysicsWorld2D = nullptr;		/**< The physics world of the scene. */
		Physics2DContactListener* m_PhysicsContactListener2D = nullptr;

		u32 m_VelocityIterations = 8;			/**< The number of velocity iterations for the physics simulation. */
		u32 m_PositionIterations = 3;			/**< The number of position iterations for the physics simulation. */
		f32 m_PhysicsFrameAccumulator = 0.0f;	/**< The frame accumulator for the physics simulation. */

		ScriptStorage m_ScriptStorage;			/**< The script storage of the scene. */

		f32 m_AnimationTime = 0.f;			/**< The time elapsed since the last frame. Used for proper 2D animation display. */

		/**
		 * @brief Register entity's rigidbody component in the physics world.
		 * 
		 * @param entity The entity which rigidbody will be registered.
		 * @param tc The transform component of the entity.
		 * @param rbc The rigidbody component to modify.
		 */
		void CreateRigidbody2D(Entity entity, const TransformComponent& tc, RigidBody2DComponent& rbc);

		/**
		 * @brief Register entity's box collider component in the physics world.
		 * 		  The entity will act like a box collider.
		 * 
		 * @param entity The entity which box collider will be registered.
		 * @param tc The transform component of the entity.
		 * @param rbc The rigidbody component of the entity.
		 * @param bcc The box collider component to modify.
		 */		
		void CreateBoxCollider2D(Entity entity, const TransformComponent& tc, const RigidBody2DComponent& rbc, BoxCollider2DComponent& bcc);

		/**
		 * @brief Creates a circle collider for a given entity.
		 *
		 * @param entity The entity for which to create the circle collider.
		 * @param tc The transform component of the entity.
		 * @param rbc The rigid body component of the entity.
		 * @param ccc The circle collider component to be created.
		 */
		void CreateCircleCollider2D(Entity entity, const TransformComponent& tc, const RigidBody2DComponent& rbc, CircleCollider2DComponent& ccc);

		/**
		 * @brief Creates a polygon collider for a given entity.
		 *
		 * @param entity The entity for which to create the polygon collider.
		 * @param tc The transform component of the entity.
		 * @param rbc The rigid body component of the entity.
		 * @param ccc The polygon collider component to be created.
		 */
		void CreatePolygonCollider2D(Entity entity, const TransformComponent& tc, const RigidBody2DComponent& rbc, PolygonCollider2DComponent& pcc);
	
		/**
		 * @brief Creates a distance joint between an entity and a rigid body component.
		 * 
		 * @param entity The entity to create the distance joint with.
		 * @param rbc The rigid body component of the entity.
		 * @param djc The distance joint component to be created.
		 */
		void CreateDistanceJoint2D(Entity entity, const RigidBody2DComponent& rbc, DistanceJoint2DComponent& djc);

		/**
		 * @brief Creates a 2D revolution joint between an entity and a rigid body.
		 * 
		 * @param entity The entity to create the revolution joint for.
		 * @param rbc The rigid body component of the entity.
		 * @param rjc The revolution joint component to be created and attached to the entity.
		 */
		void CreateRevolutionJoint2D(Entity entity, const RigidBody2DComponent& rbc, RevolutionJoint2DComponent& rjc);

		/**
		 * @brief Creates a prismatic joint between an entity and a rigid body component.
		 * 
		 * @param entity The entity to create the prismatic joint for.
		 * @param rbc The rigid body component of the entity.
		 * @param pjc The prismatic joint component to be created.
		 */
		void CreatePrismaticJoint2D(Entity entity, const RigidBody2DComponent& rbc, PrismaticJoint2DComponent& pjc);
		
		/**
		 * @brief Creates a spring joint between an entity and a rigid body component.
		 *
		 * @param entity The entity to create the spring joint for.
		 * @param rbc The rigid body component of the entity.
		 * @param sjc The spring joint component to be created.
		 */
		void CreateSpringJoint2D(Entity entity, const RigidBody2DComponent& rbc, SpringJoint2DComponent& sjc);

		/**
		 * @brief Creates a wheel joint between an entity and a rigid body component.
		 *
		 * @param entity The entity to create the wheel joint for.
		 * @param rbc The rigid body component of the entity.
		 * @param wjc The wheel joint component to be created.
		 */
		void CreateWheelJoint2D(Entity entity, const RigidBody2DComponent& rbc, WheelJoint2DComponent& wjc);

		/**
		 * @brief Function bound to the event of creating a rigidbody2D component.
		 * 		  Used to register the rigidbody2D component in the physics world at runtime.
		 * 
		 * @param registry The registry of the scene.
		 * @param handle The entity handle of the entity with the rigidbody2D component.
		 */
		//void OnRigidBody2DComponentCreated(entt::registry& registry, entt::entity handle);
	};

}
