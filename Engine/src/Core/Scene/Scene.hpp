/**
 * @file Scene.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.2
 * @date 2024-01-20
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/ECS/Entity.hpp"
#include "Core/Scene/SceneCamera.hpp"
#include "Core/Timestep.hpp"

namespace SW {

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
		 * @brief Destroys the specified entity in the scene.
		 * @param entity The entity to destroy.
		 */
		void DestroyEntity(Entity entity);

		/**
		 * @brief Updates the scene with the specified timestep and camera.
		 * @param dt The timestep since the last update.
		 * @param camera The camera to use for rendering the scene.
		 */
		void OnUpdate(Timestep dt, const SceneCamera& camera);

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

	private:
		EntityRegistry m_Registry; /**< The entity registry of the scene. */

		std::unordered_map<u64, entt::entity> m_EntityMap = {}; /**< Map of entity IDs to entt::entity handles. (cache) */

		u32 m_ViewportWidth = 0; /**< The width of the viewport. */
		u32 m_ViewportHeight = 0; /**< The height of the viewport. */
	};

}
