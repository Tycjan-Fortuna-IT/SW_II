/**
 * @file EntityRegistry.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <entt.hpp>

namespace SW {

    class Entity;

	/**
	 * @brief The EntityRegistry class is responsible for managing entities in the ECS system.
	 * 
	 * It provides functionality to create and destroy entities, as well as access to the underlying entt::registry.
	 */
	class EntityRegistry final
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		EntityRegistry() = default;

		/**
		 * @brief Default destructor.
		 */
		~EntityRegistry() = default;

		/**
		 * @brief Creates a new entity.
		 * 
		 * @return The created entity.
		 */
		Entity CreateEntity();

		/**
		 * @brief Destroys an entity.
		 * 
		 * @param entity The entity to destroy.
		 */
		void DestroyEntity(Entity entity);

		/**
		 * @brief Gets a view of entities with the given components.
		 * 
		 * @tparam Args The component types.
		 * @return A view of entities with the given components.
		 */
		template <typename... Args>
		[[nodiscard]] static auto GetEntitiesWith()
		{
			return s_EnttHandle.view<Args...>();
		}

		/**
		 * @brief Gets the underlying entt::registry instance.
		 * 
		 * @return A reference to the entt::registry instance.
		 */
		static entt::registry& GetRegistry() { return s_EnttHandle; }

	private:
		static entt::registry s_EnttHandle;			///< The underlying entt::registry instance.
	};

}
