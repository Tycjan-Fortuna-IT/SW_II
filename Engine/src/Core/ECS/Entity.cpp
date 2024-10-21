#include "Entity.hpp"

namespace SW
{

	Entity::Entity(entt::entity entity, Scene* scene) : m_Handle(entity), m_Scene(scene)
	{
	}

} // namespace SW
