/**
 * @file AssetLoader.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "AssetRegistry.hpp"

namespace SW
{

	class AssetSerializer;

	class AssetLoader
	{
	public:
		/**
		 * @brief Initialize the asset loader.
		 * 		  Initializes the asset loader by creating serializers for each supported asset type.
		 */
		static void Initialize();

		/**
		 * @brief Shutdown the asset loader.
		 * 		  Shuts down the asset loader by deleting all serializers.
		 */
		static void Shutdown();

		/**
		 * @brief Serialize the asset.
		 *
		 * @param metadata The metadata to serialize.
		 */
		static void Serialize(const AssetMetaData& metadata);

		/**
		 * @brief Try to load the asset.
		 *
		 * @param metadata The metadata of the asset.
		 * @return Asset* The loaded asset if successful, placeholder otherwise.
		 */
		[[nodiscard]] static Asset* TryLoadAsset(const AssetMetaData& metadata);

	private:
		static std::unordered_map<AssetType, Scope<AssetSerializer>> s_Serializers;
	};

} // namespace SW
