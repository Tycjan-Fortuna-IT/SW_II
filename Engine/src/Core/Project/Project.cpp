#include "Project.hpp"

#include "Core/Utils/FileSystem.hpp"

namespace SW {

	Project::Project(const ProjectConfig& config)
		: m_Config(config) {}

	Project::~Project()
	{
		delete m_AssetManager;
	}

	void Project::Initialize()
	{
		FileSystem::CreateEmptyDirectoryIfNotExists(m_Config.AssetsDirectory / "cache");
		FileSystem::CreateEmptyDirectoryIfNotExists(m_Config.AssetsDirectory / "cache" / "thumbnails");
		FileSystem::CreateEmptyDirectoryIfNotExists(m_Config.AssetsDirectory / "cache" / "fonts");

		m_AssetManager = new EditorAssetManager();
	}

}
