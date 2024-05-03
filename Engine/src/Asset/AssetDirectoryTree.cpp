#include "AssetDirectoryTree.hpp"

#include "Asset/AssetManager.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Utils/FileSystem.hpp"

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

	void AssetDirectoryTree::RefetchChanges()
	{
		AssetManager::GetRegistryRaw().RefetchAvailableAssets();

		TraverseDirectoryAndMapAssets();
	}

	void AssetDirectoryTree::TraverseDirectoryAndMapAssets()
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
		m_Root->Type = AssetType::Directory;
		m_Root->Thumbnail = Asset::GetThumbnailFromAssetType(AssetType::Directory);
		m_Root->Icon = SW_ICON_FILE;
		m_Root->Color = IM_COL32(204, 204, 178, 255);
		m_Root->Path = ".";

		TraverseAndEmplace(pathToIdMap, m_Root->Path, m_Root);
	}

	void AssetDirectoryTree::TraverseAndEmplace(
		const std::unordered_map<std::filesystem::path, AssetHandle>& pathToIdMap,
		const std::filesystem::path& dir, AssetSourceItem* item
	) {
		const std::filesystem::path fullPath = ProjectContext::Get()->GetAssetDirectory() / dir;

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(fullPath)) {
			AssetSourceItem* newItem = new AssetSourceItem();

			const std::filesystem::path itemPath = std::filesystem::relative(entry.path(), ProjectContext::Get()->GetAssetDirectory());

			AssetType type = std::filesystem::is_directory(entry) ? AssetType::Directory : Asset::GetAssetTypeFromExtension(itemPath.extension().string());

			if (
				type == AssetType::AssetRegistry ||
				(type == AssetType::Directory && 
					(itemPath.filename() == "build" || itemPath.filename() == "cache"))
			)
				continue;

			newItem->Handle = pathToIdMap.at(itemPath);
			newItem->Type = type;
			newItem->Thumbnail = Asset::GetThumbnailFromAssetType(newItem->Type);
			newItem->Icon = Asset::GetIconFromAssetType(newItem->Type);
			newItem->Color = Asset::GetColorFromAssetType(newItem->Type);
			newItem->Parent = item;
			newItem->Path = itemPath;
			newItem->ModificationTime = FileSystem::GetLastWriteTime(entry);

			item->Children.emplace_back(newItem);

			if (newItem->Type == AssetType::Directory)
				TraverseAndEmplace(pathToIdMap, entry.path(), newItem);
		}
	}

}
