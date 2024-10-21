#pragma once

#include "Core/OpenGL/Texture2D.hpp"

namespace SW
{

	class EditorResources
	{
	public:
		static void Initialize()
		{
			const std::filesystem::path exePath = std::filesystem::current_path();

			SW_Icon      = new Texture2D(exePath / "assets/icons/sw.png", true);
			CloseIcon    = new Texture2D(exePath / "assets/icons/editor/windows/Close.png", true);
			MaximizeIcon = new Texture2D(exePath / "assets/icons/editor/windows/Maximize.png", true);
			MinimizeIcon = new Texture2D(exePath / "assets/icons/editor/windows/Minimize.png", true);
			RestoreIcon  = new Texture2D(exePath / "assets/icons/editor/windows/Restore.png", true);

			MissingAssetIcon   = new Texture2D(exePath / "assets/icons/editor/asset_icons/missing.jpg", true);
			UnknownAssetIcon   = new Texture2D(exePath / "assets/icons/editor/asset_icons/unknown_icon.png", true);
			DirectoryAssetIcon = new Texture2D(exePath / "assets/icons/editor/asset_icons/directory_icon.png", true);
			FontAssetIcon      = new Texture2D(exePath / "assets/icons/editor/asset_icons/font_icon.png", true);
			SceneAssetIcon     = new Texture2D(exePath / "assets/icons/editor/asset_icons/scene_icon.png", true);
			PrefabAssetIcon    = new Texture2D(exePath / "assets/icons/editor/asset_icons/prefab_icon.png", true);
			ScriptAssetIcon    = new Texture2D(exePath / "assets/icons/editor/asset_icons/script_icon.png", true);
			ShaderAssetIcon    = new Texture2D(exePath / "assets/icons/editor/asset_icons/shader_icon.png", true);
			AudioAssetIcon     = new Texture2D(exePath / "assets/icons/editor/asset_icons/audio_icon.png", true);
		}

		static void Shutdown()
		{
			delete SW_Icon;
			delete CloseIcon;
			delete MaximizeIcon;
			delete MinimizeIcon;
			delete RestoreIcon;

			delete MissingAssetIcon;
			delete UnknownAssetIcon;
			delete DirectoryAssetIcon;
			delete FontAssetIcon;
			delete SceneAssetIcon;
			delete PrefabAssetIcon;
			delete ScriptAssetIcon;
			delete ShaderAssetIcon;
			delete AudioAssetIcon;
		}

		// EngineEditor icons
		inline static Texture2D* SW_Icon      = nullptr;
		inline static Texture2D* CloseIcon    = nullptr;
		inline static Texture2D* MaximizeIcon = nullptr;
		inline static Texture2D* MinimizeIcon = nullptr;
		inline static Texture2D* RestoreIcon  = nullptr;

		// AssetSourceType icons
		inline static Texture2D* MissingAssetIcon   = nullptr;
		inline static Texture2D* UnknownAssetIcon   = nullptr;
		inline static Texture2D* DirectoryAssetIcon = nullptr;
		inline static Texture2D* FontAssetIcon      = nullptr;
		inline static Texture2D* SceneAssetIcon     = nullptr;
		inline static Texture2D* PrefabAssetIcon    = nullptr;
		inline static Texture2D* ScriptAssetIcon    = nullptr;
		inline static Texture2D* ShaderAssetIcon    = nullptr;
		inline static Texture2D* AudioAssetIcon     = nullptr;
	};

} // namespace SW
