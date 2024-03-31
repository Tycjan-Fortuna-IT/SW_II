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

		void TraverseDirectoryAndMapAssets(const std::filesystem::path& dir);

		AssetSourceItem* GetRootItem() const { return m_Root; }
		// Might return nulltpr
		AssetSourceItem* FindChildItemByPath(AssetSourceItem* parent, const std::filesystem::path& dir) const;

		void RefetchChanges(const std::filesystem::path& dir);

	private:
		AssetSourceItem* m_Root = nullptr;

	private:
		void CleanUp(AssetSourceItem* item);

		void TraverseAndEmplace(const std::unordered_map<std::filesystem::path, AssetHandle>& pathToIdMap,
			const std::filesystem::path& dir, AssetSourceItem* item);
	};

}
