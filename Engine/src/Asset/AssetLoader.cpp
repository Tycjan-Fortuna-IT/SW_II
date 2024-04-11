#include "AssetLoader.hpp"

#include "AssetSerializer.hpp"

namespace SW {

	std::unordered_map<AssetType, Scope<AssetSerializer>> AssetLoader::s_Serializers;

	void AssetLoader::Initialize()
	{
		s_Serializers[AssetType::Texture2D] = CreateScope<Texture2DSerializer>();
		s_Serializers[AssetType::Sprite] = CreateScope<SpriteSerializer>();
		s_Serializers[AssetType::Spritesheet] = CreateScope<SpritesheetSerializer>();
		s_Serializers[AssetType::Font] = CreateScope<FontSerializer>();
		s_Serializers[AssetType::FontSource] = CreateScope<FontSourceSerializer>();
		s_Serializers[AssetType::Animation2D] = CreateScope<AnimationSerializer>();
	}

	void AssetLoader::Shutdown()
	{
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
