#include "SceneSerializer.hpp"
#include "Scene.hpp"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Core/Math/Vector3.hpp"
#include "Core/Math/Vector4.hpp"
#include "Core/AssetManager.hpp"
#include "Core/ECS/Entity.hpp"

namespace YAML {

	Emitter& operator<<(Emitter& out, const glm::vec2& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << EndSeq;
		return out;
	}

	Emitter& operator<<(Emitter& out, const glm::vec3& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << v.z << EndSeq;
		return out;
	}

	Emitter& operator<<(Emitter& out, const glm::vec4& v) {
		out << Flow;
		out << BeginSeq << v.r << v.g << v.b << v.a << EndSeq;
		return out;
	}

	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<f32>();
			rhs.y = node[1].as<f32>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<f32>();
			rhs.y = node[1].as<f32>();
			rhs.z = node[2].as<f32>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<f32>();
			rhs.y = node[1].as<f32>();
			rhs.z = node[2].as<f32>();
			rhs.w = node[3].as<f32>();
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

		std::map<u64, Entity> sortedEntities;

		for (auto&& [handle, idc] : scene->GetRegistry().GetEntitiesWith<IDComponent>().each())
			sortedEntities[idc.ID] = Entity{ handle, scene };

		for (auto [id, entity] : sortedEntities) {
			SW_DEBUG("Serialized scene entity with id: {}", id);

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

			if (entity.HasComponent<CircleComponent>()) {
				CircleComponent& cc = entity.GetComponent<CircleComponent>();
				output << YAML::Key << "CircleComponent";
				output << YAML::BeginMap;
				output << YAML::Key << "Color" << YAML::Value << cc.Color;
				output << YAML::Key << "Thickness" << YAML::Value << cc.Thickness;
				output << YAML::Key << "Fade" << YAML::Value << cc.Fade;
				output << YAML::EndMap;
			}

			if (entity.HasComponent<CameraComponent>()) {
				CameraComponent& cc = entity.GetComponent<CameraComponent>();

				output << YAML::Key << "CameraComponent";
				output << YAML::BeginMap;
				output << YAML::Key << "Primary" << YAML::Value << cc.Primary;
				output << YAML::Key << "AspectRatio" << YAML::Value << cc.Camera.GetAspectRatio();
				output << YAML::EndMap;
			}

			if (entity.HasComponent<RigidBody2DComponent>()) {
				RigidBody2DComponent& rbc = entity.GetComponent<RigidBody2DComponent>();
				
				output << YAML::Key << "RigidBody2DComponent";
				output << YAML::BeginMap;
				output << YAML::Key << "Type" << YAML::Value << (int)rbc.Type;
				output << YAML::Key << "GravityScale" << YAML::Value << rbc.GravityScale;
				output << YAML::Key << "Friction" << YAML::Value << rbc.Friction;
				output << YAML::Key << "Restitution" << YAML::Value << rbc.Restitution;
				output << YAML::Key << "RestitutionThreshold" << YAML::Value << rbc.RestitutionThreshold;
				output << YAML::Key << "AllowSleep" << YAML::Value << rbc.AllowSleep;
				output << YAML::EndMap;
			}

			if (entity.HasComponent<BoxCollider2DComponent>()) {
				BoxCollider2DComponent& bcc = entity.GetComponent<BoxCollider2DComponent>();

				output << YAML::Key << "BoxCollider2DComponent";
				output << YAML::BeginMap;
				output << YAML::Key << "Size" << YAML::Value << bcc.Size;
				output << YAML::Key << "Offset" << YAML::Value << bcc.Offset;
				output << YAML::Key << "Density" << YAML::Value << bcc.Density;
				output << YAML::Key << "IsSensor" << YAML::Value << bcc.IsSensor;
				output << YAML::EndMap;
			}

			if (entity.HasComponent<CircleCollider2DComponent>()) {
				CircleCollider2DComponent& ccc = entity.GetComponent<CircleCollider2DComponent>();

				output << YAML::Key << "CircleCollider2DComponent";
				output << YAML::BeginMap;
				output << YAML::Key << "Radius" << YAML::Value << ccc.Radius;
				output << YAML::Key << "Offset" << YAML::Value << ccc.Offset;
				output << YAML::Key << "Density" << YAML::Value << ccc.Density;
				output << YAML::Key << "IsSensor" << YAML::Value << ccc.IsSensor;
				output << YAML::EndMap;
			}

			if (entity.HasComponent<BuoyancyEffector2DComponent>()) {
				BuoyancyEffector2DComponent& bec = entity.GetComponent<BuoyancyEffector2DComponent>();

				output << YAML::Key << "BuoyancyEffector2DComponent";
				output << YAML::BeginMap;
				output << YAML::Key << "DragMultiplier" << YAML::Value << bec.DragMultiplier;
				output << YAML::Key << "FlowAngle" << YAML::Value << bec.FlowAngle;
				output << YAML::Key << "FlowMagnitude" << YAML::Value << bec.FlowMagnitude;
				output << YAML::Key << "Density" << YAML::Value << bec.Density;
				output << YAML::EndMap;
			}

			output << YAML::EndMap; // End Entity components content

			output << YAML::EndMap; // End Entity content
		}

