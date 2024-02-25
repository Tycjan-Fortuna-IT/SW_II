/**
 * @file PropertiesPanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.3
 * @date 2024-01-30
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "GUI/Panel.hpp"
#include "SceneViewportPanel.hpp"

namespace SW {

	/**
	 * @brief The PropertiesPanel class represents a panel that displays for the selected object its properties and components.
	 * 		  Allows to edit the properties and add/remove components.
	 */
	class PropertiesPanel final : public Panel {
	public:
		/**
		 * @brief Constructs a PropertiesPanel object with an optional name.
		 *
		 * @param context Current scene context.
		 */
		explicit PropertiesPanel(SceneViewportPanel* sceneViewportPanel);

		/**
		 * @brief Default destructor for the PropertiesPanel class.
		 */
		~PropertiesPanel() override = default;

		/** @brief Copy constructor (deleted). */
		PropertiesPanel(const PropertiesPanel& other) = delete;
		/** @brief Move constructor (deleted). */
		PropertiesPanel(PropertiesPanel&& other) = delete;
		/** @brief Copy assignment operator (deleted). */
		PropertiesPanel& operator=(const PropertiesPanel& other) = delete;
		/** @brief Move assignment operator (deleted). */
		PropertiesPanel& operator=(PropertiesPanel&& other) = delete;

		/**
		 * @brief Called every frame to update the panel.
		 * @param dt The time since the last frame.
		 */
		void OnUpdate(Timestep dt) override;

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() override;

	private:
		SceneViewportPanel* m_SceneViewportPanel = nullptr;	/**< The current scene viewport context. */

		void DrawAddComponentMenu(Entity entity);
	};

}
