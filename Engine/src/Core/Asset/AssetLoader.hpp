#pragma once

#include "Asset.hpp"

namespace SW {

	class AssetSerializer;

	class AssetLoader
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void Serialize(const Asset* asset);
		static Asset* TryLoadAsset(const std::filesystem::path& path);

	private:
		static std::unordered_map<AssetType, AssetSerializer*> s_Serializers;
	};

}
