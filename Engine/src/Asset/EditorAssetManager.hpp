/**
 * @file EditorAssetManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "AssetManagerBase.hpp"

namespace SW {
	
	class EditorAssetManager final : public AssetManagerBase
	{
	public:
		~EditorAssetManager() override;

		/**
		 * @brief Get the asset.
		 * @warning If the asset is not available, the placeholder will be returned.
		 * 
		 * @param handle The handle of the asset.
		 * @return Asset** The asset.
		 */
		Asset** GetAssetRaw(AssetHandle handle) override;
		
		/**
		 * @brief Get the asset.
		 * @warning If the asset is not available, the placeholder will be returned.
		 * 
		 * @param handle The handle of the asset.
		 * @return const Asset* The asset.
		 */
		const Asset** GetAsset(AssetHandle handle) override;

		/**
		 * @brief Force unload the asset.
		 * @param handle The handle of the asset.
		 */
		bool ForceUnload(AssetHandle handle) override;
		
		/**
		 * @brief Force reload the asset.
		 * @param handle The handle of the asset.
		 */
		bool ForceReload(AssetHandle handle) override;

		/**
		 * @brief Count the assets.
		 * 
		 * @return u64 The number of assets.
		 */
		u64 CountAssets() const { return m_Registry.size(); }
		
		/**
		 * @brief Check if contains the asset.
		 * 
		 * @param handle The handle of the asset.
		 * @return If the asset is contained.
		 */
		bool ContainsAsset(AssetHandle handle) const { return m_Registry.find(handle) != m_Registry.end(); }

	private:
		std::unordered_map<AssetHandle, Asset*> m_Registry; // contains all loaded assets
		//std::unordered_map<AssetHandle, Asset*> m_Cache; // contains all cached memory-only assets
	};

}
