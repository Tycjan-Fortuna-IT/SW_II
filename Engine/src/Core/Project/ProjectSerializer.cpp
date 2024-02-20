#include "ProjectSerializer.hpp"

#include <yaml-cpp/yaml.h>

#include "Project.hpp"

namespace SW {

	void ProjectSerializer::Serialize(Project* project, const std::string& path)
	{
		const ProjectConfig& config = project->GetConfig();

		YAML::Emitter out;
		{
			out << YAML::BeginMap;	// Root
			out << YAML::Key << "Project" << YAML::Value;
			{
				out << YAML::BeginMap;	// Project
				out << YAML::Key << "Name" << YAML::Value << config.Name.c_str();
				out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetsDirectory.string();
				out << YAML::EndMap;	// Project
			}
			out << YAML::EndMap;	// Root
		}

		std::ofstream fout(path);
		fout << out.c_str();
	}

	Project* ProjectSerializer::Deserialize(const std::string& path)
	{
		ProjectConfig deserialized;

		YAML::Node data = YAML::LoadFile(path);

		YAML::Node projectNode = data["Project"];

		deserialized.Name = projectNode["Name"].as<std::string>().c_str();
		deserialized.AssetsDirectory = projectNode["AssetDirectory"].as<std::string>();

		Project* newProject = new Project(deserialized);

		return newProject;
	}

}
