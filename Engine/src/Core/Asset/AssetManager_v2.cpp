#include "AssetManager_v2.hpp"

#include "AssetLoader.hpp"

namespace SW {

	AssetRegistry AssetManager_v2::s_Registry;

	Texture2D* AssetManager_v2::s_WhiteTexture;
	Texture2D* AssetManager_v2::s_BlackTexture;

	std::unordered_map<std::filesystem::path, Texture2D*> AssetManager_v2::s_EditorTextures;

	void AssetManager_v2::Initialize()
	{
		AssetLoader::Initialize();

		s_Registry.Initialize();

		s_WhiteTexture = new Texture2D(1, 1);
		u32 whiteTextureData = 0xffffffff;
		s_WhiteTexture->SetData(&whiteTextureData, sizeof(u32));

		s_BlackTexture = new Texture2D(1, 1);
		u32 blackTextureData = 0x000000ff;
		s_BlackTexture->SetData(&blackTextureData, sizeof(u32));
	}

	void AssetManager_v2::Shutdown()
	{
		AssetLoader::Shutdown();

		s_Registry.Shutdown();

		for (auto&& [path, ptr] : s_EditorTextures) {
			delete ptr;
		}

		s_EditorTextures.clear();

		delete s_WhiteTexture;
		delete s_BlackTexture;
	}

	Texture2D* AssetManager_v2::GetWhiteTexture()
	{
		return s_WhiteTexture;
	}

	Texture2D* AssetManager_v2::GetBlackTexture()
	{
		return s_BlackTexture;
	}

	Texture2D* AssetManager_v2::GetEditorTexture2D(const std::filesystem::path path)
	{
		auto texture = s_EditorTextures.find(path);

		if (texture != s_EditorTextures.end())
			return texture->second;

		Texture2D* newTexture = new Texture2D(path, true);

		s_EditorTextures[path] = newTexture;

		return newTexture;
	}

}
