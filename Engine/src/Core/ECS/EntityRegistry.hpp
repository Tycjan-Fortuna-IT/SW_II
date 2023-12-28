#pragma once

#include <entt.hpp>

namespace SW {

    class Entity;

    class EntityRegistry final
    {
    public:
        EntityRegistry() = default;
        ~EntityRegistry() = default;

        Entity CreateEntity();

        void DestroyEntity(Entity entity);

        static entt::registry& Get() { return s_EnttHandle; }

    private:
        static entt::registry s_EnttHandle;
    };

}
