/**
 * @file AssetDirectoryTree.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "AssetSourceItem.hpp"

namespace SW {

	class AssetDirectoryTree
	{
	public:
		AssetDirectoryTree() = default;
		~AssetDirectoryTree();

		AssetDirectoryTree(const AssetDirectoryTree& other) = delete;
		AssetDirectoryTree(AssetDirectoryTree&& other) = delete;
		AssetDirectoryTree& operator=(const AssetDirectoryTree& other) = delete;
		AssetDirectoryTree& operator=(AssetDirectoryTree&& other) = delete;

		/**
		 * @brief Traverse the asset directory and map the assets.
		 * 		  Scans the directory and maps the assets to the tree.
		 */
		void TraverseDirectoryAndMapAssets();

		/**
		 * @brief Get the root item of the tree.
		 * 
		 * @return AssetSourceItem* The root item.
		 */
		AssetSourceItem* GetRootItem() const { return m_Root; }
		
		/**
		 * @brief Find a child item by path.
		 * @warning Might return nullptr.
		 * 
		 * @param parent The parent item.
		 * @param dir The path of the child item.
		 * @return AssetSourceItem* The child item if found, nullptr otherwise.
		 */
		AssetSourceItem* FindChildItemByPath(AssetSourceItem* parent, const std::filesystem::path& dir) const;

		/**
		 * @brief Refetch the changes in the directory.
		 * 		  Refetches the changes in the directory and updates the tree.
		 * 
		 * @param dir The directory to refetch.
		 */
		void RefetchChanges();

	private:
		AssetSourceItem* m_Root = nullptr;

	private:
		/**
		 * @brief Recursively clean up the item and its children.
		 * 
		 * @param item The item to clean up.
		 */
		void CleanUp(AssetSourceItem* item);

		/**
		 * @brief Traverse the directory and emplace the assets.
		 * 		  Scans the directory and emplaces the assets to the tree.
		 * 
		 * @param pathToIdMap The map of path to asset handle.
		 * @param dir The directory to traverse.
		 * @param item The item to emplace the assets.
		 */
		void TraverseAndEmplace(const std::unordered_map<std::filesystem::path, AssetHandle>& pathToIdMap,
			const std::filesystem::path& dir, AssetSourceItem* item);
	};

}
