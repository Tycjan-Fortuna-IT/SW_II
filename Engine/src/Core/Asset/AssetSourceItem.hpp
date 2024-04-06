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
		bool IsParentOfDescendant(const AssetSourceItem* descendant) const;
	};

}
