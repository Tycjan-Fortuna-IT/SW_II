#include "AssetEditorPanelManager.hpp"
#include "SpritesheetEditor.hpp"

namespace SW {

	std::unordered_map<SW::AssetType, AssetEditorPanel*> AssetEditorPanelManager::s_Editors;

	SW::Scene* AssetEditorPanelManager::s_SceneContext;

	void AssetEditorPanelManager::Initialize()
	{
		s_Editors[AssetType::Spritesheet] = new SpritesheetEditor("Spritesheet Editor", SW_ICON_TEXTURE);
	}

	void AssetEditorPanelManager::Shutdown()
	{
		delete s_Editors[AssetType::Spritesheet];

		s_Editors.clear();
	}

	void AssetEditorPanelManager::OnUpdate(Timestep ts)
	{
		for (auto& kv : s_Editors)
			kv.second->OnUpdate(ts);
	}

	void AssetEditorPanelManager::SetSceneContext(Scene* scene)
	{
		s_SceneContext = scene;

		for (auto& kv : s_Editors)
			kv.second->SetSceneContext(scene);
	}

	void AssetEditorPanelManager::OnRender()
	{
		for (auto& kv : s_Editors)
			kv.second->OnRender();
	}

	void AssetEditorPanelManager::OpenEditor(Asset* asset)
	{
		if (asset == nullptr)
			return;

		if (s_Editors.find(asset->GetAssetType()) == s_Editors.end())
			return;

		AssetEditorPanel* editor = s_Editors[asset->GetAssetType()];

		editor->SetAsset(asset);
		editor->SetSceneContext(s_SceneContext);
		editor->Open();
	}

}
