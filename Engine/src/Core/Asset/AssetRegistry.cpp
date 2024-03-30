#include "AssetRegistry.hpp"

#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"

namespace SW {

	void AssetRegistry::Initialize()
	{

	}

	void AssetRegistry::Shutdown()
	{
		for (std::pair<AssetHandle, Asset*> pair : m_AssetRegistry) {
			delete pair.second;
		}
	}

	Asset* AssetRegistry::operator[](AssetHandle handle)
	{
		return m_AssetRegistry[handle];
	}

}
