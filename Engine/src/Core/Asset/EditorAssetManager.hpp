#pragma once

#include "AssetManagerBase.hpp"

namespace SW {
	
	class EditorAssetManager final : public AssetManagerBase
	{
	public:
		~EditorAssetManager() override;

		// ------- Registry -------
		Asset** GetAssetRaw(AssetHandle handle) override;
		const Asset** GetAsset(AssetHandle handle) override;

		bool ForceUnload(AssetHandle handle) override;
		bool ForceReload(AssetHandle handle) override;

		u64 CountAssets() const { return m_Registry.size(); }
		bool ContainsAsset(AssetHandle handle) const { return m_Registry.find(handle) != m_Registry.end(); }

	private:
		std::unordered_map<AssetHandle, Asset*> m_Registry; // contains all loaded assets
		//std::unordered_map<AssetHandle, Asset*> m_Cache; // contains all cached memory-only assets
	};

}
