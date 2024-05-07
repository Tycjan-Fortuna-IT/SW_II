#include "SceneSerializer.hpp"
#include "Scene.hpp"

#include <fstream>

#include "Asset/AssetManager.hpp"
#include "Core/Scripting/ScriptingCore.hpp"
#include "Core/Utils/SerializationUtils.hpp"
#include "Audio/SoundInstance.hpp"

namespace SW {

	void SceneSerializer::Serialize(Scene* scene, const std::filesystem::path& path)
	{
		YAML::Emitter output;

		output << YAML::BeginMap;
		output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		std::map<u64, Entity> sortedEntities;

		for (auto&& [handle, idc] : scene->GetRegistry().GetEntitiesWith<IDComponent>().each())
			sortedEntities[idc.ID] = Entity{ handle, scene };

		for (auto [id, entity] : sortedEntities) {
			SW_DEBUG("Serialized scene entity with id: {}", id);

			SerializeEntity(output, entity, scene);
		}

		output << YAML::EndSeq;

		std::ofstream fout(path);
		fout << output.c_str();
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& output, Entity entity, const Scene* scene)
	{
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

		const TransformComponent& tc = entity.GetComponent<TransformComponent>();

		output << YAML::Key << "TransformComponent";
		output << YAML::BeginMap;
		output << YAML::Key << "Transform" << YAML::Value << tc.Position;
		output << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
		output << YAML::Key << "Scale" << YAML::Value << tc.Scale;
		output << YAML::EndMap;

		if (entity.HasComponent<SpriteComponent>()) {
			const SpriteComponent& sc = entity.GetComponent<SpriteComponent>();

			output << YAML::Key << "SpriteComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Color" << YAML::Value << sc.Color;
			output << YAML::Key << "AssetHandle" << YAML::Value << sc.Handle;

			output << YAML::EndMap;
		}

		if (entity.HasComponent<AnimatedSpriteComponent>()) {
			const AnimatedSpriteComponent& asc = entity.GetComponent<AnimatedSpriteComponent>();

			output << YAML::Key << "AnimatedSpriteComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "CurrentFrame" << YAML::Value << asc.CurrentFrame;
			output << YAML::Key << "CurrentAnimationHandle" << YAML::Value << (asc.CurrentAnimation ?
				(*asc.CurrentAnimation)->GetHandle() : 0u);
			output << YAML::Key << "DefaultAnimationHandle" << YAML::Value << (asc.DefaultAnimation ?
				(*asc.DefaultAnimation)->GetHandle() : 0u);

			output << YAML::Key << "Animations" << YAML::Value << YAML::BeginSeq;

			for (auto&& [name, anim] : asc.Animations) {
				output << YAML::BeginMap;
				output << YAML::Key << "Animation";
				output << YAML::BeginMap;
				output << YAML::Key << "Name" << YAML::Value << name;
				output << YAML::Key << "AnimationHandle" << YAML::Value << (*anim)->GetHandle();
				output << YAML::EndMap;
				output << YAML::EndMap;
			}
			output << YAML::EndSeq;

			output << YAML::EndMap;
		}

		if (entity.HasComponent<CircleComponent>()) {
			const CircleComponent& cc = entity.GetComponent<CircleComponent>();

			output << YAML::Key << "CircleComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Color" << YAML::Value << cc.Color;
			output << YAML::Key << "Thickness" << YAML::Value << cc.Thickness;
			output << YAML::Key << "Fade" << YAML::Value << cc.Fade;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<TextComponent>()) {
			const TextComponent& textc = entity.GetComponent<TextComponent>();

			output << YAML::Key << "TextComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "AssetHandle" << YAML::Value << textc.Handle;
			output << YAML::Key << "TextString" << YAML::Value << textc.TextString;
			output << YAML::Key << "Color" << YAML::Value << textc.Color;
			output << YAML::Key << "Kerning" << YAML::Value << textc.Kerning;
			output << YAML::Key << "LineSpacing" << YAML::Value << textc.LineSpacing;

			output << YAML::EndMap;
		}

		if (entity.HasComponent<RelationshipComponent>()) {
			const RelationshipComponent& rsc = entity.GetComponent<RelationshipComponent>();

			output << YAML::Key << "RelationshipComponent";
			output << YAML::BeginMap;

			output << YAML::Key << "ParentID" << YAML::Value << rsc.ParentID;
			output << YAML::Key << "ChildrenCount" << YAML::Value << rsc.ChildrenIDs.size();

			if (!rsc.ChildrenIDs.empty()) {
				output << YAML::Key << "ChildrenIDs";
				output << YAML::BeginMap;
				for (size_t i = 0; i < rsc.ChildrenIDs.size(); i++)
					output << YAML::Key << i << YAML::Value << rsc.ChildrenIDs[i];
				output << YAML::EndMap;
			}

			output << YAML::EndMap;
		}

		if (entity.HasComponent<ScriptComponent>()) {
			const ScriptComponent& sc = entity.GetComponent<ScriptComponent>();

			output << YAML::Key << "ScriptComponent";
			output << YAML::BeginMap;

			output << YAML::Key << "ScriptID" << YAML::Value << sc.ScriptID;

			ScriptingCore& core = ScriptingCore::Get();
			if (core.IsValidScript(sc.ScriptID)) {
				const auto& scriptMetadata = core.GetScriptMetadata(sc.ScriptID);
				const auto& entityStorage = scene->GetScriptStorageC().EntityStorage.at(entity.GetID());

				output << YAML::Key << "ScriptName" << YAML::Value << scriptMetadata.FullName;
				output << YAML::Key << "Fields" << YAML::Value << YAML::BeginSeq;
				
				for (const auto& [fieldID, fieldStorage] : entityStorage.Fields) {
					const auto& fieldMetadata = scriptMetadata.Fields.at(fieldID);

					output << YAML::BeginMap;
					output << YAML::Key << "ID" << YAML::Value << fieldID;
					output << YAML::Key << "Name" << YAML::Value << fieldMetadata.Name;
					output << YAML::Key << "Type" << YAML::Value << DataTypeToString(fieldMetadata.Type);
					output << YAML::Key << "Value" << YAML::Value;

					if (fieldStorage.IsArray()) {

					} else {
						switch (fieldMetadata.Type) {
							case DataType::Byte:
								output << fieldStorage.GetValue<u8>();
								break;
							case DataType::Short:
								output << fieldStorage.GetValue<i16>();
								break;
							case DataType::UShort:
								output << fieldStorage.GetValue<u16>();
								break;
							case DataType::Int:
								output << fieldStorage.GetValue<i32>();
								break;
							case DataType::UInt:
								output << fieldStorage.GetValue<u32>();
								break;
							case DataType::Long:
								output << fieldStorage.GetValue<i64>();
								break;
							case DataType::ULong:
								output << fieldStorage.GetValue<u64>();
								break;
							case DataType::Float:
								output << fieldStorage.GetValue<f32>();
								break;
							case DataType::Double:
								output << fieldStorage.GetValue<f64>();
								break;
							case DataType::Bool:
								output << fieldStorage.GetValue<bool>();
								break;
							case DataType::Entity:
								output << fieldStorage.GetValue<u64>();
								break;
							case DataType::Prefab:
								output << fieldStorage.GetValue<u64>();
								break;
							default:
								break;
						}
					}

					output << YAML::EndMap;
				}

				output << YAML::EndSeq;
			}

			output << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>()) {
			const CameraComponent& cc = entity.GetComponent<CameraComponent>();

			output << YAML::Key << "CameraComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			output << YAML::Key << "AspectRatio" << YAML::Value << cc.Camera.GetAspectRatio();
			output << YAML::Key << "ProjectionType" << YAML::Value << (int)cc.Camera.GetProjectionType();

			if (cc.Camera.GetProjectionType() == ProjectionType::Orthographic) {

				output << YAML::Key << "OrthographicSize" << YAML::Value << cc.Camera.GetOrthographicSize();
				output << YAML::Key << "OrthographicNearClip" << YAML::Value << cc.Camera.GetOrthographicNearClip();
				output << YAML::Key << "OrthographicFarClip" << YAML::Value << cc.Camera.GetOrthographicFarClip();

			}
			else if (cc.Camera.GetProjectionType() == ProjectionType::Perspective) {

				output << YAML::Key << "PerspectiveVerticalFOV" << YAML::Value << cc.Camera.GetPerspectiveVerticalFOV();
				output << YAML::Key << "PerspectiveNearClip" << YAML::Value << cc.Camera.GetPerspectiveNearClip();
				output << YAML::Key << "PerspectiveFarClip" << YAML::Value << cc.Camera.GetPerspectiveFarClip();

			}

			output << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBody2DComponent>()) {
			const RigidBody2DComponent& rbc = entity.GetComponent<RigidBody2DComponent>();

			output << YAML::Key << "RigidBody2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Type" << YAML::Value << (int)rbc.Type;
			output << YAML::Key << "AutoMass" << YAML::Value << rbc.AutoMass;
			output << YAML::Key << "Mass" << YAML::Value << rbc.Mass;
			output << YAML::Key << "GravityScale" << YAML::Value << rbc.GravityScale;
			output << YAML::Key << "Friction" << YAML::Value << rbc.Friction;
			output << YAML::Key << "Restitution" << YAML::Value << rbc.Restitution;
			output << YAML::Key << "RestitutionThreshold" << YAML::Value << rbc.RestitutionThreshold;
			output << YAML::Key << "LinearDamping" << YAML::Value << rbc.LinearDamping;
			output << YAML::Key << "AngularDamping" << YAML::Value << rbc.AngularDamping; 
			output << YAML::Key << "FixedRotation" << YAML::Value << rbc.FixedRotation;
			output << YAML::Key << "AllowSleep" << YAML::Value << rbc.AllowSleep;
			output << YAML::Key << "InitiallyAwake" << YAML::Value << rbc.InitiallyAwake;
			output << YAML::Key << "IsBullet" << YAML::Value << rbc.IsBullet;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>()) {
			const BoxCollider2DComponent& bcc = entity.GetComponent<BoxCollider2DComponent>();

			output << YAML::Key << "BoxCollider2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Size" << YAML::Value << bcc.Size;
			output << YAML::Key << "Offset" << YAML::Value << bcc.Offset;
			output << YAML::Key << "Density" << YAML::Value << bcc.Density;
			output << YAML::Key << "IsSensor" << YAML::Value << bcc.IsSensor;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<CircleCollider2DComponent>()) {
			const CircleCollider2DComponent& ccc = entity.GetComponent<CircleCollider2DComponent>();

			output << YAML::Key << "CircleCollider2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Radius" << YAML::Value << ccc.Radius;
			output << YAML::Key << "Offset" << YAML::Value << ccc.Offset;
			output << YAML::Key << "Density" << YAML::Value << ccc.Density;
			output << YAML::Key << "IsSensor" << YAML::Value << ccc.IsSensor;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<PolygonCollider2DComponent>()) {
			const PolygonCollider2DComponent& pcc = entity.GetComponent<PolygonCollider2DComponent>();

			output << YAML::Key << "PolygonCollider2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Vertices" << YAML::BeginMap;
			for (u64 i = 0; i < pcc.Vertices.size(); ++i)
				output << YAML::Key << i << YAML::Key << pcc.Vertices[i];
			output << YAML::EndMap;
			output << YAML::Key << "VerticesCount" << YAML::Value << pcc.Vertices.size();
			output << YAML::Key << "Offset" << YAML::Value << pcc.Offset;
			output << YAML::Key << "Density" << YAML::Value << pcc.Density;
			output << YAML::Key << "IsSensor" << YAML::Value << pcc.IsSensor;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<BuoyancyEffector2DComponent>()) {
			const BuoyancyEffector2DComponent& bec = entity.GetComponent<BuoyancyEffector2DComponent>();

			output << YAML::Key << "BuoyancyEffector2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "DragMultiplier" << YAML::Value << bec.DragMultiplier;
			output << YAML::Key << "FlowAngle" << YAML::Value << bec.FlowAngle;
			output << YAML::Key << "FlowMagnitude" << YAML::Value << bec.FlowMagnitude;
			output << YAML::Key << "Density" << YAML::Value << bec.Density;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<DistanceJoint2DComponent>()) {
			const DistanceJoint2DComponent& djc = entity.GetComponent<DistanceJoint2DComponent>();

			output << YAML::Key << "DistanceJoint2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "ConnectedEntityID" << YAML::Value << djc.ConnectedEntityID;
			output << YAML::Key << "EnableCollision" << YAML::Value << djc.EnableCollision;
			output << YAML::Key << "AutoLength" << YAML::Value << djc.AutoLength;
			output << YAML::Key << "OriginAnchor" << YAML::Value << djc.OriginAnchor;
			output << YAML::Key << "ConnectedAnchor" << YAML::Value << djc.ConnectedAnchor;
			output << YAML::Key << "Length" << YAML::Value << djc.Length;
			output << YAML::Key << "MinLength" << YAML::Value << djc.MinLength;
			output << YAML::Key << "MaxLength" << YAML::Value << djc.MaxLength;
			output << YAML::Key << "BreakingForce" << YAML::Value << djc.BreakingForce;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<RevolutionJoint2DComponent>()) {
			const RevolutionJoint2DComponent& rjc = entity.GetComponent<RevolutionJoint2DComponent>();

			output << YAML::Key << "RevolutionJoint2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "ConnectedEntityID" << YAML::Value << rjc.ConnectedEntityID;
			output << YAML::Key << "OriginAnchor" << YAML::Value << rjc.OriginAnchor;
			output << YAML::Key << "LowerAngle" << YAML::Value << rjc.LowerAngle;
			output << YAML::Key << "UpperAngle" << YAML::Value << rjc.UpperAngle;
			output << YAML::Key << "MotorSpeed" << YAML::Value << rjc.MotorSpeed;
			output << YAML::Key << "MaxMotorTorque" << YAML::Value << rjc.MaxMotorTorque;
			output << YAML::Key << "BreakingForce" << YAML::Value << rjc.BreakingForce;
			output << YAML::Key << "BreakingTorque" << YAML::Value << rjc.BreakingTorque;
			output << YAML::Key << "EnableLimit" << YAML::Value << rjc.EnableLimit;
			output << YAML::Key << "EnableMotor" << YAML::Value << rjc.EnableMotor;
			output << YAML::Key << "EnableCollision" << YAML::Value << rjc.EnableCollision;

			output << YAML::EndMap;
		}

		if (entity.HasComponent<PrismaticJoint2DComponent>()) {
			const PrismaticJoint2DComponent& pjc = entity.GetComponent<PrismaticJoint2DComponent>();

			output << YAML::Key << "PrismaticJoint2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "ConnectedEntityID" << YAML::Value << pjc.ConnectedEntityID;
			output << YAML::Key << "OriginAnchor" << YAML::Value << pjc.OriginAnchor;
			output << YAML::Key << "Angle" << YAML::Value << pjc.Angle;
			output << YAML::Key << "LowerTranslation" << YAML::Value << pjc.LowerTranslation;
			output << YAML::Key << "UpperTranslation" << YAML::Value << pjc.UpperTranslation;
			output << YAML::Key << "MotorSpeed" << YAML::Value << pjc.MotorSpeed;
			output << YAML::Key << "MaxMotorForce" << YAML::Value << pjc.MaxMotorForce;
			output << YAML::Key << "BreakingForce" << YAML::Value << pjc.BreakingForce;
			output << YAML::Key << "BreakingTorque" << YAML::Value << pjc.BreakingTorque;
			output << YAML::Key << "EnableLimit" << YAML::Value << pjc.EnableLimit;
			output << YAML::Key << "EnableMotor" << YAML::Value << pjc.EnableMotor;
			output << YAML::Key << "EnableCollision" << YAML::Value << pjc.EnableCollision;

			output << YAML::EndMap;
		}

		if (entity.HasComponent<SpringJoint2DComponent>()) {
			const SpringJoint2DComponent& sjc = entity.GetComponent<SpringJoint2DComponent>();

			output << YAML::Key << "SpringJoint2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "ConnectedEntityID" << YAML::Value << sjc.ConnectedEntityID;
			output << YAML::Key << "EnableCollision" << YAML::Value << sjc.EnableCollision;
			output << YAML::Key << "AutoLength" << YAML::Value << sjc.AutoLength;
			output << YAML::Key << "OriginAnchor" << YAML::Value << sjc.OriginAnchor;
			output << YAML::Key << "ConnectedAnchor" << YAML::Value << sjc.ConnectedAnchor;
			output << YAML::Key << "Length" << YAML::Value << sjc.Length;
			output << YAML::Key << "MinLength" << YAML::Value << sjc.MinLength;
			output << YAML::Key << "MaxLength" << YAML::Value << sjc.MaxLength;
			output << YAML::Key << "BreakingForce" << YAML::Value << sjc.BreakingForce;
			output << YAML::Key << "Frequency" << YAML::Value << sjc.Frequency;
			output << YAML::Key << "DampingRatio" << YAML::Value << sjc.DampingRatio;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<WheelJoint2DComponent>()) {
			const WheelJoint2DComponent& wjc = entity.GetComponent<WheelJoint2DComponent>();

			output << YAML::Key << "WheelJoint2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "ConnectedEntityID" << YAML::Value << wjc.ConnectedEntityID;
			output << YAML::Key << "OriginAnchor" << YAML::Value << wjc.OriginAnchor;
			output << YAML::Key << "LowerTranslation" << YAML::Value << wjc.LowerTranslation;
			output << YAML::Key << "UpperTranslation" << YAML::Value << wjc.UpperTranslation;
			output << YAML::Key << "MotorSpeed" << YAML::Value << wjc.MotorSpeed;
			output << YAML::Key << "MaxMotorTorque" << YAML::Value << wjc.MaxMotorTorque;
			output << YAML::Key << "BreakingForce" << YAML::Value << wjc.BreakingForce;
			output << YAML::Key << "BreakingTorque" << YAML::Value << wjc.BreakingTorque;
			output << YAML::Key << "Frequency" << YAML::Value << wjc.Frequency;
			output << YAML::Key << "DampingRatio" << YAML::Value << wjc.DampingRatio;
			output << YAML::Key << "EnableLimit" << YAML::Value << wjc.EnableLimit;
			output << YAML::Key << "EnableMotor" << YAML::Value << wjc.EnableMotor;
			output << YAML::Key << "EnableCollision" << YAML::Value << wjc.EnableCollision;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<AudioSourceComponent>()) {
			const AudioSourceComponent& asc = entity.GetComponent<AudioSourceComponent>();

			output << YAML::Key << "AudioSourceComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "AudioHandle" << YAML::Value << asc.Handle;
			output << YAML::Key << "Volume" << YAML::Value << asc.Volume;
			output << YAML::Key << "Pitch" << YAML::Value << asc.Pitch;
			output << YAML::Key << "Looping" << YAML::Value << asc.Looping;
			output << YAML::Key << "PlayOnCreate" << YAML::Value << asc.PlayOnCreate;
			output << YAML::Key << "Is3D" << YAML::Value << asc.Is3D;
			output << YAML::Key << "Attenuation" << YAML::Value << (int)asc.Attenuation;
			output << YAML::Key << "RollOff" << YAML::Value << asc.RollOff;
			output << YAML::Key << "MinGain" << YAML::Value << asc.MinGain;
			output << YAML::Key << "MaxGain" << YAML::Value << asc.MaxGain;
			output << YAML::Key << "MinDistance" << YAML::Value << asc.MinDistance;
			output << YAML::Key << "MaxDistance" << YAML::Value << asc.MaxDistance;
			output << YAML::Key << "ConeInnerAngle" << YAML::Value << asc.ConeInnerAngle;
			output << YAML::Key << "ConeOuterAngle" << YAML::Value << asc.ConeOuterAngle;
			output << YAML::Key << "ConeOuterGain" << YAML::Value << asc.ConeOuterGain;
			output << YAML::Key << "DopplerFactor" << YAML::Value << asc.DopplerFactor;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<AudioListenerComponent>()) {
			const AudioListenerComponent& alc = entity.GetComponent<AudioListenerComponent>();

			output << YAML::Key << "AudioListenerComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "ConeInnerAngle" << YAML::Value << alc.ConeInnerAngle;
			output << YAML::Key << "ConeOuterAngle" << YAML::Value << alc.ConeOuterAngle;
			output << YAML::Key << "ConeOuterGain" << YAML::Value << alc.ConeOuterGain;
			output << YAML::EndMap;
		}

		output << YAML::EndMap; // End Entity components content

		output << YAML::EndMap; // End Entity content
	}

