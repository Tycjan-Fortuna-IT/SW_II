#pragma once

#include "GUI/Panel.hpp"

namespace SW
{
	class AssetManagerPanel final : public Panel
	{
	public:
		AssetManagerPanel();
		~AssetManagerPanel() override = default;

		PanelType GetPanelType() const override { return PanelType::AssetManagerPanel; }

		void OnUpdate(Timestep dt) override;

		void OnRender() override;

	private:
		std::string m_AvailableSearchString;
		std::string m_LoadedSearchString;
	};

} // namespace SW
