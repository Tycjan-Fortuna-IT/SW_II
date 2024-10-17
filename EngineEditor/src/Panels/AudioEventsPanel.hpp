#pragma once

#include "GUI/Panel.hpp"
#include "Audio/AudioEngine.hpp"

namespace SW {

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

}

