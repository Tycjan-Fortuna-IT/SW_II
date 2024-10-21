/**
 * @file Panel.hpp
 * @version 0.1.0
 * @date 2024-05-12
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#include "Core/Timestep.hpp"
#include "GUI/Panel.hpp"

namespace SW
{
	class PanelManager
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void OnUpdate(Timestep ts);
		static void OnRender();

		static void AddPanel(PanelType type, Panel* panel);
		static Panel* GetPanel(PanelType type);
		static void RemovePanel(PanelType type);
		static void OpenPanel(PanelType type);
		static void ClosePanel(PanelType type);

		static const std::unordered_map<PanelType, Panel*>& GetPanels() { return s_Panels; }

	private:
		static std::unordered_map<PanelType, Panel*> s_Panels;
	};

} // namespace SW
