/**
 * @file EditorAssetManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "AssetLoader.hpp"
#include "AssetManagerBase.hpp"
#include "Core/Utils/Random.hpp"

namespace SW
{

	class EditorAssetManager final : public AssetManagerBase
	{
	public:
		~EditorAssetManager() override;

		/**
		 * @brief Creates a new instance of an asset of type T and returns a pointer to it.
		 * 		  The asset is created using the provided path and optional arguments.
		 * @warning Path is expected to be valid!
		 *
		 * @tparam T The type of asset to create. Must be derived from Asset.
		 * @tparam Args The types of the optional arguments.
		 * @param path The path to the asset.
		 * @param args The optional arguments to pass to the asset constructor.
		 * @return A pointer to the newly created asset.
		 */
		template <typename T, typename... Args>
		    requires std::is_base_of_v<Asset, T> && std::is_constructible_v<T, Args...>
		T** CreateNew(const std::filesystem::path& path, Args&&... args)
		{
			Asset* newAsset = new T(std::forward<Args...>(args...));

			std::map<AssetHandle, AssetMetaData>& avail = m_AvailRegistry.GetAvailableAssetsRaw();

			AssetMetaData metadata;
			metadata.Handle = Random::CreateID();
			metadata.Path   = path;
			metadata.Type   = T::GetStaticType();

			avail[metadata.Handle]      = metadata;
			m_Registry[metadata.Handle] = newAsset;

			newAsset->m_Handle = metadata.Handle;

			AssetLoader::Serialize(metadata);

			return (T**)&m_Registry[metadata.Handle];
		}

		/**
		 * @brief Get the asset.
		 * @warning If the asset is not available, the nullptr will be returned.
		 *
		 * @param handle The handle of the asset.
		 * @return Asset** The asset.
		 */
		Asset** GetAssetRaw(AssetHandle handle) override;

		/**
		 * @brief Get the asset.
		 * @warning If the asset is not available, the nullptr will be returned.
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

		/**
		 * @brief Get the registry (all loaded assets).
		 *
		 * @return const std::unordered_map<AssetHandle, Asset*> The registry.
		 */
		const std::unordered_map<AssetHandle, Asset*> GetLoadedAssets() const { return m_Registry; }

	private:
		std::unordered_map<AssetHandle, Asset*> m_Registry; // contains all loaded assets
		// std::unordered_map<AssetHandle, Asset*> m_Cache; // contains all cached memory-only assets
	};

} // namespace SW
