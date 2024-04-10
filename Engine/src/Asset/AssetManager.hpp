/**
 * @file AssetManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "AssetRegistry.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"

namespace SW {

	class AssetManager
	{
	public:
		/**
		 * @brief Initialize the asset manager.
		 * @warning Must be called before any other asset manager functions.
		 */
		static void Initialize();

		/**
		 * @brief Shutdown the asset manager.
		 * @warning Must be called before the application closes.
		 */
		static void Shutdown();

		/**
		 * @brief Get the asset metadata.
		 * @warning The asset must be available. (means registered in the asset registry)
		 * 
		 * @param handle The handle of the asset.
		 * @return const AssetMetaData& The metadata of the asset.
		 */
		static const AssetMetaData& GetAssetMetaData(AssetHandle handle) { return GetRegistry().GetAssetMetaData(handle); }

		/**
		 * @brief Get the asset metadata.
		 * @warning If the asset is not available, the nullptr will be returned.
		 * 
		 * @param handle The handle of the asset.
		 * @return const AssetMetaData& The metadata of the asset.
		 */
		static Asset** GetAssetRaw(AssetHandle handle)
		{
			return ProjectContext::Get()->GetAssetManager()->GetAssetRaw(handle);
		}

		/**
		 * @brief Get the asset metadata.
		 * @warning If the asset is not available, the nullptr will be returned.
		 * 
		 * @param handle The handle of the asset.
		 * @return const AssetMetaData& The metadata of the asset.
		 */
		template <typename T>
		static T** GetAssetRaw(AssetHandle handle)
		{
			return (T**)ProjectContext::Get()->GetAssetManager()->GetAssetRaw(handle);
		}

		/**
		 * @brief Get the asset metadata.
		 * @warning If the asset is not available, the nullptr will be returned.
		 * 
		 * @param handle The handle of the asset.
		 * @return const AssetMetaData& The metadata of the asset.
		 */
		static const Asset** GetAsset(AssetHandle handle)
		{
			return ProjectContext::Get()->GetAssetManager()->GetAsset(handle);
		}

		/**
		 * @brief Get the asset metadata.
		 * @warning If the asset is not available, the nullptr will be returned.
		 * 
		 * @param handle The handle of the asset.
		 * @return const AssetMetaData& The metadata of the asset.
		 */
		template <typename T>
		static const T** GetAsset(AssetHandle handle)
		{
			return (const T**)ProjectContext::Get()->GetAssetManager()->GetAsset(handle);
		}

		/**
		 * @brief Check if the asset is valid.
		 * 
		 * @param handle The handle of the asset.
		 * @returns If the asset is valid.
		 */
		static bool IsValid(AssetHandle handle)
		{
			return ProjectContext::Get()->GetAssetManager()->IsValid(handle);
		}
		
		/**
		 * @brief Force unload the asset.
		 * @param handle The handle of the asset.
		 */
		static bool ForceUnload(AssetHandle handle)
		{
			return ProjectContext::Get()->GetAssetManager()->ForceUnload(handle);
		}

		/**
		 * @brief Force reload the asset.
		 * @param handle The handle of the asset.
		 */
		static bool ForceReload(AssetHandle handle)
		{
			return ProjectContext::Get()->GetAssetManager()->ForceReload(handle);
		}

		/**
		 * @brief Get the asset registry.
		 * @returns The asset registry.
		 */
		static const AssetRegistry& GetRegistry() { return ProjectContext::Get()->GetAssetManager()->GetRegistry(); }
		
		/**
		 * @brief Get the asset registry.
		 * @returns The asset registry.
		 */
		static AssetRegistry& GetRegistryRaw() { return ProjectContext::Get()->GetAssetManager()->GetRegistryRaw(); }

	};

}
