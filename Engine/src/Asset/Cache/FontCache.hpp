/**
 * @file FontCache.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	class Texture2D;

	/**
	 * @brief The FontCache class provides functionality for caching font atlases.
	 */
	class FontCache
	{
	public:
		using Timestamp   = u64;
		using AssetHandle = u64;

		/**
		 * @brief Tries to retrieve a cached atlas texture based on the provided handle and last modified timestamp.
		 * @warning The caller is responsible for managing its lifetime. Must be deleted when no longer needed.
		 *
		 * @param handle The handle of the font atlas.
		 * @param lastModified The last modified timestamp of the font atlas.
		 * @return A pointer to the cached atlas texture if it exists, nullptr otherwise.
		 */
		static Texture2D* TryGetCachedAtlas(AssetHandle handle, Timestamp lastModified);

		/**
		 * @brief Caches the provided atlas texture with the given handle and last modified timestamp.
		 * @note Cache is stored in the project's asset directory / cache / fonts.
		 * 		 The cache file is named as follows: [handle]_[lastModified].cache
		 * 		 The cache file contains the width, height, channels, and pixel data of the atlas texture.
		 * 		 The cache file is in binary format.
		 *
		 * @param atlas The atlas texture to be cached.
		 * @param handle The handle of the font atlas.
		 * @param lastModified The last modified timestamp of the font atlas.
		 */
		static void CacheAtlas(const Texture2D* atlas, AssetHandle handle, Timestamp lastModified);
	};

} // namespace SW
