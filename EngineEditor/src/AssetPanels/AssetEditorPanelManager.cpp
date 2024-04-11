#include "AssetEditorPanelManager.hpp"

#include "SpritesheetEditor.hpp"
#include "AnimationEditor.hpp"

namespace SW {

	std::unordered_map<AssetType, Scope<AssetEditorPanel>> AssetEditorPanelManager::s_Editors;

	SW::Scene* AssetEditorPanelManager::s_SceneContext;

	void AssetEditorPanelManager::Initialize()
	{
		s_Editors[AssetType::Spritesheet] = CreateScope<SpritesheetEditor>("Spritesheet Editor", SW_ICON_TEXTURE);
		s_Editors[AssetType::Animation2D] = CreateScope<AnimationEditor>("Animation Editor", SW_ICON_VIDEO);
	}

	void AssetEditorPanelManager::Shutdown()
	{
		s_Editors.clear();
	}

	void AssetEditorPanelManager::OnUpdate(Timestep ts)
	{
		for (auto& kv : s_Editors)
			if (kv.second->IsOpen())
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
			if (kv.second->IsOpen())
				kv.second->OnRender();
	}

	void AssetEditorPanelManager::OpenEditor(AssetHandle handle, AssetType type)
	{
		if (!handle)
			return;

		if (s_Editors.find(type) == s_Editors.end())
			return;

		const Scope<AssetEditorPanel>& editor = s_Editors[type];

		editor->SetAssetHandle(handle);
		editor->SetSceneContext(s_SceneContext);
		editor->Open();
	}

}
