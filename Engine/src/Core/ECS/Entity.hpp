/**
 * @file Entity.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Debug/LogSystem.hpp"
#include "EntityRegistry.hpp"

namespace SW {

	/**
	 * @brief Represents an entity in the ECS (Entity Component System).
	 */
    class Entity final
    {
    public:
		/**
		 * @brief Default constructor.
		 */
        Entity() = default;
		
		/**
		 * @brief Constructs an Entity object with the given entity handle.
		 * @param entity The entity handle.
		 */
        Entity(entt::entity entity);
	
		/**
		 * @brief Copy constructor.
		 * @param other The other Entity object to copy from.
		 */
        Entity(const Entity& other) = default;

		/**
		 * @brief Checks if the entity has a component of type T.
		 * @tparam T The component type.
		 * @return True if the entity has the component, false otherwise.
		 */
        template <typename T>
        bool HasComponent() const
		{
            return EntityRegistry::GetRegistry().all_of<T>(m_Handle);
        }

		/**
		 * @brief Adds a component of type T to the entity.
		 * @tparam T The component type.
		 * @tparam Args The argument types for constructing the component.
		 * @param args The arguments for constructing the component.
		 * @return The reference to the added component.
		 */
        template <typename T, typename... Args>
        T& AddComponent(Args&&... args)
		{
            ASSERT(!HasComponent<T>(), "Entity already has component!");

            return EntityRegistry::GetRegistry().emplace<T>(m_Handle, std::forward<Args>(args)...);
        }

		/**
		 * @brief Gets the component of type T from the entity.
		 * @tparam T The component type.
		 * @return The reference to the component.
		 */
        template <typename T>
        T& GetComponent()
		{
            ASSERT(HasComponent<T>(), "Entity does not have component!");

            return EntityRegistry::GetRegistry().get<T>(m_Handle);
        }

		/**
		 * @brief Removes the component of type T from the entity.
		 * @tparam T The component type.
		 */
        template <typename T>
        void RemoveComponent()
		{
            ASSERT(HasComponent<T>(), "Entity does not have component!");

            EntityRegistry::GetRegistry().remove<T>(m_Handle);
        }

		/**
		 * @brief Conversion operator to check if the entity is valid.
		 * @return True if the entity is valid, false otherwise.
		 */
        operator bool() const { return m_Handle != entt::null; }
		
		/**
		 * @brief Conversion operator to get the entity handle.
		 * @return The entity handle.
		 */
        operator entt::entity() const { return m_Handle; }

		/**
		 * @brief Conversion operator to get the entity handle as an unsigned integer.
		 *        Basically it's internal id.
		 * @return The entity handle as an unsigned integer.
		 */
        operator u32() const { return (u32)m_Handle; }

    private:
        entt::entity m_Handle{ entt::null };	///< The entity handle.
    };

}