	Scene* SceneSerializer::Deserialize(const std::filesystem::path& path)
	{
		Scene* scene = new Scene();

		YAML::Node data = YAML::LoadFile(path.string());

		if (!data["Entities"]) {
			SW_ERROR("Error while deserializing the scene: {}, no entities section found!", path.string());
			return scene;
		}

		YAML::Node entities = data["Entities"];

		DeserializeEntitiesNode(entities, scene);

		return scene;
	}

	void SceneSerializer::DeserializeEntitiesNode(YAML::Node& entitiesNode, Scene* scene)
	{
		for (auto entity : entitiesNode) {
			YAML::Node idComponent = entity["Entity"]["IDComponent"];

			u64 id = idComponent["ID"].as<u64>();

			SW_DEBUG("Deserialized scene entity with id: {}", id);

			YAML::Node tagComponent = entity["Entity"]["TagComponent"];
			std::string tag = tagComponent["Tag"].as<std::string>();

			Entity deserialized = scene->CreateEntityWithID(id, tag);

			if (YAML::Node transformComponent = entity["Entity"]["TransformComponent"]) {
				TransformComponent& tc = deserialized.GetComponent<TransformComponent>();

				tc.Position = transformComponent["Transform"].as<glm::vec3>();
				tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc.Scale = transformComponent["Scale"].as<glm::vec3>();
			}

			if (YAML::Node spriteComponent = entity["Entity"]["SpriteComponent"]) {
				SpriteComponent& sc = deserialized.AddComponent<SpriteComponent>();

				sc.Color = spriteComponent["Color"].as<glm::vec4>();
				sc.Handle = spriteComponent["AssetHandle"].as<AssetHandle>();
			}

			if (YAML::Node animatedSpriteComponent = entity["Entity"]["AnimatedSpriteComponent"]) {
				AnimatedSpriteComponent& asc = deserialized.AddComponent<AnimatedSpriteComponent>();

				asc.CurrentFrame = animatedSpriteComponent["CurrentFrame"].as<int>();
				AssetHandle currentAnimHandle = animatedSpriteComponent["CurrentAnimationHandle"].as<u64>();
				AssetHandle defaultAnimHandle = animatedSpriteComponent["DefaultAnimationHandle"].as<u64>();

				asc.CurrentAnimation = currentAnimHandle ? AssetManager::GetAssetRaw<Animation2D>(currentAnimHandle) : nullptr;
				asc.DefaultAnimation = defaultAnimHandle ? AssetManager::GetAssetRaw<Animation2D>(defaultAnimHandle) : nullptr;

				YAML::Node animations = animatedSpriteComponent["Animations"];
				for (YAML::Node animation : animations) {
					Animation2D** anim = AssetManager::GetAssetRaw<Animation2D>(animation["Animation"]["AnimationHandle"].as<u64>());
					std::string key = animation["Animation"]["Name"].as<std::string>();

					asc.Animations[key] = anim;
				}
			}

			if (YAML::Node circleComponent = entity["Entity"]["CircleComponent"]) {
				CircleComponent& cc = deserialized.AddComponent<CircleComponent>();

				cc.Color = circleComponent["Color"].as<glm::vec4>();
				cc.Thickness = circleComponent["Thickness"].as<f32>();
				cc.Fade = circleComponent["Fade"].as<f32>();
			}

			if (YAML::Node textComponent = entity["Entity"]["TextComponent"]) {
				TextComponent& tc = deserialized.AddComponent<TextComponent>();

				tc.TextString = textComponent["TextString"].as<std::string>();
				tc.Color = textComponent["Color"].as<glm::vec4>();
				tc.Kerning = textComponent["Kerning"].as<f32>();
				tc.LineSpacing = textComponent["LineSpacing"].as<f32>();
				tc.Handle = textComponent["AssetHandle"].as<AssetHandle>();
			}

			if (YAML::Node relationshipComponent = entity["Entity"]["RelationshipComponent"]) {
				RelationshipComponent& rsc = deserialized.GetComponent<RelationshipComponent>();
				rsc.ParentID = relationshipComponent["ParentID"].as<u64>();

				const u64 childCount = relationshipComponent["ChildrenCount"].as<u64>();

				rsc.ChildrenIDs.clear();
				rsc.ChildrenIDs.reserve(childCount);

				const YAML::Node children = relationshipComponent["ChildrenIDs"];

				if (children && childCount > 0) {
					for (size_t i = 0; i < childCount; i++) {
						u64 child = children[i].as<u64>();

						if (child)
							rsc.ChildrenIDs.push_back(child);
					}
				}
			}

			if (YAML::Node scriptComponent = entity["Entity"]["ScriptComponent"]) {
				ScriptComponent& sc = deserialized.AddComponent<ScriptComponent>();

				u64 scriptId = scriptComponent["ScriptID"].as<u64>();

				ScriptingCore& core = ScriptingCore::Get();

				if (core.IsValidScript(scriptId)) {
					sc.ScriptID = scriptId;

					const auto& scriptMetadata = core.GetScriptMetadata(scriptId);

					scene->GetScriptStorage().InitializeEntityStorage(scriptId, id);

					for (YAML::Node field : scriptComponent["Fields"]) {
						u32 fieldID = field["ID"].as<uint32_t>(0);
						std::string fieldName = field["Name"].as<std::string>();

						if (scriptMetadata.Fields.contains(fieldID)) {
							const auto& fieldMetadata = scriptMetadata.Fields.at(fieldID);
							auto& fieldStorage = scene->GetScriptStorage().EntityStorage.at(id).Fields[fieldID];
							YAML::Node valueNode = field["Value"];

							if (fieldStorage.IsArray()) {

							} else {
								switch (fieldMetadata.Type) {
									case DataType::Byte:
									{
										fieldStorage.SetValue(valueNode.as<u8>());
									} break;
									case DataType::Short:
									{
										fieldStorage.SetValue(valueNode.as<i16>());
									} break;
									case DataType::UShort:
									{
										fieldStorage.SetValue(valueNode.as<u16>());
									} break;
									case DataType::Int:
									{
										fieldStorage.SetValue(valueNode.as<i32>());
									} break;
									case DataType::UInt:
									{
										fieldStorage.SetValue(valueNode.as<u32>());
									} break;
									case DataType::Long:
									{
										fieldStorage.SetValue(valueNode.as<i64>());
									} break;
									case DataType::ULong:
									{
										fieldStorage.SetValue(valueNode.as<u64>());
									} break;
									case DataType::Float:
									{
										fieldStorage.SetValue(valueNode.as<f32>());
									} break;
									case DataType::Double:
									{
										fieldStorage.SetValue(valueNode.as<f64>());
									} break;
									case DataType::Bool:
									{
										fieldStorage.SetValue(valueNode.as<bool>());
									} break;
									case DataType::Entity:
									{
										fieldStorage.SetValue(valueNode.as<u64>());
									} break;
									case DataType::Prefab:
									{
										fieldStorage.SetValue(valueNode.as<u64>());
									} break;
									default:
										break;
								}
							}
						}
					}
				}
			}

			if (YAML::Node cameraComponent = entity["Entity"]["CameraComponent"]) {
				SceneCamera camera(cameraComponent["AspectRatio"].as<f32>());

				CameraComponent& cc = deserialized.AddComponent<CameraComponent>(camera);

				cc.Primary = cameraComponent["Primary"].as<bool>();
				cc.Camera.SetProjectionType((ProjectionType)cameraComponent["ProjectionType"].as<int>());

				if (cc.Camera.GetProjectionType() == ProjectionType::Orthographic) {

					cc.Camera.SetOrthographicSize(cameraComponent["OrthographicSize"].as<f32>());
					cc.Camera.SetOrthographicNearClip(cameraComponent["OrthographicNearClip"].as<f32>());
					cc.Camera.SetOrthographicFarClip(cameraComponent["OrthographicFarClip"].as<f32>());

				} else if (cc.Camera.GetProjectionType() == ProjectionType::Perspective) {

					cc.Camera.SetPerspectiveVerticalFOV(cameraComponent["PerspectiveVerticalFOV"].as<f32>());
					cc.Camera.SetPerspectiveNearClip(cameraComponent["PerspectiveNearClip"].as<f32>());
					cc.Camera.SetPerspectiveFarClip(cameraComponent["PerspectiveFarClip"].as<f32>());

				}
			}

			if (YAML::Node rigidBody2DComponent = entity["Entity"]["RigidBody2DComponent"]) {
				RigidBody2DComponent& rbc = deserialized.AddComponent<RigidBody2DComponent>();

				rbc.Type = (PhysicBodyType)rigidBody2DComponent["Type"].as<int>();
				rbc.AutoMass = rigidBody2DComponent["AutoMass"].as<bool>();
				rbc.Mass = rigidBody2DComponent["Mass"].as<f32>();
				rbc.GravityScale = rigidBody2DComponent["GravityScale"].as<f32>();
				rbc.Friction = rigidBody2DComponent["Friction"].as<f32>();
				rbc.Restitution = rigidBody2DComponent["Restitution"].as<f32>();
				rbc.RestitutionThreshold = rigidBody2DComponent["RestitutionThreshold"].as<f32>();
				rbc.LinearDamping = rigidBody2DComponent["LinearDamping"].as<f32>();
				rbc.AngularDamping = rigidBody2DComponent["AngularDamping"].as<f32>();
				rbc.FixedRotation = rigidBody2DComponent["FixedRotation"].as<bool>();
				rbc.AllowSleep = rigidBody2DComponent["AllowSleep"].as<bool>();
				rbc.InitiallyAwake = rigidBody2DComponent["InitiallyAwake"].as<bool>();
				rbc.IsBullet = rigidBody2DComponent["IsBullet"].as<bool>();
			}

			if (YAML::Node boxCollider2DComponent = entity["Entity"]["BoxCollider2DComponent"]) {
				BoxCollider2DComponent& bcc = deserialized.AddComponent<BoxCollider2DComponent>();

				bcc.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
				bcc.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
				bcc.Density = boxCollider2DComponent["Density"].as<f32>();
				bcc.IsSensor = boxCollider2DComponent["IsSensor"].as<bool>();
			}

			if (YAML::Node circleCollider2DComponent = entity["Entity"]["CircleCollider2DComponent"]) {
				CircleCollider2DComponent& ccc = deserialized.AddComponent<CircleCollider2DComponent>();

				ccc.Radius = circleCollider2DComponent["Radius"].as<f32>();
				ccc.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
				ccc.Density = circleCollider2DComponent["Density"].as<f32>();
				ccc.IsSensor = circleCollider2DComponent["IsSensor"].as<bool>();
			}

			if (YAML::Node polygonCollider2DComponent = entity["Entity"]["PolygonCollider2DComponent"]) {
				PolygonCollider2DComponent& pcc = deserialized.AddComponent<PolygonCollider2DComponent>();

				YAML::Node vertices = polygonCollider2DComponent["Vertices"];
				u64 count = polygonCollider2DComponent["VerticesCount"].as<u64>();

				pcc.Vertices.clear();

				for (u64 i = 0; i < count; ++i) {
					pcc.Vertices.push_back(vertices[i].as<glm::vec2>());
				}

				pcc.Offset = polygonCollider2DComponent["Offset"].as<glm::vec2>();
				pcc.Density = polygonCollider2DComponent["Density"].as<f32>();
				pcc.IsSensor = polygonCollider2DComponent["IsSensor"].as<bool>();
			}

			if (YAML::Node buoyancyEffector2DComponent = entity["Entity"]["BuoyancyEffector2DComponent"]) {
				BuoyancyEffector2DComponent& bec = deserialized.AddComponent<BuoyancyEffector2DComponent>();

				bec.DragMultiplier = buoyancyEffector2DComponent["DragMultiplier"].as<f32>();
				bec.FlowAngle = buoyancyEffector2DComponent["FlowAngle"].as<f32>();
				bec.FlowMagnitude = buoyancyEffector2DComponent["FlowMagnitude"].as<f32>();
				bec.Density = buoyancyEffector2DComponent["Density"].as<f32>();
			}

			if (YAML::Node distanceJoint2DComponent = entity["Entity"]["DistanceJoint2DComponent"]) {
				DistanceJoint2DComponent& djc = deserialized.AddComponent<DistanceJoint2DComponent>();

				djc.ConnectedEntityID = distanceJoint2DComponent["ConnectedEntityID"].as<u64>();
				djc.EnableCollision = distanceJoint2DComponent["EnableCollision"].as<bool>();
				djc.AutoLength = distanceJoint2DComponent["AutoLength"].as<bool>();
				djc.OriginAnchor = distanceJoint2DComponent["OriginAnchor"].as<glm::vec2>();
				djc.ConnectedAnchor = distanceJoint2DComponent["ConnectedAnchor"].as<glm::vec2>();
				djc.Length = distanceJoint2DComponent["Length"].as<f32>();
				djc.MinLength = distanceJoint2DComponent["MinLength"].as<f32>();
				djc.MaxLength = distanceJoint2DComponent["MaxLength"].as<f32>();
				djc.BreakingForce = distanceJoint2DComponent["BreakingForce"].as<f32>();
			}

			if (YAML::Node revolutionJoint2DComponent = entity["Entity"]["RevolutionJoint2DComponent"]) {
				RevolutionJoint2DComponent& rjc = deserialized.AddComponent<RevolutionJoint2DComponent>();

				rjc.ConnectedEntityID = revolutionJoint2DComponent["ConnectedEntityID"].as<u64>();
				rjc.OriginAnchor = revolutionJoint2DComponent["OriginAnchor"].as<glm::vec2>();
				rjc.LowerAngle = revolutionJoint2DComponent["LowerAngle"].as<f32>();
				rjc.UpperAngle = revolutionJoint2DComponent["UpperAngle"].as<f32>();
				rjc.MotorSpeed = revolutionJoint2DComponent["MotorSpeed"].as<f32>();
				rjc.MaxMotorTorque = revolutionJoint2DComponent["MaxMotorTorque"].as<f32>();
				rjc.BreakingForce = revolutionJoint2DComponent["BreakingForce"].as<f32>();
				rjc.BreakingTorque = revolutionJoint2DComponent["BreakingTorque"].as<f32>();
				rjc.EnableLimit = revolutionJoint2DComponent["EnableLimit"].as<bool>();
				rjc.EnableMotor = revolutionJoint2DComponent["EnableMotor"].as<bool>();
				rjc.EnableCollision = revolutionJoint2DComponent["EnableCollision"].as<bool>();
			}

			if (YAML::Node prismaticJoint2DComponent = entity["Entity"]["PrismaticJoint2DComponent"]) {
				PrismaticJoint2DComponent& pjc = deserialized.AddComponent<PrismaticJoint2DComponent>();

				pjc.ConnectedEntityID = prismaticJoint2DComponent["ConnectedEntityID"].as<u64>();
				pjc.OriginAnchor = prismaticJoint2DComponent["OriginAnchor"].as<glm::vec2>();
				pjc.Angle = prismaticJoint2DComponent["Angle"].as<f32>();
				pjc.LowerTranslation = prismaticJoint2DComponent["LowerTranslation"].as<f32>();
				pjc.UpperTranslation = prismaticJoint2DComponent["UpperTranslation"].as<f32>();
				pjc.MotorSpeed = prismaticJoint2DComponent["MotorSpeed"].as<f32>();
				pjc.MaxMotorForce = prismaticJoint2DComponent["MaxMotorForce"].as<f32>();
				pjc.BreakingForce = prismaticJoint2DComponent["BreakingForce"].as<f32>();
				pjc.BreakingTorque = prismaticJoint2DComponent["BreakingTorque"].as<f32>();
				pjc.EnableLimit = prismaticJoint2DComponent["EnableLimit"].as<bool>();
				pjc.EnableMotor = prismaticJoint2DComponent["EnableMotor"].as<bool>();
				pjc.EnableCollision = prismaticJoint2DComponent["EnableCollision"].as<bool>();
			}

			if (YAML::Node springJoint2DComponent = entity["Entity"]["SpringJoint2DComponent"]) {
				SpringJoint2DComponent& sjc = deserialized.AddComponent<SpringJoint2DComponent>();

				sjc.ConnectedEntityID = springJoint2DComponent["ConnectedEntityID"].as<u64>();
				sjc.EnableCollision = springJoint2DComponent["EnableCollision"].as<bool>();
				sjc.AutoLength = springJoint2DComponent["AutoLength"].as<bool>();
				sjc.OriginAnchor = springJoint2DComponent["OriginAnchor"].as<glm::vec2>();
				sjc.ConnectedAnchor = springJoint2DComponent["ConnectedAnchor"].as<glm::vec2>();
				sjc.Length = springJoint2DComponent["Length"].as<f32>();
				sjc.MinLength = springJoint2DComponent["MinLength"].as<f32>();
				sjc.MaxLength = springJoint2DComponent["MaxLength"].as<f32>();
				sjc.BreakingForce = springJoint2DComponent["BreakingForce"].as<f32>();
				sjc.Frequency = springJoint2DComponent["Frequency"].as<f32>();
				sjc.DampingRatio = springJoint2DComponent["DampingRatio"].as<f32>();
			}

			if (YAML::Node wheelJoint2DComponent = entity["Entity"]["WheelJoint2DComponent"]) {
				WheelJoint2DComponent& wjc = deserialized.AddComponent<WheelJoint2DComponent>();

				wjc.ConnectedEntityID = wheelJoint2DComponent["ConnectedEntityID"].as<u64>();
				wjc.OriginAnchor = wheelJoint2DComponent["OriginAnchor"].as<glm::vec2>();
				wjc.LowerTranslation = wheelJoint2DComponent["LowerTranslation"].as<f32>();
				wjc.UpperTranslation = wheelJoint2DComponent["UpperTranslation"].as<f32>();
				wjc.MotorSpeed = wheelJoint2DComponent["MotorSpeed"].as<f32>();
				wjc.MaxMotorTorque = wheelJoint2DComponent["MaxMotorTorque"].as<f32>();
				wjc.BreakingForce = wheelJoint2DComponent["BreakingForce"].as<f32>();
				wjc.BreakingTorque = wheelJoint2DComponent["BreakingTorque"].as<f32>();
				wjc.Frequency = wheelJoint2DComponent["Frequency"].as<f32>();
				wjc.DampingRatio = wheelJoint2DComponent["DampingRatio"].as<f32>();
				wjc.EnableLimit = wheelJoint2DComponent["EnableLimit"].as<bool>();
				wjc.EnableMotor = wheelJoint2DComponent["EnableMotor"].as<bool>();
				wjc.EnableCollision = wheelJoint2DComponent["EnableCollision"].as<bool>();
			}

			if (YAML::Node audioSourceComponent = entity["Entity"]["AudioSourceComponent"]) {
				AudioSourceComponent& asc = deserialized.AddComponent<AudioSourceComponent>();

				asc.Handle = audioSourceComponent["AudioHandle"].as<AssetHandle>();
				asc.Volume = audioSourceComponent["Volume"].as<f32>();
				asc.Pitch = audioSourceComponent["Pitch"].as<f32>();
				asc.Looping = audioSourceComponent["Looping"].as<bool>();
				asc.PlayOnCreate = audioSourceComponent["PlayOnCreate"].as<bool>();
				asc.Is3D = audioSourceComponent["Is3D"].as<bool>();
				asc.Attenuation = (AttenuationType)audioSourceComponent["Attenuation"].as<int>();
				asc.RollOff = audioSourceComponent["RollOff"].as<f32>();
				asc.MinGain = audioSourceComponent["MinGain"].as<f32>();
				asc.MaxGain = audioSourceComponent["MaxGain"].as<f32>();
				asc.MinDistance = audioSourceComponent["MinDistance"].as<f32>();
				asc.MaxDistance = audioSourceComponent["MaxDistance"].as<f32>();
				asc.ConeInnerAngle = audioSourceComponent["ConeInnerAngle"].as<f32>();
				asc.ConeOuterAngle = audioSourceComponent["ConeOuterAngle"].as<f32>();
				asc.ConeOuterGain = audioSourceComponent["ConeOuterGain"].as<f32>();
				asc.DopplerFactor = audioSourceComponent["DopplerFactor"].as<f32>();
			}

			if (YAML::Node audioListenerComponent = entity["Entity"]["AudioListenerComponent"]) {
				AudioListenerComponent& alc = deserialized.AddComponent<AudioListenerComponent>();

				alc.ConeInnerAngle = audioListenerComponent["ConeInnerAngle"].as<f32>();
				alc.ConeOuterAngle = audioListenerComponent["ConeOuterAngle"].as<f32>();
				alc.ConeOuterGain = audioListenerComponent["ConeOuterGain"].as<f32>();
			}
		}
	}

}
