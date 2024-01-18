/**
 * @file StatisticsPanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <vector>

#include "GUI/Panel.hpp"

namespace SW {

	/**
	 * @brief The StatisticsPanel class represents a panel that displays statistics.
	 */
	class StatisticsPanel final : public Panel
	{
	public:
		/**
		 * @brief Constructs a StatisticsPanel object with an optional name.
		 * 
		 * @param name The name of the panel. Default is "Stats".
		 */
		explicit StatisticsPanel(const char* name = "Stats");

		/**
		 * @brief Default destructor for the StatisticsPanel class.
		 */
		~StatisticsPanel() override = default;

		/** @brief Copy constructor (deleted). */
		StatisticsPanel(const StatisticsPanel& other) = delete;
		/** @brief Move constructor (deleted). */
		StatisticsPanel(StatisticsPanel&& other) = delete;
		/** @brief Copy assignment operator (deleted). */
		StatisticsPanel& operator=(const StatisticsPanel& other) = delete;
		/** @brief Move assignment operator (deleted). */
		StatisticsPanel& operator=(StatisticsPanel&& other) = delete;

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() override;

	private:
		float m_FpsValues[200];					///< Last 200 fps values
		std::vector<f32> m_FrameTimes = {};		///< Last 200 frame times
	};

}