		output << YAML::EndSeq;

		std::ofstream fout(path);
		fout << output.c_str();
	}

	Scene* SceneSerializer::Deserialize(const std::string& path)
	{
		Scene* scene = new Scene(path);

		YAML::Node data = YAML::LoadFile(path);

		if (!data["Entities"]) {
			SW_ERROR("Error while deserializing the scene: {}, no entities section found!", path);
			return scene;
		}

		YAML::Node entities = data["Entities"];

		for (auto entity : entities) {
			YAML::Node idComponent = entity["Entity"]["IDComponent"];

			u64 id = idComponent["ID"].as<u64>();

			SW_DEBUG("Deserialized scene entity with id: {}", id);

			YAML::Node tagComponent = entity["Entity"]["TagComponent"];
			std::string tag = tagComponent["Tag"].as<std::string>();

			Entity deserialized = scene->CreateEntityWithID(id, tag);

			YAML::Node transformComponent = entity["Entity"]["TransformComponent"];
			if (transformComponent) {
				TransformComponent& tc = deserialized.GetComponent<TransformComponent>();

				tc.Position = transformComponent["Transform"].as<glm::vec3>();
				tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc.Scale = transformComponent["Scale"].as<glm::vec3>();
			}

			YAML::Node spriteComponent = entity["Entity"]["SpriteComponent"];
			if (spriteComponent) {
				SpriteComponent& sc = deserialized.AddComponent<SpriteComponent>();

				sc.Color = spriteComponent["Color"].as<glm::vec4>();

				if (spriteComponent["TexturePath"]) {
					std::string path = spriteComponent["TexturePath"].as<std::string>();

					sc.Texture = AssetManager::GetTexture2D(path.c_str());
				}
			}

			YAML::Node circleComponent = entity["Entity"]["CircleComponent"];
			if (circleComponent) {
				CircleComponent& cc = deserialized.AddComponent<CircleComponent>();

				cc.Color = circleComponent["Color"].as<glm::vec4>();
				cc.Thickness = circleComponent["Thickness"].as<f32>();
				cc.Fade = circleComponent["Fade"].as<f32>();
			}

			YAML::Node cameraComponent = entity["Entity"]["CameraComponent"];
			if (cameraComponent) {
				SceneCamera camera(cameraComponent["AspectRatio"].as<f32>());

				CameraComponent& cc = deserialized.AddComponent<CameraComponent>(camera);

				cc.Primary = cameraComponent["Primary"].as<bool>();
			}

			YAML::Node rigidBody2DComponent = entity["Entity"]["RigidBody2DComponent"];
			if (rigidBody2DComponent) {
				RigidBody2DComponent& rbc = deserialized.AddComponent<RigidBody2DComponent>();

				rbc.Type = (PhysicBodyType)rigidBody2DComponent["Type"].as<int>();
				rbc.GravityScale = rigidBody2DComponent["GravityScale"].as<f32>();
				rbc.Friction = rigidBody2DComponent["Friction"].as<f32>();
				rbc.Restitution = rigidBody2DComponent["Restitution"].as<f32>();
				rbc.RestitutionThreshold = rigidBody2DComponent["RestitutionThreshold"].as<f32>();
				rbc.AllowSleep = rigidBody2DComponent["AllowSleep"].as<bool>();
			}

			YAML::Node boxCollider2DComponent = entity["Entity"]["BoxCollider2DComponent"];
			if (boxCollider2DComponent) {
				BoxCollider2DComponent& bcc = deserialized.AddComponent<BoxCollider2DComponent>();

				bcc.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
				bcc.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
				bcc.Density = boxCollider2DComponent["Density"].as<f32>();
				bcc.IsSensor = boxCollider2DComponent["IsSensor"].as<bool>();
			}

			YAML::Node circleCollider2DComponent = entity["Entity"]["CircleCollider2DComponent"];
			if (circleCollider2DComponent) {
				CircleCollider2DComponent& ccc = deserialized.AddComponent<CircleCollider2DComponent>();

				ccc.Radius = circleCollider2DComponent["Radius"].as<f32>();
				ccc.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
				ccc.Density = circleCollider2DComponent["Density"].as<f32>();
				ccc.IsSensor = circleCollider2DComponent["IsSensor"].as<bool>();
			}

			YAML::Node buoyancyEffector2DComponent = entity["Entity"]["BuoyancyEffector2DComponent"];
			if (buoyancyEffector2DComponent) {
				BuoyancyEffector2DComponent& bec = deserialized.AddComponent<BuoyancyEffector2DComponent>();

				bec.DragMultiplier = buoyancyEffector2DComponent["DragMultiplier"].as<f32>();
				bec.FlowAngle = buoyancyEffector2DComponent["FlowAngle"].as<f32>();
				bec.FlowMagnitude = buoyancyEffector2DComponent["FlowMagnitude"].as<f32>();
				bec.Density = buoyancyEffector2DComponent["Density"].as<f32>();
			}
		}

		return scene;
	}

}
