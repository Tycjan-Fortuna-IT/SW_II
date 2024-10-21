/**
 * @file AudioEventsPanel.hpp
 * @version 0.1.0
 * @date 2024-05-12
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#include "Audio/AudioEngine.hpp"
#include "GUI/Panel.hpp"

namespace SW
{
	class AudioEventsPanel final : public Panel
	{
	public:
		AudioEventsPanel();
		~AudioEventsPanel() override = default;

		PanelType GetPanelType() const override { return PanelType::AudioEventsPanel; }

		void OnUpdate(Timestep dt) override;

		void OnRender() override;

	private:
		AudioEvent* m_SelectedEvent = nullptr;
		std::string m_Selected;
	};

} // namespace SW
