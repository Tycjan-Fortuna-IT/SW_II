#pragma once

#include "AssetRegistry.hpp"

namespace SW {

	class AssetManager_v2
	{
	public:
		static void Initialize();
		static void Shutdown();

		template <typename T>
		static const T* GetAsset(AssetHandle handle)
		{
			return nullptr;
		}

		template <typename T>
		static T* GetAssetRaw(AssetHandle handle)
		{
			return nullptr;
		}

		static Texture2D* GetWhiteTexture();

		static Texture2D* GetBlackTexture();

		static Texture2D* GetEditorTexture2D(const std::filesystem::path path);

		static const AssetRegistry& GetRegistry() { return s_Registry; }

	private:
		static AssetRegistry s_Registry; // All asset pointers are owned by the AssetRegistry!

		static Texture2D* s_WhiteTexture;
		static Texture2D* s_BlackTexture;

		// Editor only textures! In the future part of EditorAssetManager
		static std::unordered_map<std::filesystem::path, Texture2D*> s_EditorTextures;
	};

}
