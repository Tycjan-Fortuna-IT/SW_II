/**
 * @file ThumbnailCache.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset/Asset.hpp"

namespace SW
{

	class Texture2DAsset;
	struct AssetSourceItem;

	struct ThumbnailCacheData
	{
		Texture2DAsset* Texture; // The actual thumbnail texture.
		u64 LastModified;        // The last modified timestamp of the asset.
	};

	class ThumbnailCache
	{
	public:
		ThumbnailCache() = default;
		~ThumbnailCache();

		using Timestamp = u64;

		/**
		 * @brief Retrieves the thumbnail texture of the asset with the provided handle.
		 * @warning Returned texture pointer is managed by the cache. Do not delete it.
		 *
		 * @param itemPath The path of the asset.
		 * @param handle The handle of the asset.
		 * @param lastModified The last modified timestamp of the asset.
		 * @return A pointer to the thumbnail texture if it exists, nullptr otherwise.
		 */
		Texture2DAsset** GetTextureThumbnail(const std::filesystem::path& itemPath, AssetHandle handle,
		                                     Timestamp lastModified);

		/**
		 * @brief Retrieves the thumbnail texture of the font atlas with the provided handle.
		 * @warning Returned texture pointer is managed by the cache. Do not delete it.
		 *
		 * @param itemPath The path of the font atlas.
		 * @param handle The handle of the font atlas.
		 * @param lastModified The last modified timestamp of the font atlas.
		 * @return A pointer to the thumbnail texture if it exists, nullptr otherwise.
		 */
		Texture2DAsset** GetFontAtlasThumbnail(const std::filesystem::path& itemPath, AssetHandle handle,
		                                       Timestamp lastModified);

		/**
		 * @brief Clear the cache of all thumbnails.
		 * @note Both in memory and on disk.
		 */
		void Clear();

	private:
		std::unordered_map<AssetHandle, ThumbnailCacheData> m_Thumbnails; // The cache of thumbnails.

	private:
		/**
		 * @brief Downscale the provided texture to a smaller size.
		 *
		 * @param texture The texture to downscale.
		 */
		void DownscaleTexture(Texture2DAsset* texture);

		/**
		 * @brief Load and cache the thumbnail texture of the asset with the provided handle.
		 *
		 * @param itemPath The path of the asset.
		 * @param cachePath The path of the cache directory.
		 * @param handle The handle of the asset.
		 * @param lastModified The last modified timestamp of the asset.
		 * @return A pointer to the cached thumbnail texture.
		 */
		Texture2DAsset** LoadAndCacheTexture(const std::filesystem::path& itemPath,
		                                     const std::filesystem::path& cachePath, AssetHandle handle,
		                                     Timestamp lastModified);

		/**
		 * @brief Load and cache the thumbnail texture of the font atlas with the provided handle.
		 *
		 * @param itemPath The path of the font source file.
		 * @param cachePath The path of the cache directory.
		 * @param handle The handle of the font atlas.
		 * @param lastModified The last modified timestamp of the font atlas.
		 * @return Texture2DAsset**
		 */
		Texture2DAsset** LoadAndCacheFontAtlas(const std::filesystem::path& itemPath,
		                                       const std::filesystem::path& cachePath, AssetHandle handle,
		                                       Timestamp lastModified);
	};

} // namespace SW
