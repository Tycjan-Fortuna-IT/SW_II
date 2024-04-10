#include "AssetEditorPanelManager.hpp"

#include "SpritesheetEditor.hpp"
#include "AnimationEditor.hpp"

namespace SW {

	std::unordered_map<AssetType, AssetEditorPanel*> AssetEditorPanelManager::s_Editors;

	SW::Scene* AssetEditorPanelManager::s_SceneContext;

	void AssetEditorPanelManager::Initialize()
	{
		//Scope<SpritesheetEditor> ptr = CreateScope<SpritesheetEditor>("Animation Editor", SW_ICON_CAMERA);

		s_Editors[AssetType::Spritesheet] = new SpritesheetEditor("Spritesheet Editor", SW_ICON_TEXTURE);
		s_Editors[AssetType::Animation] = new AnimationEditor("Animation Editor", SW_ICON_CAMERA);
	}

	void AssetEditorPanelManager::Shutdown()
	{
		delete s_Editors[AssetType::Spritesheet];
		delete s_Editors[AssetType::Animation];
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

	void AssetEditorPanelManager::OpenEditor(AssetHandle handle, AssetType type)
	{
		if (!handle)
			return;

		if (s_Editors.find(type) == s_Editors.end())
			return;

		AssetEditorPanel* editor = s_Editors[type];

		editor->SetAssetHandle(handle);
		editor->SetSceneContext(s_SceneContext);
		editor->Open();
	}

}
