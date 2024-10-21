#include "PanelManager.hpp"

namespace SW
{
	std::unordered_map<PanelType, Panel*> PanelManager::s_Panels;

	void PanelManager::Initialize()
	{
	}

	void PanelManager::Shutdown()
	{
		for (auto& [type, panel] : s_Panels)
		{
			delete panel;
		}

		s_Panels.clear();
	}

	void PanelManager::OnUpdate(Timestep ts)
	{
		for (auto& [type, panel] : s_Panels)
		{
			if (panel->IsOpen())
				panel->OnUpdate(ts);
		}
	}

	void PanelManager::OnRender()
	{
		for (auto& [type, panel] : s_Panels)
		{
			if (panel->IsOpen())
				panel->OnRender();
		}
	}

	void PanelManager::AddPanel(PanelType type, Panel* panel)
	{
		s_Panels[type] = panel;
	}

	Panel* PanelManager::GetPanel(PanelType type)
	{
		return s_Panels.at(type);
	}

	void PanelManager::RemovePanel(PanelType type)
	{
		delete s_Panels[type];
		s_Panels.erase(type);
	}

	void PanelManager::OpenPanel(PanelType type)
	{
		s_Panels.at(type)->Open();
	}

	void PanelManager::ClosePanel(PanelType type)
	{
		s_Panels.at(type)->Close();
	}

} // namespace SW
