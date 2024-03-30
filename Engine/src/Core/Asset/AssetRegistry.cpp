#include "AssetRegistry.hpp"

#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"

namespace SW {

	AssetRegistry::AssetRegistry()
	{
		FetchAvailableAssets();
	}

	AssetRegistry::~AssetRegistry()
	{
		for (std::pair<AssetHandle, Asset*> pair : m_AssetRegistry) {
			delete pair.second;
		}
	}

	void AssetRegistry::FetchAvailableAssets()
	{
		const std::filesystem::path& assetsDir = ProjectContext::Get()->GetAssetDirectory();

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(assetsDir)) {

		}
	}

	Asset* AssetRegistry::operator[](AssetHandle handle)
	{
		return m_AssetRegistry[handle];
	}

}
