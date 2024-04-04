#pragma once

#include "AssetRegistry.hpp"

namespace SW {

	class Asset;

	// When adding new serializer class remember to register it in the AssetLoader

	class AssetSerializer
	{
	public:
		virtual void Serialize(const AssetMetaData& metadata) = 0;
	
		virtual Asset* TryLoadAsset(const AssetMetaData& metadata) = 0;
	};

	class Texture2DSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class SpriteSerializer final : public AssetSerializer {
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class SpritesheetSerializer final : public AssetSerializer {
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

}
