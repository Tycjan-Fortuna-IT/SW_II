#include "SceneSerializer.hpp"
#include "Scene.hpp"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Core/Math/Vector3.hpp"
#include "Core/Math/Vector4.hpp"

namespace YAML {

	Emitter& operator<<(Emitter& out, const SW::Vector3<f32>& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << v.z << EndSeq;
		return out;
	}

	Emitter& operator<<(Emitter& out, const SW::Vector4<f32>& v) {
		out << Flow;
		out << BeginSeq << v.r << v.g << v.b << v.a << EndSeq;
		return out;
	}

}

namespace SW {

	void SceneSerializer::Serialize(Scene* scene, const std::string& path)
	{
		YAML::Emitter output;

		output << YAML::BeginMap;
		output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto&& [handle, idc] : EntityRegistry::GetEntitiesWith<IDComponent>().each()) {
			Entity entity = { handle };

			SW_DEBUG("Serialized scene entity with id: {}", idc.ID);

			if (!entity) {
				SW_WARN("SceneSerializer - NULL entity, skipped.");
			}

			output << YAML::BeginMap; // Start Entity content

			output << YAML::Key << "Entity";

			output << YAML::BeginMap; // Start Entity components content

			output << YAML::Key << "IDComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "ID" << YAML::Value << entity.GetID();
			output << YAML::EndMap;

			output << YAML::Key << "TagComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Tag" << YAML::Value << entity.GetTag();
			output << YAML::EndMap;

			TransformComponent& tc = entity.GetComponent<TransformComponent>();
			output << YAML::Key << "TransformComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Transform" << YAML::Value << tc.Position;
			output << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			output << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			output << YAML::EndMap;

			if (entity.HasComponent<SpriteComponent>()) {
				SpriteComponent& sc = entity.GetComponent<SpriteComponent>();
				output << YAML::Key << "SpriteComponent";
				output << YAML::BeginMap;
				output << YAML::Key << "Color" << YAML::Value << sc.Color;
				output << YAML::EndMap;
			}

			output << YAML::EndMap; // End Entity components content

			output << YAML::EndMap; // End Entity content
		}

		output << YAML::EndSeq;

		std::ofstream fout(path);
		fout << output.c_str();
	}

	void SceneSerializer::Deserialize(Scene** scene, const std::string& path)
	{

	}

}
