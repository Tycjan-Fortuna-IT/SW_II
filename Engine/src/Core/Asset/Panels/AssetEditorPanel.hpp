/**
 * @file AssetEditorPanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-17
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Asset/Asset.hpp"
#include "Core/Timestep.hpp"
#include "Core/Scene/Scene.hpp"

namespace SW {

	class AssetEditorPanel
	{		
	protected:
		AssetEditorPanel(const char* name, const char* icon);

	public:
		virtual ~AssetEditorPanel() = default;

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnRender();

		void Open();
		void Close();

		void SetSceneContext(Scene* scene) { m_Scene = scene; }
		
		virtual void SetAsset(Asset* asset) = 0;
	
	protected:
		virtual void OnWindowStylePush() {}
		virtual void OnWindowStylePop() {}

		std::string m_ID;   ///< The ID of the panel.
		std::string m_Name; ///< The name of the panel.
		std::string m_Icon; ///< The icon of the panel.

		ImVec2 m_MinSize, m_MaxSize;

		bool m_IsOpen = false;

		Scene* m_Scene = nullptr;

	private:
		virtual void OnOpen() = 0;
		virtual void OnClose() = 0;
		virtual void Render() = 0;
	};

}

