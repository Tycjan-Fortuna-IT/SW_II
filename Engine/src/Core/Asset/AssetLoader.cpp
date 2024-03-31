#include "AssetLoader.hpp"

#include "AssetSerializer.hpp"

namespace SW {

	std::unordered_map<AssetSourceType, AssetSerializer*> AssetLoader::s_Serializers;

	void AssetLoader::Initialize()
	{
		s_Serializers[AssetSourceType::Texture2D] = new Texture2DSerializer();
		s_Serializers[AssetSourceType::Spritesheet] = new SpritesheetSerializer();
	}

	void AssetLoader::Shutdown()
	{
		delete s_Serializers[AssetSourceType::Texture2D];
		delete s_Serializers[AssetSourceType::Spritesheet];

		s_Serializers.clear();
	}

	void AssetLoader::Serialize(const Asset* asset, const AssetMetaData& metadata)
	{
		ASSERT(s_Serializers.find(metadata.Type) != s_Serializers.end(), "Asset serializer not available for this file: {} !", metadata.Path.string());

		s_Serializers.at(metadata.Type)->Serialize(asset);
	}

	Asset* AssetLoader::TryLoadAsset(const AssetMetaData& metadata)
	{
		ASSERT(s_Serializers.find(metadata.Type) != s_Serializers.end(), "Asset serializer not available for this file: {} !", metadata.Path.string());

		return s_Serializers.at(metadata.Type)->TryLoadAsset(metadata);
	}

}
