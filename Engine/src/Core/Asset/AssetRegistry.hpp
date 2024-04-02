#pragma once

#include "Core/OpenGL/Texture2D.hpp"
#include "Asset.hpp"
#include "AssetSourceItem.hpp"

namespace SW {
	
	using Timestamp = u64;

	struct AssetMetaData
	{
		AssetHandle Handle;
		AssetType Type;
		std::filesystem::path Path;
		Timestamp ModificationTime;
	};
	
	class AssetRegistry
	{
	public:
		AssetRegistry();		
		~AssetRegistry();

		void FetchDirectory(std::map<std::filesystem::path, AssetMetaData>& registered, const std::filesystem::path& dir, bool reload);
		void RefetchAvailableAssets();

		const AssetMetaData& GetAssetMetaData(AssetHandle handle) const;
		const std::map<AssetHandle, AssetMetaData>& GetAvailableAssets() const { return m_AvailableAssets; }

	private:
		std::map<AssetHandle, AssetMetaData> m_AvailableAssets;

	private:
		void FetchAvailableAssets();
	};

}
