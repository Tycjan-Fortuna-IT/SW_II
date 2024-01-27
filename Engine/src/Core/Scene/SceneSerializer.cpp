#include "SceneSerializer.hpp"
#include "Scene.hpp"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Core/Math/Vector3.hpp"
#include "Core/Math/Vector4.hpp"
#include "Core/AssetManager.hpp"

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

	template<>
	struct convert<SW::Vector3<f32>> {
		static Node encode(const SW::Vector3<f32>& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, SW::Vector3<f32>& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<SW::Vector4<f32>> {
		static Node encode(const SW::Vector4<f32>& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, SW::Vector4<f32>& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

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

				if (sc.Texture) {
					output << YAML::Key << "TexturePath" << YAML::Value << sc.Texture->GetPath();
				}

				output << YAML::EndMap;
			}

			output << YAML::EndMap; // End Entity components content

			output << YAML::EndMap; // End Entity content
		}

		output << YAML::EndSeq;

		std::ofstream fout(path);
		fout << output.c_str();
	}

	void SceneSerializer::Deserialize(Scene* scene, const std::string& path)
	{
		YAML::Node data = YAML::LoadFile(path);

		if (!data["Entities"]) {
			SW_ERROR("Error while deserializing the scene: {}, no entities section found!", path);
			return;
		}

		scene->DestroyAllEntities();

		YAML::Node entities = data["Entities"];

		for (auto entity : entities) {
			YAML::Node idComponent = entity["Entity"]["IDComponent"];

			u64 id = idComponent["ID"].as<u64>();

			SW_DEBUG("Serialized scene entity with id: {}", id);

			YAML::Node tagComponent = entity["Entity"]["TagComponent"];
			std::string tag = tagComponent["Tag"].as<std::string>();

			Entity deserialized = scene->CreateEntityWithID(id, tag);

			auto transformComponent = entity["Entity"]["TransformComponent"];
			if (transformComponent) {
				TransformComponent& tc = deserialized.GetComponent<TransformComponent>();

				tc.Position = transformComponent["Transform"].as<Vector3<f32>>();
				tc.Rotation = transformComponent["Rotation"].as<Vector3<f32>>();
				tc.Scale = transformComponent["Scale"].as<Vector3<f32>>();
			}

			auto spriteComponent = entity["Entity"]["SpriteComponent"];
			if (spriteComponent) {
				auto& sprite = deserialized.AddComponent<SpriteComponent>();

				sprite.Color = spriteComponent["Color"].as<Vector4<f32>>();

				if (spriteComponent["TexturePath"]) {
					std::string path = spriteComponent["TexturePath"].as<std::string>();

					sprite.Texture = AssetManager::GetTexture2D(path.c_str());
				}
			}
		}
	}

}
