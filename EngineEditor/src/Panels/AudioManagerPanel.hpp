#pragma once

#include "GUI/Panel.hpp"

namespace SW {

	class AudioManagerPanel final : public Panel
	{
	public:
		AudioManagerPanel();
		~AudioManagerPanel() override = default;

		PanelType GetPanelType() const override { return PanelType::AudioManagerPanel; }

		void OnUpdate(Timestep dt) override;

		void OnRender() override;

	private:

	};

}

