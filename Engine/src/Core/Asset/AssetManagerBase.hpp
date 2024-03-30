#pragma once

#include "AssetRegistry.hpp"
#include "Core/OpenGL/Texture2D.hpp"

namespace SW {
	
	class AssetManagerBase
	{
	public:
		AssetManagerBase();
		virtual ~AssetManagerBase();

		virtual const Asset* GetAsset(AssetHandle handle) = 0;
		virtual Asset* GetAssetRaw(AssetHandle handle) = 0;

		const AssetRegistry& GetRegistry() { return m_Registry; }
		AssetRegistry& GetRegistryRaw() { return m_Registry; }

	private:
		AssetRegistry m_Registry; // All asset pointers are owned by the AssetRegistry!
	};

}
