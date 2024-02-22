/**
 * @file Entity.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.2
 * @date 2024-02-22
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Debug/LogSystem.hpp"
#include "EntityRegistry.hpp"
#include "Components.hpp"
#include "Core/Scene/Scene.hpp"

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
        Entity(entt::entity entity, Scene* scene);
	
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
            return m_Scene->GetRegistry().GetRegistryHandle().all_of<T>(m_Handle);
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

            return m_Scene->GetRegistry().GetRegistryHandle().emplace<T>(m_Handle, std::forward<Args>(args)...);
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

			return m_Scene->GetRegistry().GetRegistryHandle().get<T>(m_Handle);
		}

		/**
		 * @brief Removes the component of type T from the entity.
		 * @tparam T The component type.
		 */
		template <typename T>
		void RemoveComponent()
		{
			ASSERT(HasComponent<T>(), "Entity does not have component!");

			m_Scene->GetRegistry().GetRegistryHandle().remove<T>(m_Handle);
        }

		/**
		 * @brief Gets the entity ID.
		 * 
		 * @return u64 The entity ID.
		 */
		u64 GetID() { return GetComponent<IDComponent>().ID; }

		/**
		 * @brief Gets the entity tag.
		 * 
		 * @return const std::string& The entity tag.
		 */
		const std::string& GetTag() { return GetComponent<TagComponent>().Tag; }

		/**
		 * @brief Returns the RelationshipComponent associated with the Entity.
		 *
		 * @return Reference to the RelationshipComponent.
		 */
		RelationshipComponent& GetRelations() { return GetComponent<RelationshipComponent>(); }

		/**
		 * @brief Retrieves all components of the specified types associated with the entity.
		 * 
		 * @tparam Components The types of components to retrieve.
		 * @return A tuple containing references to the components.
		 */
		template<typename... Components>
		[[nodiscard]] inline auto GetAllComponents() const
		{
			return m_Scene->GetRegistry().GetRegistryHandle().get<Components...>(m_Handle);
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

		/**
		 * Checks if the current entity is the parent of the given entity.
		 * @param entity The entity to check.
		 * @return True if the current entity is the parent of the given entity, false otherwise.
		 */
		bool IsParentOf(Entity entity)
		{
			return GetID() == entity.GetRelations().ParentID;
		}

		/**
		 * Checks if the entity is a child of the specified entity.
		 *
		 * @param entity The entity to check if it is the parent.
		 * @return True if the entity is a child of the specified entity, false otherwise.
		 */
		bool IsChildOf(Entity entity)
		{
			return GetRelations().ParentID == entity.GetID();
		}

		/**
		 * @brief Retrieves the parent entity of the current entity.
		 * @warning If the entity has no parent, an empty entity is returned.
		 * 
		 * @return The parent entity, or an empty entity if there is no parent.
		 */
		Entity GetParent()
		{
			RelationshipComponent& rc  = GetComponent<RelationshipComponent>();

			return rc.ParentID != 0 ? m_Scene->GetEntityByID(rc.ParentID) : Entity{};
		}

		/**
		 * Sets the parent of the entity.
		 * @warning If the entity already has a parent, the old parent will be replaced by the new one.
		 * @warning DO NOT pass invalid entities to this function. like {}.
		 * @param parent The entity to set as the parent.
		 */
		void SetParent(Entity parent)
		{
			RelationshipComponent& rc = GetComponent<RelationshipComponent>();

			if (rc.ParentID == 0) {
				rc.ParentID = parent.GetID();
				parent.GetRelations().ChildrenIDs.emplace_back(GetID());
				return;
			}

			Entity oldParent = GetParent();
			RelationshipComponent& oldParentRelations = oldParent.GetRelations();

			for (auto it = oldParentRelations.ChildrenIDs.begin(); it != oldParentRelations.ChildrenIDs.end(); it++) {
				if (*it == GetID()) {
					oldParentRelations.ChildrenIDs.erase(it);
					break;
				}
			}
			
			rc.ParentID = parent.GetID();
			parent.GetRelations().ChildrenIDs.emplace_back(GetID());
		}

		/**
		 * @brief Removes the parent relationship of the entity.
		 * 
		 * This function removes the parent relationship of the entity by updating the RelationshipComponent
		 * of both the entity and its parent. It removes the entity's ID from the parent's list of children IDs.
		 * 
		 * @note If the entity does not have a parent, this function does nothing.
		 */
		void RemoveParent()
		{
			RelationshipComponent& rc = GetComponent<RelationshipComponent>();

			Entity oldParent = GetParent();

			if (!oldParent) {
				return;
			}

			RelationshipComponent& oldParentRelations = oldParent.GetRelations();

			for (auto it = oldParentRelations.ChildrenIDs.begin(); it != oldParentRelations.ChildrenIDs.end(); it++) {
				if (*it == GetID()) {
					oldParentRelations.ChildrenIDs.erase(it);
					break;
				}
			}

			rc.ParentID = 0;
		}

    private:
        entt::entity m_Handle{ entt::null };	/** @brief The internal entity handle. */
		Scene* m_Scene = nullptr;				/** @brief The scene to which the entity belongs. */
    };

}
