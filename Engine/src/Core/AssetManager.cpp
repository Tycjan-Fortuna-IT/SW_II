#include "AssetManager.hpp"

#include "OpenGL/Texture2D.hpp"
#include "Project/ProjectContext.hpp"
#include "Project/Project.hpp"
#include "OpenGL/Font.hpp"

namespace SW {

	static Texture2D* s_WhiteTexture = nullptr;
	static Texture2D* s_BlackTexture = nullptr;

	static std::unordered_map<std::string, Texture2D*> s_Textures;
	static std::unordered_map<std::string, Texture2D*> s_EditorTextures;

	static std::unordered_map<std::string, Font*> s_Fonts;

	void AssetManager::Initialize()
	{
		s_WhiteTexture = new Texture2D(1, 1);
		u32 whiteTextureData = 0xffffffff;
		s_WhiteTexture->SetData(&whiteTextureData, sizeof(u32));

		s_BlackTexture = new Texture2D(1, 1);
		u32 blackTextureData = 0x000000ff;
		s_BlackTexture->SetData(&blackTextureData, sizeof(u32));
	}

	void AssetManager::Shutdown()
	{
		for (auto&& [path, ptr] : s_Textures) {
			delete ptr;
		}

		for (auto&& [path, ptr] : s_EditorTextures) {
			delete ptr;
		}

		for (auto&& [path, ptr] : s_Fonts) {
			delete ptr;
		}

		s_Textures.clear();
		s_EditorTextures.clear();
		s_Fonts.clear();

		delete s_WhiteTexture;
		delete s_BlackTexture;
	}

	Texture2D* AssetManager::GetWhiteTexture()
	{
		return s_WhiteTexture;
	}

	Texture2D* AssetManager::GetBlackTexture()
	{
		return s_BlackTexture;
	}

	Texture2D* AssetManager::GetTexture2D(const char* path, bool flipped)
	{
		auto texture = s_Textures.find(path);

		if (texture != s_Textures.end())
			return texture->second;

		std::filesystem::path filepath = path;
		Texture2D* newTexture = new Texture2D(filepath, flipped);
		
		s_Textures[path] = newTexture;

		return newTexture;
	}

	Texture2D* AssetManager::GetEditorTexture2D(const char* path)
	{
		auto texture = s_EditorTextures.find(path);

		if (texture != s_EditorTextures.end())
			return texture->second;

		Texture2D* newTexture = new Texture2D(path, true);

		s_EditorTextures[path] = newTexture;

		return newTexture;
	}

    Font* AssetManager::GetFont(const char* path)
    {
		auto font = s_Fonts.find(path);

		if (font != s_Fonts.end())
			return font->second;

		Font* newFont = new Font(path);

		s_Fonts[path] = newFont;

		return newFont;
    }

}
