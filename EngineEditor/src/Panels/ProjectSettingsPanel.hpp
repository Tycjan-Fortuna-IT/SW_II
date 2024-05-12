#pragma once

#include "GUI/Panel.hpp"

namespace SW {

	class ProjectSettingsPanel final : public Panel
	{
	public:
		ProjectSettingsPanel();
		~ProjectSettingsPanel() override = default;

		PanelType GetPanelType() const override { return PanelType::ProjectSettingsPanel; }

		void OnUpdate(Timestep dt) override;

		void OnRender() override;
	};

}
