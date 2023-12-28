#pragma once

#include "EntityRegistry.hpp"

namespace SW {

    class Entity final
    {
    public:
        Entity() = default;
        Entity(entt::entity entity);
        Entity(const Entity& other) = default;

        template <typename T>
        bool HasComponent() const {
            return EntityRegistry::Get().all_of<T>(m_Handle);
        }

        template <typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            ASSERT(!HasComponent<T>(), "Entity already has component!");

            return EntityRegistry::Get().emplace<T>(m_Handle, std::forward<Args>(args)...);
        }

        template <typename T>
        T& GetComponent() {
            ASSERT(HasComponent<T>(), "Entity does not have component!");

            return EntityRegistry::Get().get<T>(m_Handle);
        }

        template <typename T>
        void RemoveComponent() {
            ASSERT(HasComponent<T>(), "Entity does not have component!");

            EntityRegistry::Get().remove<T>(m_Handle);
        }

        operator bool() const { return m_Handle != entt::null; }
        operator entt::entity() const { return m_Handle; }
        operator u32() const { return (u32)m_Handle; }

    private:
        entt::entity m_Handle{ entt::null };
    };

}
