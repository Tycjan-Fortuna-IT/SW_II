#include "AssetLoader.hpp"

#include "AssetSerializer.hpp"

namespace SW {

	std::unordered_map<AssetType, AssetSerializer*> AssetLoader::s_Serializers;

	void AssetLoader::Initialize()
	{
		s_Serializers[AssetType::Spritesheet] = new SpritesheetSerializer();
	}

	void AssetLoader::Shutdown()
	{
		delete s_Serializers[AssetType::Spritesheet];

		s_Serializers.clear();
	}

	void AssetLoader::Serialize(const Asset* asset)
	{

	}

	Asset* AssetLoader::TryLoadAsset(const std::filesystem::path& path)
	{
		return nullptr;
	}

}
