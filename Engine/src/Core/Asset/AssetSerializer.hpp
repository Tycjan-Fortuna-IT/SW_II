#pragma once

#include "AssetRegistry.hpp"

namespace SW {

	class Asset;

	class AssetSerializer
	{
	public:
		virtual void Serialize(const Asset* asset) = 0;
	
		virtual Asset* TryLoadAsset(const AssetMetaData& metadata) = 0;
	};

	class Texture2DSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const Asset* asset) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class SpritesheetSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const Asset* asset) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

}
