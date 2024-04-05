#include "Asset.hpp"

#include "GUI/Editor/EditorResources.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "GUI/Icons.hpp"

namespace SW {

	static std::unordered_map<std::string, AssetType> s_StringifiedSourceTypesMapped =
	{
		{ "Unknown",		AssetType::Unknown				},
		{ "Directory",		AssetType::Directory			},
		{ "Texture2D",		AssetType::Texture2D			},
		{ "Sprite",			AssetType::Sprite				},
		{ "Spritesheet",	AssetType::Spritesheet			},
		{ "Font",			AssetType::Font					},
		{ "FontSource",		AssetType::FontSource			},
		{ "Scene",			AssetType::Scene				},
		{ "Prefab",			AssetType::Prefab				},
		{ "Script",			AssetType::Script				},
		{ "Shader",			AssetType::Shader				},
	};

	static std::unordered_map<std::string, AssetType> s_AssetSourceTypes =
	{
		{ ".png",			AssetType::Texture2D		},
		{ ".jpg",			AssetType::Texture2D		},
		{ ".ttf",			AssetType::FontSource		},
		{ ".sw_font",		AssetType::Font				},
		{ ".sw_scene",		AssetType::Scene			},
		{ ".sw_prefab",		AssetType::Prefab			},
		{ ".cs",			AssetType::Script			},
		{ ".sw_sprite",		AssetType::Sprite			},
		{ ".sw_spritesh",	AssetType::Spritesheet		},
		{ ".glsl",			AssetType::Shader			},
	};

	static const std::unordered_map<AssetType, ImU32> s_AssetSourceTypeColors =
	{
		{ AssetType::Unknown,		IM_COL32(179, 167, 167, 255) },
		{ AssetType::Texture2D,		IM_COL32(229,  70,  70, 255) },
		{ AssetType::Directory,		IM_COL32(255, 171,   0, 255) },
		{ AssetType::Scene,			IM_COL32( 70, 103, 226, 255) },
		{ AssetType::Font,			IM_COL32(204, 255, 153, 255) },
		{ AssetType::FontSource,	IM_COL32( 88,  88,  88, 255) },
		{ AssetType::Prefab,		IM_COL32( 70, 199, 152, 255) },
		{ AssetType::Script,		IM_COL32(  0, 255, 17, 255)  },
		{ AssetType::Sprite,		IM_COL32(230, 255,   0, 255) },
		{ AssetType::Spritesheet,	IM_COL32(172, 101, 183, 255) },
		{ AssetType::Shader,		IM_COL32(201, 197, 202, 255) },
	};

	static const std::unordered_map <AssetType, const char*> s_AssetSourceTypesToIcon =
	{
		{ AssetType::Unknown,		SW_ICON_FILE			},
		{ AssetType::Texture2D,		SW_ICON_FILE_IMAGE		},
		{ AssetType::Directory,		SW_ICON_FOLDER			},
		{ AssetType::Scene,			SW_ICON_FILE_VIDEO		},
		{ AssetType::Font,			SW_ICON_FORMAT_TEXT		},
		{ AssetType::FontSource,	SW_ICON_FORMAT_TEXT		},
		{ AssetType::Prefab,		SW_ICON_FILE_IMPORT		},
		{ AssetType::Script,		SW_ICON_LANGUAGE_CSHARP },
		{ AssetType::Sprite,		SW_ICON_FILE_IMAGE		},
		{ AssetType::Spritesheet,	SW_ICON_FILE_IMAGE		},
		{ AssetType::Shader,		SW_ICON_LANGUAGE_C      },
	};

	const char* Asset::GetStringifiedAssetType(AssetType type)
	{
		switch (type) {
			case AssetType::Unknown:		return "Unknown";		break;
			case AssetType::Directory:		return "Directory";		break;
			case AssetType::Texture2D:		return "Texture2D";		break;
			case AssetType::Sprite:			return "Sprite";		break;
			case AssetType::Spritesheet:	return "Spritesheet";	break;
			case AssetType::Font:			return "Font";			break;
			case AssetType::FontSource:		return "FontSource";	break;
			case AssetType::Scene:			return "Scene";			break;
			case AssetType::Prefab:			return "Prefab";		break;
			case AssetType::Script:			return "Script";		break;
			case AssetType::Shader:			return "Shader";		break;
		}

		return "Unknown";
	}

	AssetType Asset::GetAssetTypeFromStringified(const std::string& type)
	{
		const auto it = s_StringifiedSourceTypesMapped.find(type);

		if (it != s_StringifiedSourceTypesMapped.end())
			return it->second;

		return AssetType::Unknown;
	}

	AssetType Asset::GetAssetTypeFromExtension(const std::string& extension)
	{
		const auto it = s_AssetSourceTypes.find(extension);

		if (it != s_AssetSourceTypes.end())
			return it->second;

		return AssetType::Unknown;
	}

	const char* Asset::GetIconFromAssetType(AssetType type)
	{
		const auto it = s_AssetSourceTypesToIcon.find(type);

		if (it != s_AssetSourceTypesToIcon.end())
			return it->second;

		return SW_ICON_FILE;
	}

	u32 Asset::GetColorFromAssetType(AssetType type)
	{
		const auto it = s_AssetSourceTypeColors.find(type);

		if (it != s_AssetSourceTypeColors.end())
			return it->second;

		return IM_COL32(204, 133, 0, 255);
	}

	Thumbnail Asset::GetThumbnailFromAssetType(AssetType type)
	{
		Texture2D** texture = nullptr;
		
		switch (type) {
			case AssetType::Unknown: {
				texture = &EditorResources::UnknownAssetIcon;
			} break;
			case AssetType::Directory: {
				texture = &EditorResources::DirectoryAssetIcon;
			} break;
			case AssetType::Texture2D: {
				// Thumbnail will be lazy loaded individually request based. (No need to load at the beginning).
			} break;	
			case AssetType::Sprite: {
				// Thumbnail will be lazy loaded individually request based. (No need to load at the beginning).
			} break;	// same
			case AssetType::Spritesheet: {
				// Thumbnail will be lazy loaded individually request based. (No need to load at the beginning).
			} break;	// same
			case AssetType::Font: {
				// Thumbnail will be lazy loaded individually request based. (No need to load at the beginning).
			} break;	// same
			case AssetType::FontSource: {
				texture = &EditorResources::FontAssetIcon;
			} break;
			case AssetType::Scene: {
				texture = &EditorResources::SceneAssetIcon;
			} break;
			case AssetType::Prefab: {
				texture = &EditorResources::PrefabAssetIcon;
			} break;
			case AssetType::Script: {
				texture = &EditorResources::ScriptAssetIcon;
			} break;
			case AssetType::Shader: {
				texture = &EditorResources::ShaderAssetIcon;
			} break;
		}

		Thumbnail thumbnail;
		thumbnail.Texture = texture ? texture : nullptr;
		thumbnail.Width = texture ? (f32)(*texture)->GetWidth() : 64.f;
		thumbnail.Height = texture ? (f32)(*texture)->GetHeight() : 64.f;

		return thumbnail;
	}

}
