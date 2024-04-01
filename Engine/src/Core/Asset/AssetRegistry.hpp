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

		// If asset didn't exist previously creates a new entry
		Asset** operator[](AssetHandle handle);

		u64 Count() const { return m_AssetRegistry.size(); }
		bool Contains(AssetHandle handle) const { return m_AssetRegistry.find(handle) != m_AssetRegistry.end(); }

		auto begin() { return m_AssetRegistry.begin(); }
		auto end() { return m_AssetRegistry.end(); }
		auto cbegin() const { return m_AssetRegistry.cbegin(); }
		auto cend() const { return m_AssetRegistry.cend(); }

	private:
		std::unordered_map<AssetHandle, Asset*> m_AssetRegistry; // contains all loaded assets

		std::map<AssetHandle, AssetMetaData> m_AvailableAssets;

	private:
		void FetchAvailableAssets();
	};

}
