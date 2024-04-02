#include "EditorAssetManager.hpp"

#include "AssetLoader.hpp"

namespace SW {
	
	EditorAssetManager::~EditorAssetManager()
	{
		for (std::pair<AssetHandle, Asset*> pair : m_Registry) {
			delete pair.second;
		}
	}

	Asset** EditorAssetManager::GetAssetRaw(AssetHandle handle)
	{
		Asset** element = &m_Registry[handle];

		if (*element != nullptr)
			return element;

		const AssetMetaData& metadata = GetAssetMetaData(handle);

		Asset* newAsset = AssetLoader::TryLoadAsset(metadata);
		newAsset->m_Handle = handle;

		m_Registry[handle] = newAsset;

		return &m_Registry[handle];
	}

	const Asset** EditorAssetManager::GetAsset(AssetHandle handle)
	{
		return const_cast<const Asset**>(GetAssetRaw(handle));
	}

	bool EditorAssetManager::ForceUnload(AssetHandle handle)
    {
		//ASSERT(m_Registry.Contains(handle), "Can not unload asset: {}", handle);

		return true;
    }

    bool EditorAssetManager::ForceReload(AssetHandle handle)
    {
		if (!Contains(handle))
			return false;

		const AssetMetaData& metadata = GetAssetMetaData(handle);

		Asset* oldAsset = m_Registry[handle];

		Asset* reloadedAsset = AssetLoader::TryLoadAsset(metadata);
		reloadedAsset->m_Handle = handle;

		m_Registry[handle] = reloadedAsset;

		delete oldAsset;

		return true;
    }

}
