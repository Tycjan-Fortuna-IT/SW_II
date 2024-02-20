/**
 * @file AssetManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	class Texture2D;

	/**
	 * @brief Asset manager system.
	 */
	class AssetManager final
	{
	public:
		/**
		 * @brief Initializes asset manager system.
		 * @warning Must be called before any other asset manager system operation.
		 */
		static void Initialize();

		/**
		 * @brief Shuts down asset manager system.
		 * @warning Must be called before application exit.
		 */
		static void Shutdown();

		/**
		 * @brief Returns white texture.
		 * @return White texture.
		 */
		static Texture2D* GetWhiteTexture();

		/**
		 * @brief Returns black texture.
		 * @return Black texture.
		 */
		static Texture2D* GetBlackTexture();

		/**
		 * @brief Returns texture from given path. If texture is already loaded, returns it from cache.
		 *  	  Otherwise, loads texture from file and caches it.
		 * @param path Path to texture.
		 * @param flipped Whether to flip texture vertically.
		 * @return Texture from given path.
		 */
		static Texture2D* GetTexture2D(const char* path, bool flipped = true);

		/**
		 * @brief Returns texture from given path. If texture is already loaded, returns it from cache.
		 *  	  Otherwise, loads texture from file and caches it.
		 * @param path Path to texture.
		 * @param flipped Whether to flip texture vertically.
		 * @warning Editor assets only!
		 * 
		 * @return Texture from given path.
		 */
		static Texture2D* GetEditorTexture2D(const char* path);
	};

}
