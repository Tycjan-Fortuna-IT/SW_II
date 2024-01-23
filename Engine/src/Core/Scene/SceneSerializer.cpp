#include "SceneSerializer.hpp"
#include "Scene.hpp"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace SW {


	void SceneSerializer::Serialize(Scene* const scene, const std::string& path)
	{
		YAML::Emitter output;

		output << YAML::BeginMap;
		output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		auto it = scene->GetRegistry().GetRegistryHandle().storage<Entity>().each();

		for (auto&& [handle, entity] : it) {
			if (!entity) {
				SW_WARN("SceneSerializer - NULL entity, skipped.");
			}

			output << YAML::BeginMap;

			output << YAML::Key << "Entity";

			output << YAML::Key << "IDComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "ID" << YAML::Value << entity.GetID();
			output << YAML::EndMap;

			output << YAML::Key << "TagComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Tag" << YAML::Value << entity.GetTag();
			output << YAML::EndMap;

			output << YAML::EndMap;
		}

		output << YAML::EndSeq;

		std::ofstream fout(path);
		fout << output.c_str();
	}

	void SceneSerializer::Deserialize(Scene** scene, const std::string& path)
	{

	}

}
