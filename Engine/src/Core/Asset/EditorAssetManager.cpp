#include "EditorAssetManager.hpp"

#include "AssetLoader.hpp"

namespace SW {
	
    const Asset* EditorAssetManager::GetAsset(AssetHandle handle)
    {
		return GetAssetRaw(handle);
    }

	Asset* EditorAssetManager::GetAssetRaw(AssetHandle handle)
    {
		Asset* element = *m_Registry[handle];

		if (element != nullptr)
			return element;

		const AssetMetaData& metadata = GetAssetMetaData(handle);

		Asset* newAsset = AssetLoader::TryLoadAsset(metadata);
		newAsset->m_Handle = handle;

		*m_Registry[handle] = newAsset;

        return newAsset;
    }

    bool EditorAssetManager::ForceUnload(AssetHandle handle)
    {
		//ASSERT(m_Registry.Contains(handle), "Can not unload asset: {}", handle);

		return true;
    }

    bool EditorAssetManager::ForceReload(AssetHandle handle)
    {
		//ASSERT(m_Registry.Contains(handle), "Can not reload asset: {}", handle);

		return true;
    }

}
