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
		Texture2D,
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

		bool IsDirectory() const { return Type == AssetSourceType::Directory; }
		bool IsFile() const { return Type != AssetSourceType::Directory; }

		static const char* GetStringifiedAssetSourceType(AssetSourceType type);
		static AssetSourceType GetAssetSourceTypeFromStringified(const std::string& type);
		static AssetSourceType GetTypeFromExtension(const std::string& extension);
		static const char* GetIconFromFileType(AssetSourceType type);
		static u32 GetColorFromFileType(AssetSourceType type);
		static Texture2D* GetThumbnailFromFileType(AssetSourceType type);
	};

}
