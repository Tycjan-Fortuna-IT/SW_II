#pragma once

#include "AssetRegistry.hpp"

namespace SW {

	class AssetSerializer;

	class AssetLoader
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void Serialize(const Asset* asset, const AssetMetaData& metadata);
		[[nodiscard]] static Asset* TryLoadAsset(const AssetMetaData& metadata);

	private:
		static std::unordered_map<AssetType, AssetSerializer*> s_Serializers;
	};

}
