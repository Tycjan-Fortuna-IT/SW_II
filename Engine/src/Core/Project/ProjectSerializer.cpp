#include "ProjectSerializer.hpp"

#include <yaml-cpp/yaml.h>

#include "Core/Utils/SerializationUtils.hpp"
#include "Project.hpp"

namespace SW
{

	void ProjectSerializer::Serialize(Project* project, const std::string& path)
	{
		const ProjectConfig& config = project->GetConfig();

		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "Project" << YAML::Value;
			{
				out << YAML::BeginMap; // Project
				out << YAML::Key << "Name" << YAML::Value << config.Name.c_str();
				out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetsDirectory.string();
				out << YAML::Key << "AssetRegistryPath" << YAML::Value << config.AssetRegistryPath.string();
				out << YAML::Key << "AudioRegistryPath" << YAML::Value << config.AudioRegistryPath.string();
				out << YAML::EndMap; // Project
			}
			out << YAML::EndMap; // Root
		}

		std::ofstream fout(path);
		fout << out.c_str();
	}

	Project* ProjectSerializer::Deserialize(const std::string& path)
	{
		YAML::Node data = YAML::LoadFile(path);

		YAML::Node projectNode = data["Project"];

		ProjectConfig deserialized;
		deserialized.Name            = TryDeserializeNode<std::string>(projectNode, "Name", "Unnamed");
		deserialized.AssetsDirectory = TryDeserializeNode<std::string>(projectNode, "AssetDirectory", "assets");
		deserialized.AssetRegistryPath =
		    TryDeserializeNode<std::string>(projectNode, "AssetRegistryPath", "assets/assets.sw_registry");
		deserialized.AudioRegistryPath =
		    TryDeserializeNode<std::string>(projectNode, "AudioRegistryPath", "assets/audio.sw_registry");

		Project* newProject = new Project(deserialized);

		return newProject;
	}

} // namespace SW
