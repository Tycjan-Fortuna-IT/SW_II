#include "AssetSourceItem.hpp"

#include "GUI/Editor/EditorResources.hpp"

namespace SW {

	static std::unordered_map<std::string, AssetSourceType> s_StringifiedSourceTypesMapped =
	{
		{ "Unknown",		AssetSourceType::Unknown			},
		{ "Directory",		AssetSourceType::Directory			},
		{ "Texture2D",		AssetSourceType::Texture2D			},
		{ "Sprite",			AssetSourceType::Sprite				},
		{ "Spritesheet",	AssetSourceType::Spritesheet		},
		{ "Font",			AssetSourceType::Font				},
		{ "Scene",			AssetSourceType::Scene				},
		{ "Prefab",			AssetSourceType::Prefab				},
		{ "Script",			AssetSourceType::Script				},
		{ "Shader",			AssetSourceType::Shader				},
	};

	static std::unordered_map<std::string, AssetSourceType> s_AssetSourceTypes =
	{
		{ ".png",			AssetSourceType::Texture2D		},
		{ ".jpg",			AssetSourceType::Texture2D		},
		{ ".ttf",			AssetSourceType::Font			},
		{ ".sw_scene",		AssetSourceType::Scene			},
		{ ".sw_prefab",		AssetSourceType::Prefab			},
		{ ".cs",			AssetSourceType::Script			},
		{ ".sw_sprite",		AssetSourceType::Sprite			},
		{ ".sw_spritesh",	AssetSourceType::Spritesheet	},
		{ ".glsl",			AssetSourceType::Shader			},
	};

	static const std::unordered_map<AssetSourceType, ImU32> s_AssetSourceTypeColors =
	{
		{ AssetSourceType::Unknown,		IM_COL32(204, 133,   0, 255) },
		{ AssetSourceType::Texture2D,	IM_COL32(204, 102, 102, 255) },
		{ AssetSourceType::Directory,	IM_COL32(204, 204, 178, 255) },
		{ AssetSourceType::Scene,		IM_COL32(122, 163, 204, 255) },
		{ AssetSourceType::Font,		IM_COL32(135, 135, 135, 255) },
		{ AssetSourceType::Prefab,		IM_COL32(184, 102, 184, 255) },
		{ AssetSourceType::Script,		IM_COL32(115, 189, 115, 255) },
		{ AssetSourceType::Sprite,		IM_COL32(115, 189, 189, 255) },
		{ AssetSourceType::Spritesheet, IM_COL32(189, 115, 115, 255) },
		{ AssetSourceType::Shader,		IM_COL32(189, 189, 115, 255) },
	};

	static const std::unordered_map <AssetSourceType, const char*> s_AssetSourceTypesToIcon =
	{
		{ AssetSourceType::Unknown,		SW_ICON_FILE			},
		{ AssetSourceType::Texture2D,	SW_ICON_FILE_IMAGE		},
		{ AssetSourceType::Directory,	SW_ICON_FOLDER			},
		{ AssetSourceType::Scene,		SW_ICON_FILE_VIDEO		},
		{ AssetSourceType::Font,		SW_ICON_FORMAT_TEXT		},
		{ AssetSourceType::Prefab,		SW_ICON_FILE_IMPORT		},
		{ AssetSourceType::Script,		SW_ICON_LANGUAGE_CSHARP },
		{ AssetSourceType::Sprite,		SW_ICON_FILE_IMAGE		},
		{ AssetSourceType::Spritesheet, SW_ICON_FILE_IMAGE		},
		{ AssetSourceType::Shader,      SW_ICON_LANGUAGE_C      },
	};

	const char* AssetSourceItem::GetStringifiedAssetSourceType(AssetSourceType type)
	{
		switch (type) {
			case AssetSourceType::Unknown:		return "Unknown";		break;
			case AssetSourceType::Directory:	return "Directory";		break;
			case AssetSourceType::Texture2D:	return "Texture2D";		break;
			case AssetSourceType::Sprite:		return "Sprite";		break;
			case AssetSourceType::Spritesheet:  return "Spritesheet";	break;
			case AssetSourceType::Font:			return "Font";			break;
			case AssetSourceType::Scene:		return "Scene";			break;
			case AssetSourceType::Prefab:		return "Prefab";		break;
			case AssetSourceType::Script:		return "Script";		break;
			case AssetSourceType::Shader:		return "Shader";		break;
		}

		return "Unknown";
	}

	AssetSourceType AssetSourceItem::GetAssetSourceTypeFromStringified(const std::string& type)
	{
		const auto it = s_StringifiedSourceTypesMapped.find(type);

		if (it != s_StringifiedSourceTypesMapped.end())
			return it->second;

		return AssetSourceType::Unknown;
	}

	AssetSourceType AssetSourceItem::GetTypeFromExtension(const std::string& extension)
	{
		const auto it = s_AssetSourceTypes.find(extension);

		if (it != s_AssetSourceTypes.end())
			return it->second;

		return AssetSourceType::Unknown;
	}

	const char* AssetSourceItem::GetIconFromFileType(AssetSourceType type)
	{
		const auto it = s_AssetSourceTypesToIcon.find(type);

		if (it != s_AssetSourceTypesToIcon.end())
			return it->second;

		return SW_ICON_FILE;
	}

	u32 AssetSourceItem::GetColorFromFileType(AssetSourceType type)
	{
		const auto it = s_AssetSourceTypeColors.find(type);

		if (it != s_AssetSourceTypeColors.end())
			return it->second;

		return IM_COL32(204, 133, 0, 255);
	}

	Texture2D* AssetSourceItem::GetThumbnailFromFileType(AssetSourceType type)
	{
		switch (type) {
			case AssetSourceType::Unknown:
				return EditorResources::UnknownAssetIcon; break;
			case AssetSourceType::Directory:
				return EditorResources::DirectoryAssetIcon; break;
			case AssetSourceType::Texture2D:
				return nullptr; break;		// Thumbnail will be loaded individually request based. (Dont need to load all at the beginning).
			case AssetSourceType::Sprite:
				return nullptr; break;	// same
			case AssetSourceType::Spritesheet:
				return nullptr; break;		// same
			case AssetSourceType::Font:
				return EditorResources::FontAssetIcon; break;
			case AssetSourceType::Scene:
				return EditorResources::SceneAssetIcon; break;
			case AssetSourceType::Prefab:
				return EditorResources::PrefabAssetIcon; break;
			case AssetSourceType::Script:
				return EditorResources::ScriptAssetIcon; break;
			case AssetSourceType::Shader:
				return EditorResources::ShaderAssetIcon; break;
		}

		return nullptr;
	}

}
