#include "EditorAssetManager.hpp"

namespace SW {
	
    const Asset* EditorAssetManager::GetAsset(AssetHandle handle)
    {
		return nullptr;
    }

	Asset* EditorAssetManager::GetAssetRaw(AssetHandle handle)
    {
        return nullptr;
    }

    bool EditorAssetManager::ForceUnload(AssetHandle handle)
    {
		return true;
    }

    bool EditorAssetManager::ForceReload(AssetHandle handle)
    {
		return true;
    }

}
