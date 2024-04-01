#include "AssetSourceItem.hpp"

#include "GUI/Editor/EditorResources.hpp"

namespace SW {

	static std::unordered_map<std::string, AssetType> s_StringifiedSourceTypesMapped =
	{
		{ "Unknown",		AssetType::Unknown				},
		{ "Directory",		AssetType::Directory			},
		{ "Texture2D",		AssetType::Texture2D			},
		{ "Sprite",			AssetType::Sprite				},
		{ "Spritesheet",	AssetType::Spritesheet			},
		{ "Font",			AssetType::Font					},
		{ "Scene",			AssetType::Scene				},
		{ "Prefab",			AssetType::Prefab				},
		{ "Script",			AssetType::Script				},
		{ "Shader",			AssetType::Shader				},
	};

	static std::unordered_map<std::string, AssetType> s_AssetSourceTypes =
	{
		{ ".png",			AssetType::Texture2D		},
		{ ".jpg",			AssetType::Texture2D		},
		{ ".ttf",			AssetType::Font				},
		{ ".sw_scene",		AssetType::Scene			},
		{ ".sw_prefab",		AssetType::Prefab			},
		{ ".cs",			AssetType::Script			},
		{ ".sw_sprite",		AssetType::Sprite			},
		{ ".sw_spritesh",	AssetType::Spritesheet		},
		{ ".glsl",			AssetType::Shader			},
	};

	static const std::unordered_map<AssetType, ImU32> s_AssetSourceTypeColors =
	{
		{ AssetType::Unknown,		IM_COL32(204, 133,   0, 255) },
		{ AssetType::Texture2D,		IM_COL32(204, 102, 102, 255) },
		{ AssetType::Directory,		IM_COL32(204, 204, 178, 255) },
		{ AssetType::Scene,			IM_COL32(122, 163, 204, 255) },
		{ AssetType::Font,			IM_COL32(135, 135, 135, 255) },
		{ AssetType::Prefab,		IM_COL32(184, 102, 184, 255) },
		{ AssetType::Script,		IM_COL32(115, 189, 115, 255) },
		{ AssetType::Sprite,		IM_COL32(115, 189, 189, 255) },
		{ AssetType::Spritesheet,	IM_COL32(189, 115, 115, 255) },
		{ AssetType::Shader,		IM_COL32(189, 189, 115, 255) },
	};

	static const std::unordered_map <AssetType, const char*> s_AssetSourceTypesToIcon =
	{
		{ AssetType::Unknown,		SW_ICON_FILE			},
		{ AssetType::Texture2D,		SW_ICON_FILE_IMAGE		},
		{ AssetType::Directory,		SW_ICON_FOLDER			},
		{ AssetType::Scene,			SW_ICON_FILE_VIDEO		},
		{ AssetType::Font,			SW_ICON_FORMAT_TEXT		},
		{ AssetType::Prefab,		SW_ICON_FILE_IMPORT		},
		{ AssetType::Script,		SW_ICON_LANGUAGE_CSHARP },
		{ AssetType::Sprite,		SW_ICON_FILE_IMAGE		},
		{ AssetType::Spritesheet,	SW_ICON_FILE_IMAGE		},
		{ AssetType::Shader,		SW_ICON_LANGUAGE_C      },
	};

	const char* AssetSourceItem::GetStringifiedAssetSourceType(AssetType type)
	{
		switch (type) {
			case AssetType::Unknown:		return "Unknown";		break;
			case AssetType::Directory:		return "Directory";		break;
			case AssetType::Texture2D:		return "Texture2D";		break;
			case AssetType::Sprite:			return "Sprite";		break;
			case AssetType::Spritesheet:	return "Spritesheet";	break;
			case AssetType::Font:			return "Font";			break;
			case AssetType::Scene:			return "Scene";			break;
			case AssetType::Prefab:			return "Prefab";		break;
			case AssetType::Script:			return "Script";		break;
			case AssetType::Shader:			return "Shader";		break;
		}

		return "Unknown";
	}

	AssetType AssetSourceItem::GetAssetSourceTypeFromStringified(const std::string& type)
	{
		const auto it = s_StringifiedSourceTypesMapped.find(type);

		if (it != s_StringifiedSourceTypesMapped.end())
			return it->second;

		return AssetType::Unknown;
	}

	AssetType AssetSourceItem::GetTypeFromExtension(const std::string& extension)
	{
		const auto it = s_AssetSourceTypes.find(extension);

		if (it != s_AssetSourceTypes.end())
			return it->second;

		return AssetType::Unknown;
	}

	const char* AssetSourceItem::GetIconFromFileType(AssetType type)
	{
		const auto it = s_AssetSourceTypesToIcon.find(type);

		if (it != s_AssetSourceTypesToIcon.end())
			return it->second;

		return SW_ICON_FILE;
	}

	u32 AssetSourceItem::GetColorFromFileType(AssetType type)
	{
		const auto it = s_AssetSourceTypeColors.find(type);

		if (it != s_AssetSourceTypeColors.end())
			return it->second;

		return IM_COL32(204, 133, 0, 255);
	}

	Texture2D* AssetSourceItem::GetThumbnailFromFileType(AssetType type)
	{
		switch (type) {
			case AssetType::Unknown:
				return EditorResources::UnknownAssetIcon; break;
			case AssetType::Directory:
				return EditorResources::DirectoryAssetIcon; break;
			case AssetType::Texture2D:
				return nullptr; break;	// Thumbnail will be loaded individually request based. (Dont need to load all at the beginning).
			case AssetType::Sprite:
				return nullptr; break;	// same
			case AssetType::Spritesheet:
				return nullptr; break;	// same
			case AssetType::Font:
				return EditorResources::FontAssetIcon; break;
			case AssetType::Scene:
				return EditorResources::SceneAssetIcon; break;
			case AssetType::Prefab:
				return EditorResources::PrefabAssetIcon; break;
			case AssetType::Script:
				return EditorResources::ScriptAssetIcon; break;
			case AssetType::Shader:
				return EditorResources::ShaderAssetIcon; break;
		}

		return nullptr;
	}

}
