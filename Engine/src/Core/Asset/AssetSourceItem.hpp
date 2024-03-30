#pragma once

#include "Asset.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "GUI/Icons.hpp"

namespace SW {

	using Timestamp = u64;

	enum class AssetSourceType : u8
	{
		Unknown = 0,
		Directory,
		Texture,
		Sprite,
		Spritesheet,
		Font,
		Scene,
		Prefab,
		Script,
		Shader,
	};

	struct AssetSourceItem
	{
		AssetHandle Handle = 0;
		AssetSourceType Type = AssetSourceType::Unknown;

		Timestamp ModificationTime = 0;
		Texture2D* Thumbnail = nullptr;

		const char* Icon = SW_ICON_FILE;
		u32 Color = IM_COL32(204, 133, 0, 255);

		std::filesystem::path Path = ""; // relative to project's asset directory

		AssetSourceItem* Parent = nullptr;
		std::vector<AssetSourceItem*> Children;

		const char* GetStringifiedAssetSourceType() const;

		bool IsDirectory() const { return Type == AssetSourceType::Directory; }

		bool IsFile() const { return Type != AssetSourceType::Directory; }
	};

}
