#include "AssetRegistry.hpp"

#include <yaml-cpp/yaml.h>

#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Utils/Random.hpp"
#include "AssetManager.hpp"
#include "Core/Utils/FileSystem.hpp"

namespace SW {

	AssetRegistry::AssetRegistry()
	{
		const std::filesystem::path regPath = ProjectContext::Get()->GetAssetDirectory() / "assets.sw_registry";

		if (!FileSystem::Exists(regPath))
			FileSystem::CreateFileWithContent(regPath, "Assets:\n");

		FetchAvailableAssets();
	}

	AssetRegistry::~AssetRegistry()
	{
		for (std::pair<AssetHandle, Asset*> pair : m_AssetRegistry) {
			delete pair.second;
		}

		YAML::Emitter output;

		output << YAML::BeginMap;
		output << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq;

		for (auto&& [handle, metadata] : m_AvailableAssets) {
			output << YAML::BeginMap;

			output << YAML::Key << "Handle" << YAML::Value << handle;
			output << YAML::Key << "Path" << YAML::Value << metadata.Path.string();
			output << YAML::Key << "ModificationTime" << YAML::Value << metadata.ModificationTime;

			output << YAML::EndMap;
		}

		output << YAML::EndMap;

		output << YAML::EndSeq;

		std::ofstream fout(ProjectContext::Get()->GetAssetDirectory() / "assets.sw_registry");
		fout << output.c_str();
	}

	void AssetRegistry::FetchDirectory(std::map<std::filesystem::path, AssetMetaData>& registered, const std::filesystem::path& dir, bool reload)
	{
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dir)) {
			const std::filesystem::path& rel = std::filesystem::relative(entry.path(), ProjectContext::Get()->GetAssetDirectory());

			auto it = registered.find(rel);

			if (it != registered.end()) {
				if (reload && it->second.ModificationTime != FileSystem::GetLastWriteTime(entry)) {
					AssetManager::ForceReload(it->second.Handle);

					it->second.ModificationTime = FileSystem::GetLastWriteTime(entry);

					SW_INFO("Asset {} [{}] was reloaded!", it->second.Path.string(), it->second.Handle);
				}

				m_AvailableAssets[it->second.Handle] = it->second;

				registered.erase(it);
			} else {
				AssetMetaData metadata;
				metadata.Handle = Random::CreateID();
				metadata.Path = rel;
				metadata.ModificationTime = FileSystem::GetLastWriteTime(entry);

				m_AvailableAssets[metadata.Handle] = metadata;
			}

			if (entry.is_directory())
				FetchDirectory(registered, entry.path(), reload);
		}
	}

	void AssetRegistry::RefetchAvailableAssets()
	{
		std::map<std::filesystem::path, AssetMetaData> registeredEntries;
		for (auto&& [handle, metadata] : m_AvailableAssets) {
			registeredEntries[metadata.Path] = metadata;
		}

		m_AvailableAssets.clear(); // not clearing - looking also for file changes for hot reload of assets

		FetchDirectory(registeredEntries, ProjectContext::Get()->GetAssetDirectory(), true); // erases found registered entries

		for (auto&& [path, metadata] : registeredEntries) { // Not found after reload -> candidates for unloading
			bool result = AssetManager::ForceUnload(metadata.Handle);

			ASSERT(result, "Could not unload asset {} [{}]", path.string(), metadata.Handle);
			
			SW_INFO("Asset {} [{}] was unloaded!", path.string(), metadata.Handle);
		}
	}

    const SW::AssetMetaData& AssetRegistry::GetAssetMetaData(AssetHandle handle) const
    {
		ASSERT(m_AvailableAssets.find(handle) != m_AvailableAssets.end(), "Asset metadata for ID: {} does not exist!", handle);

		return m_AvailableAssets.at(handle);
    }

    void AssetRegistry::FetchAvailableAssets()
	{
		const std::filesystem::path& assetsDir = ProjectContext::Get()->GetAssetDirectory();
		const std::string registryPath = (assetsDir / "assets.sw_registry").string();

		YAML::Node registry = YAML::LoadFile(registryPath);

		YAML::Node assets = registry["Assets"];

		std::map<std::filesystem::path, AssetMetaData> registeredEntries;

		for (const YAML::Node& asset : assets) {
			AssetMetaData metadata;
			metadata.Handle = asset["Handle"].as<AssetHandle>();
			metadata.Path = asset["Path"].as<std::string>();
			metadata.ModificationTime = asset["ModificationTime"].as<u64>();

			registeredEntries[metadata.Path] = metadata;
		}

		m_AvailableAssets.clear();
		FetchDirectory(registeredEntries, assetsDir, false);
	}

	Asset* AssetRegistry::operator[](AssetHandle handle)
	{
		return m_AssetRegistry[handle];
	}

}
