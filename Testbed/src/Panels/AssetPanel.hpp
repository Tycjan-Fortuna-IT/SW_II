/**
 * @file AssetPanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "GUI/Panel.hpp"

namespace SW {

	/**
	 * @brief The AssetPanel class represents a panel that displays the console window.
	 *        in a tree-like structure.
	 */
	class AssetPanel final : public Panel
	{
	public:
		/**
		 * @brief Constructs a AssetPanel object with an optional name.
		 * 
		 * @param name The name of the panel. Default is "Assets".
		 */
		explicit AssetPanel(const char* name = "Assets");

		/**
		 * @brief Default destructor for the AssetPanel class.
		 */
		~AssetPanel() override = default;

		/** @brief Copy constructor (deleted). */
		AssetPanel(const AssetPanel& other) = delete;
		/** @brief Move constructor (deleted). */
		AssetPanel(AssetPanel&& other) = delete;
		/** @brief Copy assignment operator (deleted). */
		AssetPanel& operator=(const AssetPanel& other) = delete;
		/** @brief Move assignment operator (deleted). */
		AssetPanel& operator=(AssetPanel&& other) = delete;

		/**
		 * @brief Called every frame to update the panel.
		 * @param dt The time since the last frame.
		 */
		void OnUpdate(Timestep dt) override;

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() override;
	};

}
