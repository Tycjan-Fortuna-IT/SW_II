#include "AssetLoader.hpp"

#include "AssetSerializer.hpp"

namespace SW {

	std::unordered_map<AssetType, AssetSerializer*> AssetLoader::s_Serializers;

	void AssetLoader::Initialize()
	{
		s_Serializers[AssetType::Texture2D] = new Texture2DSerializer();
		s_Serializers[AssetType::Sprite] = new SpriteSerializer();
		s_Serializers[AssetType::Spritesheet] = new SpritesheetSerializer();
		s_Serializers[AssetType::Font] = new FontSerializer();
		s_Serializers[AssetType::FontSource] = new FontSourceSerializer();
	}

	void AssetLoader::Shutdown()
	{
		delete s_Serializers[AssetType::Texture2D];
		delete s_Serializers[AssetType::Sprite];
		delete s_Serializers[AssetType::Spritesheet];
		delete s_Serializers[AssetType::Font];
		delete s_Serializers[AssetType::FontSource];

		s_Serializers.clear();
	}

	void AssetLoader::Serialize(const AssetMetaData& metadata)
	{
		ASSERT(s_Serializers.find(metadata.Type) != s_Serializers.end(), "Asset serializer not available for this file: {} !", metadata.Path.string());

		s_Serializers.at(metadata.Type)->Serialize(metadata);
	}

	Asset* AssetLoader::TryLoadAsset(const AssetMetaData& metadata)
	{
		ASSERT(s_Serializers.find(metadata.Type) != s_Serializers.end(), "Asset serializer not available for this file: {} !", metadata.Path.string());

		return s_Serializers.at(metadata.Type)->TryLoadAsset(metadata);
	}

}
