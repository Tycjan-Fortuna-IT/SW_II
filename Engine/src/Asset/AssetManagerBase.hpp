/**
 * @file AssetManagerBase.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "AssetRegistry.hpp"
#include "Core/OpenGL/Texture2D.hpp"

namespace SW {
	
	class AssetManagerBase
	{
	public:
		AssetManagerBase();
		virtual ~AssetManagerBase();

		/**
		 * @brief Get the asset.
		 * @warning If the asset is not available, the placeholder will be returned.
		 * 
		 * @param handle The handle of the asset.
		 * @return Asset** The asset.
		 */
		virtual Asset** GetAssetRaw(AssetHandle handle) = 0;
		
		/**
		 * @brief Get the asset.
		 * @warning If the asset is not available, the placeholder will be returned.
		 * 
		 * @param handle The handle of the asset.
		 * @return const Asset* The asset.
		 */
		virtual const Asset** GetAsset(AssetHandle handle) = 0;

		/**
		 * @brief Force unload the asset.
		 * @param handle The handle of the asset.
		 */
		virtual bool ForceUnload(AssetHandle handle) = 0;
		
		/**
		 * @brief Force reload the asset.
		 * @param handle The handle of the asset.
		 */
		virtual bool ForceReload(AssetHandle handle) = 0;

		const AssetRegistry& GetRegistry() { return m_Registry; }
		AssetRegistry& GetRegistryRaw() { return m_Registry; }

		/**
		 * @brief Check if the asset is valid.
		 * 
		 * @param handle The handle of the asset.
		 * @return If the asset is not valid.
		 */
		bool IsValid(AssetHandle handle) const { return m_Registry.Contains(handle); }

		/**
		 * @brief Get the asset metadata.
		 * @warning The asset must be available. (means registered in the asset registry)
		 * 
		 * @param handle The handle of the asset.
		 * @return const AssetMetaData& The metadata of the asset.
		 */
		const AssetMetaData& GetAssetMetaData(AssetHandle handle) { return m_Registry.GetAssetMetaData(handle); }

	protected:
		AssetRegistry m_Registry; // All asset pointers are owned by the AssetRegistry!
	};

}
