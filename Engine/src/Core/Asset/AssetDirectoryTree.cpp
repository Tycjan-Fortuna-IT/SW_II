#include "AssetDirectoryTree.hpp"

#include "Core/Asset/AssetManager.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "GUI/Editor/EditorResources.hpp"

namespace SW {

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

	AssetSourceItem* AssetDirectoryTree::FindChildItemByPath(AssetSourceItem* parent, const std::filesystem::path& dir) const
	{
		if (parent->Path == dir) return parent;

		for (AssetSourceItem* child : parent->Children) {
			AssetSourceItem* result = FindChildItemByPath(child, dir);

			if (result != nullptr) return result;
		}

		return nullptr;
	}

	void AssetDirectoryTree::RefetchChanges(const std::filesystem::path& dir)
	{
		AssetManager::GetRegistryRaw().RefetchAvailableAssets();

		TraverseDirectoryAndMapAssets(dir);
	}

	void AssetDirectoryTree::TraverseDirectoryAndMapAssets(const std::filesystem::path& dir)
	{
		const AssetRegistry& reg = AssetManager::GetRegistry();
		const std::map<AssetHandle, AssetMetaData>& availableAssets = reg.GetAvailableAssets();

		std::unordered_map<std::filesystem::path, AssetHandle> pathToIdMap;
		for (auto&& [handle, metadata] : availableAssets) {
			pathToIdMap[metadata.Path] = handle;
		}

		if (m_Root)
			CleanUp(m_Root);

		m_Root = new AssetSourceItem();
		//m_Root->Handle = pathToIdMap.find(dir)->second; // no need for handle for not-draggable item
		m_Root->Type = AssetSourceType::Directory;
		m_Root->Thumbnail = EditorResources::DirectoryAssetIcon;
		m_Root->Icon = SW_ICON_FILE;
		m_Root->Color = IM_COL32(204, 204, 178, 255);
		m_Root->Path = dir;

		TraverseAndEmplace(pathToIdMap, m_Root->Path, m_Root);
	}

	void AssetDirectoryTree::TraverseAndEmplace(
		const std::unordered_map<std::filesystem::path, AssetHandle>& pathToIdMap,
		const std::filesystem::path& dir, AssetSourceItem* item
	) {
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dir)) {
			AssetSourceItem* newItem = new AssetSourceItem();

			const std::filesystem::path itemPath = std::filesystem::relative(entry.path(), ProjectContext::Get()->GetAssetDirectory());;

			newItem->Handle = pathToIdMap.at(itemPath);
			newItem->Type = std::filesystem::is_directory(entry) ? AssetSourceType::Directory : AssetSourceItem::GetTypeFromExtension(itemPath.extension().string());
			newItem->Thumbnail = AssetSourceItem::GetThumbnailFromFileType(newItem->Type);
			newItem->Icon = AssetSourceItem::GetIconFromFileType(newItem->Type);
			newItem->Color = AssetSourceItem::GetColorFromFileType(newItem->Type);
			newItem->Parent = item;
			newItem->Path = itemPath;
			newItem->ModificationTime = FileSystem::GetLastWriteTime(entry.path());

			item->Children.emplace_back(newItem);

			if (newItem->Type == AssetSourceType::Directory)
				TraverseAndEmplace(pathToIdMap, entry.path(), newItem);
		}
	}

}
