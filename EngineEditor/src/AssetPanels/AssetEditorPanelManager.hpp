/**
 * @file AssetEditorPanelManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-17
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset/Asset.hpp"
#include "Asset/Panels/AssetEditorPanel.hpp"

namespace SW
{
	class AssetEditorPanelManager
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void OnUpdate(Timestep ts);

		void SetSceneContext(Scene* scene);

		static void OnRender();

		static void OpenEditor(AssetHandle handle, AssetType type);

	private:
		static std::unordered_map<AssetType, Scope<AssetEditorPanel>> s_Editors;

		static Scene* s_SceneContext;
	};

} // namespace SW
