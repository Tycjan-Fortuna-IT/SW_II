#pragma once

#include "AssetRegistry.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"

namespace SW {

	class AssetManager
	{
	public:
		static void Initialize();
		static void Shutdown();

		static const AssetMetaData& GetAssetMetaData(AssetHandle handle) { return GetRegistry().GetAssetMetaData(handle); }

		static Asset** GetAssetRaw(AssetHandle handle)
		{
			return ProjectContext::Get()->GetAssetManager()->GetAssetRaw(handle);
		}

		template <typename T>
		static T** GetAssetRaw(AssetHandle handle)
		{
			return (T**)ProjectContext::Get()->GetAssetManager()->GetAssetRaw(handle);
		}

		static const Asset** GetAsset(AssetHandle handle)
		{
			return ProjectContext::Get()->GetAssetManager()->GetAsset(handle);
		}

		template <typename T>
		static const T** GetAsset(AssetHandle handle)
		{
			return (const T**)ProjectContext::Get()->GetAssetManager()->GetAsset(handle);
		}

		static bool ForceUnload(AssetHandle handle)
		{
			return ProjectContext::Get()->GetAssetManager()->ForceUnload(handle);
		}

		static bool ForceReload(AssetHandle handle)
		{
			return ProjectContext::Get()->GetAssetManager()->ForceReload(handle);
		}

		static const AssetRegistry& GetRegistry() { return ProjectContext::Get()->GetAssetManager()->GetRegistry(); }
		static AssetRegistry& GetRegistryRaw() { return ProjectContext::Get()->GetAssetManager()->GetRegistryRaw(); }

	};

}
