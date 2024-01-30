/**
 * @file Scene.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.3
 * @date 2024-01-27
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/ECS/Entity.hpp"
#include "Core/Scene/SceneCamera.hpp"
#include "Core/Timestep.hpp"

namespace SW {

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
		Scene();

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
		 * @brief Updates the scene with the specified timestep and camera.
		 * @param dt The timestep since the last update.
		 * @param camera The camera to use for rendering the scene.
		 */
		void OnUpdate(Timestep dt, const SceneCamera& camera);

		/**
		 * @brief Updates the scene in edit mode with the specified timestep and camera.
		 * @param dt The timestep since the last update.
		 * @param camera The camera to use for rendering the scene.
		 */
		void OnUpdateEditor(Timestep dt, const SceneCamera& camera);

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
		Entity GetEntityByID(u64 id) const;

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

	private:
		EntityRegistry m_Registry; /**< The entity registry of the scene. */

		std::unordered_map<u64, entt::entity> m_EntityMap = {}; /**< Map of entity IDs to entt::entity handles. (cache) */

		u32 m_ViewportWidth = 0; /**< The width of the viewport. */
		u32 m_ViewportHeight = 0; /**< The height of the viewport. */

		SceneState m_SceneState = SceneState::Edit;	/**< The current state of the scene. */
	};

}
