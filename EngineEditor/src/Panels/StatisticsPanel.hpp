/**
 * @file StatisticsPanel.hpp
 * @version 0.1.0
 * @date 2024-05-12
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#include <vector>

#include "GUI/Panel.hpp"

namespace SW
{
	/**
	 * @brief The StatisticsPanel class represents a panel that displays statistics.
	 */
	class StatisticsPanel final : public Panel
	{
	public:
		StatisticsPanel();
		~StatisticsPanel() override = default;

		StatisticsPanel(const StatisticsPanel& other)            = delete;
		StatisticsPanel(StatisticsPanel&& other)                 = delete;
		StatisticsPanel& operator=(const StatisticsPanel& other) = delete;
		StatisticsPanel& operator=(StatisticsPanel&& other)      = delete;

		PanelType GetPanelType() const override { return PanelType::StatisticsPanel; }

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() override;

		void OnUpdate(UNUSED Timestep dt) override {}

	private:
		float m_FpsValues[200]        = {}; ///< Last 200 fps values
		std::vector<f32> m_FrameTimes = {}; ///< Last 200 frame times
	};

} // namespace SW
