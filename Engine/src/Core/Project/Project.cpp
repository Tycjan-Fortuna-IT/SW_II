#include "Project.hpp"

#include "Core/Asset/EditorAssetManager.hpp"

namespace SW {

	Project::Project(const ProjectConfig& config)
		: m_Config(config) {}

	Project::~Project()
	{
		delete m_AssetManager;
	}

	void Project::Initialize()
	{
		m_AssetManager = new EditorAssetManager();
	}

}
