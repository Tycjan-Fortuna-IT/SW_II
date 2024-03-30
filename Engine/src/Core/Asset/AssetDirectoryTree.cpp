#include "AssetDirectoryTree.hpp"

#include "Core/Utils/Random.hpp"
#include "Core/Asset/AssetManager.hpp"
#include "GUI/Icons.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "GUI/Editor/EditorResources.hpp"

namespace SW {

	static std::unordered_map<std::string, AssetSourceType> s_AssetSourceTypes =
	{
		{ ".png",			AssetSourceType::Texture		},
		{ ".jpg",			AssetSourceType::Texture		},
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
		{ AssetSourceType::Texture,		IM_COL32(204, 102, 102, 255) },
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
		{ AssetSourceType::Texture,		SW_ICON_FILE_IMAGE		},
		{ AssetSourceType::Directory,	SW_ICON_FOLDER			},	
		{ AssetSourceType::Scene,		SW_ICON_FILE_VIDEO		},
		{ AssetSourceType::Font,		SW_ICON_FORMAT_TEXT		},
		{ AssetSourceType::Prefab,		SW_ICON_FILE_IMPORT		},
		{ AssetSourceType::Script,		SW_ICON_LANGUAGE_CSHARP },
		{ AssetSourceType::Sprite,		SW_ICON_FILE_IMAGE		},
		{ AssetSourceType::Spritesheet, SW_ICON_FILE_IMAGE		},
		{ AssetSourceType::Shader,      SW_ICON_LANGUAGE_C      },
	};

	AssetSourceType AssetDirectoryTree::GetTypeFromExtension(const std::filesystem::path& path)
	{
		const auto it = s_AssetSourceTypes.find(path.extension().string());

		if (it != s_AssetSourceTypes.end())
			return it->second;

		return AssetSourceType::Unknown;
	}

	const char* AssetDirectoryTree::GetIconFromFileType(AssetSourceType type)
	{
		const auto it = s_AssetSourceTypesToIcon.find(type);

		if (it != s_AssetSourceTypesToIcon.end())
			return it->second;

		return SW_ICON_FILE;
	}

	u32 AssetDirectoryTree::GetColorFromFileType(AssetSourceType type)
	{
		const auto it = s_AssetSourceTypeColors.find(type);

		if (it != s_AssetSourceTypeColors.end())
			return it->second;

		return IM_COL32(204, 133, 0, 255);
	}

	Texture2D* AssetDirectoryTree::GetThumbnailFromFileType(AssetSourceType type)
	{
		switch (type) {
			case AssetSourceType::Unknown:	   
				return EditorResources::UnknownAssetIcon; break;
			case AssetSourceType::Directory:   
				return EditorResources::DirectoryAssetIcon; break;
			case AssetSourceType::Texture:	   
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

	AssetDirectoryTree::~AssetDirectoryTree()
	{
		CleanUp(m_Root);
	}

	void AssetDirectoryTree::CleanUp(AssetSourceItem* item)
	{
		for (AssetSourceItem* child : item->Children) {
			CleanUp(child);
		}

		delete item;
	}

	void AssetDirectoryTree::RefetchChanges()
	{
		AssetManager::GetRegistryRaw().FetchAvailableAssets();
	}

	void AssetDirectoryTree::TraverseDirectoryAndMapAssets(const std::filesystem::path& dir)
	{
		if (m_Root)
			CleanUp(m_Root);

		m_Root = new AssetSourceItem();
		m_Root->Handle = Random::CreateID();
		m_Root->Type = AssetSourceType::Directory;
		m_Root->Thumbnail = EditorResources::DirectoryAssetIcon;
		m_Root->Icon = SW_ICON_FILE;
		m_Root->Color = IM_COL32(204, 204, 178, 255);
		m_Root->Path = dir;

		TraverseAndEmplace(m_Root->Path, m_Root);
	}

	void AssetDirectoryTree::TraverseAndEmplace(const std::filesystem::path& dir, AssetSourceItem* item)
	{
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dir)) {
			AssetSourceItem* newItem = new AssetSourceItem();

			newItem->Handle = Random::CreateID();
			newItem->Type = std::filesystem::is_directory(entry) ? AssetSourceType::Directory : GetTypeFromExtension(entry.path());
			newItem->Thumbnail = GetThumbnailFromFileType(newItem->Type);
			newItem->Icon = GetIconFromFileType(newItem->Type);
			newItem->Color = GetColorFromFileType(newItem->Type);
			newItem->Parent = item;
			newItem->Path = entry.path();
			newItem->ModificationTime = FileSystem::GetLastWriteTime(entry.path());

			item->Children.emplace_back(newItem);

			if (newItem->Type == AssetSourceType::Directory)
				TraverseAndEmplace(newItem->Path, newItem);
		}
	}

}
