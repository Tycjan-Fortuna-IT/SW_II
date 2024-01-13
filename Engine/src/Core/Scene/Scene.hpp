/**
 * @file Scene.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-13
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/ECS/Entity.hpp"
#include "Core/OpenGL/OrthographicCamera.hpp"

namespace SW {

	class Scene final
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& tag = "Entity");
		void DestroyEntity(Entity entity);

		void OnUpdate(f32 dt, const OrthographicCamera& camera);

		void OnViewportResize(u32 width, u32 height);

		EntityRegistry& GetRegistry() { return m_Registry; }

	private:
		EntityRegistry m_Registry;

		u32 m_ViewportWidth = 0;
		u32 m_ViewportHeight = 0;
	};

}
