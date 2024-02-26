/**
 * @file Scene.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.2
 * @date 2024-02-26
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Scene/SceneCamera.hpp"
#include "Core/Timestep.hpp"
#include "Core/Math/Vector2.hpp"
#include "Core/ECS/EntityRegistry.hpp"
#include "Core/ECS/Components.hpp"

class b2World;

namespace SW {

	class Entity;
	class EditorCamera;
	class Physics2DContactListener;
	
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
	class Scene final
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		Scene(const std::string& filepath);

		/**
		 * @brief Destructor.
		 */
		~Scene();

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
		 * Begins the rendering process using the specified camera.
		 *
		 * @param camera The camera to be used for rendering.
		 * @return Whether the scene rendering process can be proceeded.
		 */
		bool BeginRendering(EditorCamera* camera);

		/**
		 * Ends the rendering process for the scene.
		 */
		void EndRendering();

		/**
		 * @brief This function is called when the scene is started (the state has changed)
		 */
		void OnRuntimeStart();

		/**
		 * @brief This function is called when the scene is stopped (the state has changed)
		 */
		void OnRuntimeStop();

		/**
		 * @brief Updates the scene with the specified timestep and camera.
		 * @param dt The timestep since the last update.
		 * @param camera The camera to use for rendering the scene.
		 */
		void OnUpdate(Timestep dt);

		/**
		 * @brief Updates the scene in edit mode with the specified timestep and camera.
		 * @param dt The timestep since the last update.
		 * @param camera The camera to use for rendering the scene.
		 */
		void OnUpdateEditor(Timestep dt);

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
		 * @return The entity with the specified ID, or an invalid entity if not found.
		 */
		Entity GetEntityByID(u64 id);

		/**
		 * @brief Retrieves the filepath to the serialized scene.
		 * @return The filepath to the serialized scene.
		 */
		const std::string& GetFilePath() const { return m_FilePath; }

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
		 * @brief Copies the scene. (deep copy with all components and entities)
		 * 
		 * @return Scene* The copy of the scene.
		 */
		Scene* DeepCopy();

		glm::vec2 Gravity = { 0.0f, -9.80665f };	/**< The gravity of the scene. */

	private:
		EntityRegistry m_Registry; /**< The entity registry of the scene. */

		std::string m_FilePath; /**< The filepath to the serialized scene file. */

		std::unordered_map<u64, entt::entity> m_EntityMap = {}; /**< Map of entity IDs to entt::entity handles. (cache) */

		u32 m_ViewportWidth = 0; /**< The width of the viewport. */
		u32 m_ViewportHeight = 0; /**< The height of the viewport. */

		SceneState m_SceneState = SceneState::Edit;	/**< The current state of the scene. */

		b2World* m_PhysicsWorld2D;		/**< The physics world of the scene. */
		Physics2DContactListener* m_PhysicsContactListener2D = nullptr;

		u32 m_VelocityIterations = 6;			/**< The number of velocity iterations for the physics simulation. */
		u32 m_PositionIterations = 2;			/**< The number of position iterations for the physics simulation. */
		f32 m_PhysicsFrameAccumulator = 0.0f;	/**< The frame accumulator for the physics simulation. */

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
	};

}
