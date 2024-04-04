#pragma once

#include "AssetRegistry.hpp"
#include "Core/OpenGL/Texture2D.hpp"

namespace SW {
	
	class AssetManagerBase
	{
	public:
		AssetManagerBase();
		virtual ~AssetManagerBase();

		virtual Asset** GetAssetRaw(AssetHandle handle) = 0;
		virtual const Asset** GetAsset(AssetHandle handle) = 0;

		virtual bool ForceUnload(AssetHandle handle) = 0;
		virtual bool ForceReload(AssetHandle handle) = 0;

		const AssetRegistry& GetRegistry() { return m_Registry; }
		AssetRegistry& GetRegistryRaw() { return m_Registry; }

		bool IsAvailable(AssetHandle handle) const { return m_Registry.Contains(handle); }
		const AssetMetaData& GetAssetMetaData(AssetHandle handle) { return m_Registry.GetAssetMetaData(handle); }

	protected:
		AssetRegistry m_Registry; // All asset pointers are owned by the AssetRegistry!
	};

}
