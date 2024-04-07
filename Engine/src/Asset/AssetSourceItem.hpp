/**
 * @file AssetSourceItem.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset.hpp"
#include "GUI/Icons.hpp"
#include "Core/OpenGL/Texture2D.hpp"

namespace SW {

	using Timestamp = u64;

	struct AssetSourceItem
	{
		AssetHandle Handle = 0;
		AssetType Type = AssetType::Unknown;

		Timestamp ModificationTime = 0;
		Thumbnail Thumbnail;

		const char* Icon = SW_ICON_FILE;
		u32 Color = IM_COL32(204, 133, 0, 255);

		std::filesystem::path Path = ""; // relative to project's asset directory

		AssetSourceItem* Parent = nullptr;
		std::vector<AssetSourceItem*> Children;

		bool IsDirectory() const { return Type == AssetType::Directory; }
		bool IsFile() const { return Type != AssetType::Directory; }

		// recursive
		/**
		 * @brief Check if the asset is a descendant of the specified ancestor.
		 * 
		 * @param descendant The descendant asset.
		 * @return If the asset is a descendant of the specified ancestor.
		 */
		bool IsParentOfDescendant(const AssetSourceItem* descendant) const;
	};

}
