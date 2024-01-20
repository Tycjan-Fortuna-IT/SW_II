/**
 * @file SceneHierarchyPanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <imgui.h>

#include "GUI/Panel.hpp"
#include "GUI/GUI.hpp"
#include "Core/ECS/Components.hpp"
#include "Core/ECS/Entity.hpp"

namespace SW {

	/**
	 * @brief The SceneHierarchyPanel class represents a panel that displays all entities in the scene
	 *        in a tree-like structure.
	 */
	class SceneHierarchyPanel final : public Panel
	{
	public:
		/**
		 * @brief Constructs a SceneHierarchyPanel object with an optional name.
		 * 
		 * @param name The name of the panel. Default is "Stats".
		 */
		explicit SceneHierarchyPanel(const char* name = "Scene Hierarchy");

		/**
		 * @brief Default destructor for the SceneHierarchyPanel class.
		 */
		~SceneHierarchyPanel() override = default;

		/** @brief Copy constructor (deleted). */
		SceneHierarchyPanel(const SceneHierarchyPanel& other) = delete;
		/** @brief Move constructor (deleted). */
		SceneHierarchyPanel(SceneHierarchyPanel&& other) = delete;
		/** @brief Copy assignment operator (deleted). */
		SceneHierarchyPanel& operator=(const SceneHierarchyPanel& other) = delete;
		/** @brief Move assignment operator (deleted). */
		SceneHierarchyPanel& operator=(SceneHierarchyPanel&& other) = delete;

		/**
		 * @brief Called every frame to update the panel.
		 * @param dt The time since the last frame.
		 */
		void OnUpdate(Timestep dt) override;

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() override;

		void RenderEntityNode(Entity entity, const IDComponent& idc, const TagComponent& tc);

	private:
		GUI::TextFilter m_SearchFilter;
	};

}
