#include "SceneSerializer.hpp"
#include "Scene.hpp"

#include <fstream>

#include "Asset/AssetManager.hpp"
#include "Audio/SoundInstance.hpp"
#include "Core/Scripting/ScriptingCore.hpp"
#include "Core/Utils/SerializationUtils.hpp"

namespace SW
{

	void SceneSerializer::Serialize(Scene* scene, const std::filesystem::path& path)
	{
		YAML::Emitter output;

		output << YAML::BeginMap;
		output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		std::map<u64, Entity> sortedEntities;

		for (auto&& [handle, idc] : scene->GetRegistry().GetEntitiesWith<IDComponent>().each())
			sortedEntities[idc.ID] = Entity{handle, scene};

		for (auto [id, entity] : sortedEntities)
		{
			APP_DEBUG("Serialized scene entity with id: {}", id);

			SerializeEntity(output, entity, scene);
		}

		output << YAML::EndSeq;

		std::ofstream fout(path);
		fout << output.c_str();
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& output, Entity entity, const Scene* scene)
	{
		if (!entity)
		{
			APP_WARN("SceneSerializer - NULL entity, skipped.");
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

		if (entity.HasComponent<SpriteComponent>())
		{
			const SpriteComponent& sc = entity.GetComponent<SpriteComponent>();

			output << YAML::Key << "SpriteComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Color" << YAML::Value << sc.Color;
			output << YAML::Key << "AssetHandle" << YAML::Value << sc.Handle;
			output << YAML::Key << "ZIndex" << YAML::Value << sc.ZIndex;

			output << YAML::EndMap;
		}

		if (entity.HasComponent<AnimatedSpriteComponent>())
		{
			const AnimatedSpriteComponent& asc = entity.GetComponent<AnimatedSpriteComponent>();

			output << YAML::Key << "AnimatedSpriteComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "CurrentFrame" << YAML::Value << asc.CurrentFrame;
			output << YAML::Key << "CurrentAnimationHandle" << YAML::Value
			       << (asc.CurrentAnimation ? (*asc.CurrentAnimation)->GetHandle() : 0u);
			output << YAML::Key << "DefaultAnimationHandle" << YAML::Value
			       << (asc.DefaultAnimation ? (*asc.DefaultAnimation)->GetHandle() : 0u);

			output << YAML::Key << "Animations" << YAML::Value << YAML::BeginSeq;

			for (auto&& [name, anim] : asc.Animations)
			{
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

		if (entity.HasComponent<CircleComponent>())
		{
			const CircleComponent& cc = entity.GetComponent<CircleComponent>();

			output << YAML::Key << "CircleComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Color" << YAML::Value << cc.Color;
			output << YAML::Key << "Thickness" << YAML::Value << cc.Thickness;
			output << YAML::Key << "Fade" << YAML::Value << cc.Fade;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<TextComponent>())
		{
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

		if (entity.HasComponent<RelationshipComponent>())
		{
			const RelationshipComponent& rsc = entity.GetComponent<RelationshipComponent>();

			output << YAML::Key << "RelationshipComponent";
			output << YAML::BeginMap;

			output << YAML::Key << "ParentID" << YAML::Value << rsc.ParentID;
			output << YAML::Key << "ChildrenCount" << YAML::Value << rsc.ChildrenIDs.size();

			if (!rsc.ChildrenIDs.empty())
			{
				output << YAML::Key << "ChildrenIDs";
				output << YAML::BeginMap;
				for (size_t i = 0; i < rsc.ChildrenIDs.size(); i++)
					output << YAML::Key << i << YAML::Value << rsc.ChildrenIDs[i];
				output << YAML::EndMap;
			}

			output << YAML::EndMap;
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			const ScriptComponent& sc = entity.GetComponent<ScriptComponent>();

			output << YAML::Key << "ScriptComponent";
			output << YAML::BeginMap;

			output << YAML::Key << "ScriptID" << YAML::Value << sc.ScriptID;

			ScriptingCore& core = ScriptingCore::Get();
			if (core.IsValidScript(sc.ScriptID))
			{
				const auto& scriptMetadata = core.GetScriptMetadata(sc.ScriptID);
				const auto& entityStorage  = scene->GetScriptStorageC().EntityStorage.at(entity.GetID());

				output << YAML::Key << "ScriptName" << YAML::Value << scriptMetadata.FullName;
				output << YAML::Key << "Fields" << YAML::Value << YAML::BeginSeq;

				for (const auto& [fieldID, fieldStorage] : entityStorage.Fields)
				{
					const auto& fieldMetadata = scriptMetadata.Fields.at(fieldID);

					output << YAML::BeginMap;
					output << YAML::Key << "ID" << YAML::Value << fieldID;
					output << YAML::Key << "Name" << YAML::Value << fieldMetadata.Name;
					output << YAML::Key << "Type" << YAML::Value << DataTypeToString(fieldMetadata.Type);
					output << YAML::Key << "Value" << YAML::Value;

					if (fieldStorage.IsArray())
					{
					}
					else
					{
						switch (fieldMetadata.Type)
						{
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

		if (entity.HasComponent<CameraComponent>())
		{
			const CameraComponent& cc = entity.GetComponent<CameraComponent>();

			output << YAML::Key << "CameraComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			output << YAML::Key << "AspectRatio" << YAML::Value << cc.Camera.GetAspectRatio();
			output << YAML::Key << "ProjectionType" << YAML::Value << (int)cc.Camera.GetProjectionType();

			if (cc.Camera.GetProjectionType() == ProjectionType::Orthographic)
			{

				output << YAML::Key << "OrthographicSize" << YAML::Value << cc.Camera.GetOrthographicSize();
				output << YAML::Key << "OrthographicNearClip" << YAML::Value << cc.Camera.GetOrthographicNearClip();
				output << YAML::Key << "OrthographicFarClip" << YAML::Value << cc.Camera.GetOrthographicFarClip();
			}
			else if (cc.Camera.GetProjectionType() == ProjectionType::Perspective)
			{

				output << YAML::Key << "PerspectiveVerticalFOV" << YAML::Value << cc.Camera.GetPerspectiveVerticalFOV();
				output << YAML::Key << "PerspectiveNearClip" << YAML::Value << cc.Camera.GetPerspectiveNearClip();
				output << YAML::Key << "PerspectiveFarClip" << YAML::Value << cc.Camera.GetPerspectiveFarClip();
			}

			output << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
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

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			const BoxCollider2DComponent& bcc = entity.GetComponent<BoxCollider2DComponent>();

			output << YAML::Key << "BoxCollider2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Size" << YAML::Value << bcc.Size;
			output << YAML::Key << "Offset" << YAML::Value << bcc.Offset;
			output << YAML::Key << "Density" << YAML::Value << bcc.Density;
			output << YAML::Key << "IsSensor" << YAML::Value << bcc.IsSensor;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			const CircleCollider2DComponent& ccc = entity.GetComponent<CircleCollider2DComponent>();

			output << YAML::Key << "CircleCollider2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Radius" << YAML::Value << ccc.Radius;
			output << YAML::Key << "Offset" << YAML::Value << ccc.Offset;
			output << YAML::Key << "Density" << YAML::Value << ccc.Density;
			output << YAML::Key << "IsSensor" << YAML::Value << ccc.IsSensor;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<PolygonCollider2DComponent>())
		{
			const PolygonCollider2DComponent& pcc = entity.GetComponent<PolygonCollider2DComponent>();

			output << YAML::Key << "PolygonCollider2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "Vertices" << YAML::BeginMap;
			for (u64 i = 0; i < pcc.Vertices.size(); ++i) output << YAML::Key << i << YAML::Key << pcc.Vertices[i];
			output << YAML::EndMap;
			output << YAML::Key << "VerticesCount" << YAML::Value << pcc.Vertices.size();
			output << YAML::Key << "Offset" << YAML::Value << pcc.Offset;
			output << YAML::Key << "Density" << YAML::Value << pcc.Density;
			output << YAML::Key << "IsSensor" << YAML::Value << pcc.IsSensor;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<BuoyancyEffector2DComponent>())
		{
			const BuoyancyEffector2DComponent& bec = entity.GetComponent<BuoyancyEffector2DComponent>();

			output << YAML::Key << "BuoyancyEffector2DComponent";
			output << YAML::BeginMap;
			output << YAML::Key << "DragMultiplier" << YAML::Value << bec.DragMultiplier;
			output << YAML::Key << "FlowAngle" << YAML::Value << bec.FlowAngle;
			output << YAML::Key << "FlowMagnitude" << YAML::Value << bec.FlowMagnitude;
			output << YAML::Key << "Density" << YAML::Value << bec.Density;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<DistanceJoint2DComponent>())
		{
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

		if (entity.HasComponent<RevolutionJoint2DComponent>())
		{
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

		if (entity.HasComponent<PrismaticJoint2DComponent>())
		{
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

		if (entity.HasComponent<SpringJoint2DComponent>())
		{
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

		if (entity.HasComponent<WheelJoint2DComponent>())
		{
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

		if (entity.HasComponent<AudioSourceComponent>())
		{
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
			// output << YAML::Key << "ConeInnerAngle" << YAML::Value << asc.ConeInnerAngle;
			// output << YAML::Key << "ConeOuterAngle" << YAML::Value << asc.ConeOuterAngle;
			// output << YAML::Key << "ConeOuterGain" << YAML::Value << asc.ConeOuterGain;
			output << YAML::Key << "DopplerFactor" << YAML::Value << asc.DopplerFactor;
			output << YAML::EndMap;
		}

		if (entity.HasComponent<AudioListenerComponent>())
		{
			// const AudioListenerComponent& alc = entity.GetComponent<AudioListenerComponent>();

			output << YAML::Key << "AudioListenerComponent";
			output << YAML::BeginMap;
			// output << YAML::Key << "ConeInnerAngle" << YAML::Value << alc.ConeInnerAngle;
			// output << YAML::Key << "ConeOuterAngle" << YAML::Value << alc.ConeOuterAngle;
			// output << YAML::Key << "ConeOuterGain" << YAML::Value << alc.ConeOuterGain;
			output << YAML::EndMap;
		}

		output << YAML::EndMap; // End Entity components content

		output << YAML::EndMap; // End Entity content
	}

	Scene* SceneSerializer::Deserialize(const std::filesystem::path& path)
	{
		Scene* scene = new Scene();

		try
		{
			YAML::Node data = YAML::LoadFile(path.string());

			if (!data["Entities"])
			{
				APP_ERROR("Error while deserializing the scene: {}, no entities section found!", path.string());
				return scene;
			}

			YAML::Node entities = data["Entities"];

			DeserializeEntitiesNode(entities, scene);
		}
		catch (const YAML::ParserException& e)
		{
			APP_ERROR("Error while deserializing the scene: {}, {}", path.string(), e.what());
			return scene;
		}

		return scene;
	}

	void SceneSerializer::DeserializeEntitiesNode(YAML::Node& entitiesNode, Scene* scene)
	{
		for (auto entity : entitiesNode)
		{
			YAML::Node idComponent = entity["Entity"]["IDComponent"];

			u64 id = TryDeserializeNode<u64>(idComponent, "ID", 0);

			APP_TRACE("Deserializing entity with ID: {}", id);

			YAML::Node tagComponent = entity["Entity"]["TagComponent"];
			std::string tag         = TryDeserializeNode<std::string>(tagComponent, "Tag", "Entity");

			Entity deserialized = scene->CreateEntityWithID(id, tag);

			if (YAML::Node transformComponent = entity["Entity"]["TransformComponent"])
			{
				TransformComponent& tc = deserialized.GetComponent<TransformComponent>();

				tc.Position = TryDeserializeNode<glm::vec3>(transformComponent, "Transform", glm::vec3(0.0f));
				tc.Rotation = TryDeserializeNode<glm::vec3>(transformComponent, "Rotation", glm::vec3(0.0f));
				tc.Scale    = TryDeserializeNode<glm::vec3>(transformComponent, "Scale", glm::vec3(1.0f));
			}

			if (YAML::Node spriteComponent = entity["Entity"]["SpriteComponent"])
			{
				SpriteComponent& sc = deserialized.AddComponent<SpriteComponent>();

				sc.Color  = TryDeserializeNode<glm::vec4>(spriteComponent, "Color", glm::vec4(1.0f));
				sc.ZIndex = TryDeserializeNode<i32>(spriteComponent, "ZIndex", 0);

				const AssetHandle handle = TryDeserializeNode<AssetHandle>(spriteComponent, "AssetHandle", 0);

				if (handle == 0 || AssetManager::IsValid(handle))
				{
					sc.Handle = handle;
				}
				else
				{
					APP_ERROR("SceneSerializer - Invalid ID: {} for sprite, skipping.", handle);
				}
			}

			if (YAML::Node animatedSpriteComponent = entity["Entity"]["AnimatedSpriteComponent"])
			{
				AnimatedSpriteComponent& asc = deserialized.AddComponent<AnimatedSpriteComponent>();

				asc.CurrentFrame = TryDeserializeNode<int>(animatedSpriteComponent, "CurrentFrame", 0);
				AssetHandle currentAnimHandle =
				    TryDeserializeNode<AssetHandle>(animatedSpriteComponent, "CurrentAnimationHandle", 0);
				AssetHandle defaultAnimHandle =
				    TryDeserializeNode<AssetHandle>(animatedSpriteComponent, "DefaultAnimationHandle", 0);

				asc.CurrentAnimation =
				    currentAnimHandle ? AssetManager::GetAssetRaw<Animation2D>(currentAnimHandle) : nullptr;
				asc.DefaultAnimation =
				    defaultAnimHandle ? AssetManager::GetAssetRaw<Animation2D>(defaultAnimHandle) : nullptr;

				YAML::Node animations = animatedSpriteComponent["Animations"];
				for (YAML::Node animation : animations)
				{
					const AssetHandle animHandle =
					    TryDeserializeNode<AssetHandle>(animation["Animation"], "AnimationHandle", 0);
					Animation2D** anim = AssetManager::GetAssetRaw<Animation2D>(animHandle);

					if (anim && *anim)
					{
						std::string key = TryDeserializeNode<std::string>(animation["Animation"], "Name", "Unnamed");

						asc.Animations[key] = anim;
					}
					else
					{
						APP_ERROR("SceneSerializer - Invalid ID: {} for animation, skipping.", animHandle);
					}
				}
			}

			if (YAML::Node circleComponent = entity["Entity"]["CircleComponent"])
			{
				CircleComponent& cc = deserialized.AddComponent<CircleComponent>();

				cc.Color     = TryDeserializeNode<glm::vec4>(circleComponent, "Color", glm::vec4(1.0f));
				cc.Thickness = TryDeserializeNode<f32>(circleComponent, "Thickness", 1.0f);
				cc.Fade      = TryDeserializeNode<bool>(circleComponent, "Fade", false);
			}

			if (YAML::Node textComponent = entity["Entity"]["TextComponent"])
			{
				TextComponent& tc = deserialized.AddComponent<TextComponent>();

				tc.TextString  = TryDeserializeNode<std::string>(textComponent, "TextString", "Text");
				tc.Color       = TryDeserializeNode<glm::vec4>(textComponent, "Color", glm::vec4(1.0f));
				tc.Kerning     = TryDeserializeNode<f32>(textComponent, "Kerning", 0.0f);
				tc.LineSpacing = TryDeserializeNode<f32>(textComponent, "LineSpacing", 0.0f);
				tc.Handle      = TryDeserializeNode<AssetHandle>(textComponent, "AssetHandle", 0);
			}

			if (YAML::Node relationshipComponent = entity["Entity"]["RelationshipComponent"])
			{
				RelationshipComponent& rsc = deserialized.GetComponent<RelationshipComponent>();
				rsc.ParentID               = TryDeserializeNode<u64>(relationshipComponent, "ParentID", 0);

				const u64 childCount = TryDeserializeNode<u64>(relationshipComponent, "ChildrenCount", 0);

				rsc.ChildrenIDs.clear();
				rsc.ChildrenIDs.reserve(childCount);

				const YAML::Node children = relationshipComponent["ChildrenIDs"];

				if (children && childCount > 0)
				{
					for (size_t i = 0; i < childCount; i++)
					{
						u64 child = TryDeserializeNode<u64>(children, std::to_string(i), 0);

						if (child)
							rsc.ChildrenIDs.push_back(child);
					}
				}
			}

			if (YAML::Node scriptComponent = entity["Entity"]["ScriptComponent"])
			{
				ScriptComponent& sc = deserialized.AddComponent<ScriptComponent>();

				u64 scriptId = TryDeserializeNode<u64>(scriptComponent, "ScriptID", 0);

				ScriptingCore& core = ScriptingCore::Get();

				if (core.IsValidScript(scriptId))
				{
					sc.ScriptID = scriptId;

					const auto& scriptMetadata = core.GetScriptMetadata(scriptId);

					scene->GetScriptStorage().InitializeEntityStorage(scriptId, id);

					for (YAML::Node field : scriptComponent["Fields"])
					{
						u32 fieldID           = TryDeserializeNode<u32>(field, "ID", 0);
						std::string fieldName = TryDeserializeNode<std::string>(field, "Name", "Field");

						if (scriptMetadata.Fields.contains(fieldID))
						{
							const auto& fieldMetadata = scriptMetadata.Fields.at(fieldID);
							auto& fieldStorage        = scene->GetScriptStorage().EntityStorage.at(id).Fields[fieldID];
							if (fieldStorage.IsArray())
							{
							}
							else
							{
								switch (fieldMetadata.Type)
								{
								case DataType::Byte: {
									fieldStorage.SetValue(TryDeserializeNode<u8>(field, "Value", 0));
								}
								break;
								case DataType::Short: {
									fieldStorage.SetValue(TryDeserializeNode<i16>(field, "Value", 0));
								}
								break;
								case DataType::UShort: {
									fieldStorage.SetValue(TryDeserializeNode<u16>(field, "Value", 0));
								}
								break;
								case DataType::Int: {
									fieldStorage.SetValue(TryDeserializeNode<i32>(field, "Value", 0));
								}
								break;
								case DataType::UInt: {
									fieldStorage.SetValue(TryDeserializeNode<u32>(field, "Value", 0));
								}
								break;
								case DataType::Long: {
									fieldStorage.SetValue(TryDeserializeNode<i64>(field, "Value", 0));
								}
								break;
								case DataType::ULong: {
									fieldStorage.SetValue(TryDeserializeNode<u64>(field, "Value", 0));
								}
								break;
								case DataType::Float: {
									fieldStorage.SetValue(TryDeserializeNode<f32>(field, "Value", 0.0f));
								}
								break;
								case DataType::Double: {
									fieldStorage.SetValue(TryDeserializeNode<f64>(field, "Value", 0.0));
								}
								break;
								case DataType::Bool: {
									fieldStorage.SetValue(TryDeserializeNode<bool>(field, "Value", false));
								}
								break;
								case DataType::Entity: {
									fieldStorage.SetValue(TryDeserializeNode<u64>(field, "Value", 0));
								}
								break;
								case DataType::Prefab: {
									fieldStorage.SetValue(TryDeserializeNode<u64>(field, "Value", 0));
								}
								break;
								default:
									break;
								}
							}
						}
					}
				}
			}

			if (YAML::Node cameraComponent = entity["Entity"]["CameraComponent"])
			{
				SceneCamera camera = {TryDeserializeNode<f32>(cameraComponent, "AspectRatio", 0.0f)};

				CameraComponent& cc = deserialized.AddComponent<CameraComponent>(camera);

				cc.Primary = TryDeserializeNode<bool>(cameraComponent, "Primary", false);
				cc.Camera.SetProjectionType((ProjectionType)TryDeserializeNode<int>(cameraComponent, "ProjectionType",
				                                                                    (int)ProjectionType::Orthographic));

				if (cc.Camera.GetProjectionType() == ProjectionType::Orthographic)
				{

					cc.Camera.SetOrthographicSize(TryDeserializeNode<f32>(cameraComponent, "OrthographicSize", 1.0f));
					cc.Camera.SetOrthographicNearClip(
					    TryDeserializeNode<f32>(cameraComponent, "OrthographicNearClip", -1.0f));
					cc.Camera.SetOrthographicFarClip(
					    TryDeserializeNode<f32>(cameraComponent, "OrthographicFarClip", 1.0f));
				}
				else if (cc.Camera.GetProjectionType() == ProjectionType::Perspective)
				{

					cc.Camera.SetPerspectiveVerticalFOV(
					    TryDeserializeNode<f32>(cameraComponent, "PerspectiveVerticalFOV", 45.0f));
					cc.Camera.SetPerspectiveNearClip(
					    TryDeserializeNode<f32>(cameraComponent, "PerspectiveNearClip", 0.01f));
					cc.Camera.SetPerspectiveFarClip(
					    TryDeserializeNode<f32>(cameraComponent, "PerspectiveFarClip", 100.0f));
				}
			}

			if (YAML::Node rigidBody2DComponent = entity["Entity"]["RigidBody2DComponent"])
			{
				RigidBody2DComponent& rbc = deserialized.AddComponent<RigidBody2DComponent>();

				rbc.Type =
				    (PhysicBodyType)TryDeserializeNode<int>(rigidBody2DComponent, "Type", (int)PhysicBodyType::Static);
				rbc.AutoMass             = TryDeserializeNode<bool>(rigidBody2DComponent, "AutoMass", true);
				rbc.Mass                 = TryDeserializeNode<f32>(rigidBody2DComponent, "Mass", 1.0f);
				rbc.GravityScale         = TryDeserializeNode<f32>(rigidBody2DComponent, "GravityScale", 1.0f);
				rbc.Friction             = TryDeserializeNode<f32>(rigidBody2DComponent, "Friction", 0.5f);
				rbc.Restitution          = TryDeserializeNode<f32>(rigidBody2DComponent, "Restitution", 0.0f);
				rbc.RestitutionThreshold = TryDeserializeNode<f32>(rigidBody2DComponent, "RestitutionThreshold", 0.5f);
				rbc.LinearDamping        = TryDeserializeNode<f32>(rigidBody2DComponent, "LinearDamping", 0.0f);
				rbc.AngularDamping       = TryDeserializeNode<f32>(rigidBody2DComponent, "AngularDamping", 0.0f);
				rbc.FixedRotation        = TryDeserializeNode<bool>(rigidBody2DComponent, "FixedRotation", false);
				rbc.AllowSleep           = TryDeserializeNode<bool>(rigidBody2DComponent, "AllowSleep", true);
				rbc.InitiallyAwake       = TryDeserializeNode<bool>(rigidBody2DComponent, "InitiallyAwake", true);
				rbc.IsBullet             = TryDeserializeNode<bool>(rigidBody2DComponent, "IsBullet", false);
			}

			if (YAML::Node boxCollider2DComponent = entity["Entity"]["BoxCollider2DComponent"])
			{
				BoxCollider2DComponent& bcc = deserialized.AddComponent<BoxCollider2DComponent>();

				bcc.Size     = TryDeserializeNode<glm::vec2>(boxCollider2DComponent, "Size", glm::vec2(1.0f));
				bcc.Offset   = TryDeserializeNode<glm::vec2>(boxCollider2DComponent, "Offset", glm::vec2(0.0f));
				bcc.Density  = TryDeserializeNode<f32>(boxCollider2DComponent, "Density", 1.0f);
				bcc.IsSensor = TryDeserializeNode<bool>(boxCollider2DComponent, "IsSensor", false);
			}

			if (YAML::Node circleCollider2DComponent = entity["Entity"]["CircleCollider2DComponent"])
			{
				CircleCollider2DComponent& ccc = deserialized.AddComponent<CircleCollider2DComponent>();

				ccc.Radius   = TryDeserializeNode<f32>(circleCollider2DComponent, "Radius", 0.5f);
				ccc.Offset   = TryDeserializeNode<glm::vec2>(circleCollider2DComponent, "Offset", glm::vec2(0.0f));
				ccc.Density  = TryDeserializeNode<f32>(circleCollider2DComponent, "Density", 1.0f);
				ccc.IsSensor = TryDeserializeNode<bool>(circleCollider2DComponent, "IsSensor", false);
			}

			if (YAML::Node polygonCollider2DComponent = entity["Entity"]["PolygonCollider2DComponent"])
			{
				PolygonCollider2DComponent& pcc = deserialized.AddComponent<PolygonCollider2DComponent>();

				YAML::Node vertices = polygonCollider2DComponent["Vertices"];
				u64 count           = TryDeserializeNode<u64>(polygonCollider2DComponent, "VerticesCount", 0);

				pcc.Vertices.clear();

				for (u64 i = 0; i < count; ++i)
				{
					pcc.Vertices.push_back(TryDeserializeNode<glm::vec2>(vertices, std::to_string(i), glm::vec2(0.0f)));
				}

				pcc.Offset   = TryDeserializeNode<glm::vec2>(polygonCollider2DComponent, "Offset", glm::vec2(0.0f));
				pcc.Density  = TryDeserializeNode<f32>(polygonCollider2DComponent, "Density", 1.0f);
				pcc.IsSensor = TryDeserializeNode<bool>(polygonCollider2DComponent, "IsSensor", false);
			}

			if (YAML::Node buoyancyEffector2DComponent = entity["Entity"]["BuoyancyEffector2DComponent"])
			{
				BuoyancyEffector2DComponent& bec = deserialized.AddComponent<BuoyancyEffector2DComponent>();

				bec.DragMultiplier = TryDeserializeNode<f32>(buoyancyEffector2DComponent, "DragMultiplier", 1.0f);
				bec.FlowAngle      = TryDeserializeNode<f32>(buoyancyEffector2DComponent, "FlowAngle", 0.0f);
				bec.FlowMagnitude  = TryDeserializeNode<f32>(buoyancyEffector2DComponent, "FlowMagnitude", 0.0f);
				bec.Density        = TryDeserializeNode<f32>(buoyancyEffector2DComponent, "Density", 2.0f);
			}

			if (YAML::Node distanceJoint2DComponent = entity["Entity"]["DistanceJoint2DComponent"])
			{
				DistanceJoint2DComponent& djc = deserialized.AddComponent<DistanceJoint2DComponent>();

				djc.ConnectedEntityID = TryDeserializeNode<u64>(distanceJoint2DComponent, "ConnectedEntityID", 0);
				djc.EnableCollision   = TryDeserializeNode<bool>(distanceJoint2DComponent, "EnableCollision", false);
				djc.AutoLength        = TryDeserializeNode<bool>(distanceJoint2DComponent, "AutoLength", true);
				djc.OriginAnchor =
				    TryDeserializeNode<glm::vec2>(distanceJoint2DComponent, "OriginAnchor", glm::vec2(0.0f));
				djc.ConnectedAnchor =
				    TryDeserializeNode<glm::vec2>(distanceJoint2DComponent, "ConnectedAnchor", glm::vec2(0.0f));
				djc.Length        = TryDeserializeNode<f32>(distanceJoint2DComponent, "Length", 0.0f);
				djc.MinLength     = TryDeserializeNode<f32>(distanceJoint2DComponent, "MinLength", 0.0f);
				djc.MaxLength     = TryDeserializeNode<f32>(distanceJoint2DComponent, "MaxLength", 1.0f);
				djc.BreakingForce = TryDeserializeNode<f32>(distanceJoint2DComponent, "BreakingForce", FLT_MAX);
			}

			if (YAML::Node revolutionJoint2DComponent = entity["Entity"]["RevolutionJoint2DComponent"])
			{
				RevolutionJoint2DComponent& rjc = deserialized.AddComponent<RevolutionJoint2DComponent>();

				rjc.ConnectedEntityID = TryDeserializeNode<u64>(revolutionJoint2DComponent, "ConnectedEntityID", 0);
				rjc.OriginAnchor =
				    TryDeserializeNode<glm::vec2>(revolutionJoint2DComponent, "OriginAnchor", glm::vec2(0.0f));
				rjc.LowerAngle      = TryDeserializeNode<f32>(revolutionJoint2DComponent, "LowerAngle", 0.0f);
				rjc.UpperAngle      = TryDeserializeNode<f32>(revolutionJoint2DComponent, "UpperAngle", 359.0f);
				rjc.MotorSpeed      = TryDeserializeNode<f32>(revolutionJoint2DComponent, "MotorSpeed", 5.0f);
				rjc.MaxMotorTorque  = TryDeserializeNode<f32>(revolutionJoint2DComponent, "MaxMotorTorque", 10000.0f);
				rjc.BreakingForce   = TryDeserializeNode<f32>(revolutionJoint2DComponent, "BreakingForce", FLT_MAX);
				rjc.BreakingTorque  = TryDeserializeNode<f32>(revolutionJoint2DComponent, "BreakingTorque", FLT_MAX);
				rjc.EnableLimit     = TryDeserializeNode<bool>(revolutionJoint2DComponent, "EnableLimit", false);
				rjc.EnableMotor     = TryDeserializeNode<bool>(revolutionJoint2DComponent, "EnableMotor", false);
				rjc.EnableCollision = TryDeserializeNode<bool>(revolutionJoint2DComponent, "EnableCollision", false);
			}

			if (YAML::Node prismaticJoint2DComponent = entity["Entity"]["PrismaticJoint2DComponent"])
			{
				PrismaticJoint2DComponent& pjc = deserialized.AddComponent<PrismaticJoint2DComponent>();

				pjc.ConnectedEntityID = TryDeserializeNode<u64>(prismaticJoint2DComponent, "ConnectedEntityID", 0);
				pjc.OriginAnchor =
				    TryDeserializeNode<glm::vec2>(prismaticJoint2DComponent, "OriginAnchor", glm::vec2(0.0f));
				pjc.Angle            = TryDeserializeNode<f32>(prismaticJoint2DComponent, "Angle", 0.0f);
				pjc.LowerTranslation = TryDeserializeNode<f32>(prismaticJoint2DComponent, "LowerTranslation", 0.0f);
				pjc.UpperTranslation = TryDeserializeNode<f32>(prismaticJoint2DComponent, "UpperTranslation", 0.0f);
				pjc.MotorSpeed       = TryDeserializeNode<f32>(prismaticJoint2DComponent, "MotorSpeed", 5.0f);
				pjc.MaxMotorForce    = TryDeserializeNode<f32>(prismaticJoint2DComponent, "MaxMotorForce", 20.0f);
				pjc.BreakingForce    = TryDeserializeNode<f32>(prismaticJoint2DComponent, "BreakingForce", FLT_MAX);
				pjc.BreakingTorque   = TryDeserializeNode<f32>(prismaticJoint2DComponent, "BreakingTorque", FLT_MAX);
				pjc.EnableLimit      = TryDeserializeNode<bool>(prismaticJoint2DComponent, "EnableLimit", false);
				pjc.EnableMotor      = TryDeserializeNode<bool>(prismaticJoint2DComponent, "EnableMotor", false);
				pjc.EnableCollision  = TryDeserializeNode<bool>(prismaticJoint2DComponent, "EnableCollision", false);
			}

			if (YAML::Node springJoint2DComponent = entity["Entity"]["SpringJoint2DComponent"])
			{
				SpringJoint2DComponent& sjc = deserialized.AddComponent<SpringJoint2DComponent>();

				sjc.ConnectedEntityID = TryDeserializeNode<u64>(springJoint2DComponent, "ConnectedEntityID", 0);
				sjc.EnableCollision   = TryDeserializeNode<bool>(springJoint2DComponent, "EnableCollision", false);
				sjc.AutoLength        = TryDeserializeNode<bool>(springJoint2DComponent, "AutoLength", true);
				sjc.OriginAnchor =
				    TryDeserializeNode<glm::vec2>(springJoint2DComponent, "OriginAnchor", glm::vec2(0.0f));
				sjc.ConnectedAnchor =
				    TryDeserializeNode<glm::vec2>(springJoint2DComponent, "ConnectedAnchor", glm::vec2(0.0f));
				sjc.Length        = TryDeserializeNode<f32>(springJoint2DComponent, "Length", 0.0f);
				sjc.MinLength     = TryDeserializeNode<f32>(springJoint2DComponent, "MinLength", 0.0f);
				sjc.MaxLength     = TryDeserializeNode<f32>(springJoint2DComponent, "MaxLength", 1.0f);
				sjc.BreakingForce = TryDeserializeNode<f32>(springJoint2DComponent, "BreakingForce", FLT_MAX);
				sjc.Frequency     = TryDeserializeNode<f32>(springJoint2DComponent, "Frequency", 4.0f);
				sjc.DampingRatio  = TryDeserializeNode<f32>(springJoint2DComponent, "DampingRatio", 0.5f);
			}

			if (YAML::Node wheelJoint2DComponent = entity["Entity"]["WheelJoint2DComponent"])
			{
				WheelJoint2DComponent& wjc = deserialized.AddComponent<WheelJoint2DComponent>();

				wjc.ConnectedEntityID = TryDeserializeNode<u64>(wheelJoint2DComponent, "ConnectedEntityID", 0);
				wjc.OriginAnchor =
				    TryDeserializeNode<glm::vec2>(wheelJoint2DComponent, "OriginAnchor", glm::vec2(0.0f));
				wjc.LowerTranslation = TryDeserializeNode<f32>(wheelJoint2DComponent, "LowerTranslation", -0.25f);
				wjc.UpperTranslation = TryDeserializeNode<f32>(wheelJoint2DComponent, "UpperTranslation", 0.25f);
				wjc.MotorSpeed       = TryDeserializeNode<f32>(wheelJoint2DComponent, "MotorSpeed", 10.0f);
				wjc.MaxMotorTorque   = TryDeserializeNode<f32>(wheelJoint2DComponent, "MaxMotorTorque", 20.0f);
				wjc.BreakingForce    = TryDeserializeNode<f32>(wheelJoint2DComponent, "BreakingForce", FLT_MAX);
				wjc.BreakingTorque   = TryDeserializeNode<f32>(wheelJoint2DComponent, "BreakingTorque", FLT_MAX);
				wjc.Frequency        = TryDeserializeNode<f32>(wheelJoint2DComponent, "Frequency", 4.0f);
				wjc.DampingRatio     = TryDeserializeNode<f32>(wheelJoint2DComponent, "DampingRatio", 0.7f);
				wjc.EnableLimit      = TryDeserializeNode<bool>(wheelJoint2DComponent, "EnableLimit", true);
				wjc.EnableMotor      = TryDeserializeNode<bool>(wheelJoint2DComponent, "EnableMotor", true);
				wjc.EnableCollision  = TryDeserializeNode<bool>(wheelJoint2DComponent, "EnableCollision", false);
			}

			if (YAML::Node audioSourceComponent = entity["Entity"]["AudioSourceComponent"])
			{
				AudioSourceComponent& asc = deserialized.AddComponent<AudioSourceComponent>();

				asc.Handle       = TryDeserializeNode<AssetHandle>(audioSourceComponent, "AudioHandle", 0);
				asc.Volume       = TryDeserializeNode<f32>(audioSourceComponent, "Volume", 1.0f);
				asc.Pitch        = TryDeserializeNode<f32>(audioSourceComponent, "Pitch", 1.0f);
				asc.Looping      = TryDeserializeNode<bool>(audioSourceComponent, "Looping", false);
				asc.PlayOnCreate = TryDeserializeNode<bool>(audioSourceComponent, "PlayOnCreate", false);
				asc.Is3D         = TryDeserializeNode<bool>(audioSourceComponent, "Is3D", false);
				asc.Attenuation  = (AttenuationType)TryDeserializeNode<int>(audioSourceComponent, "Attenuation",
                                                                           (int)AttenuationType::None);
				asc.RollOff      = TryDeserializeNode<f32>(audioSourceComponent, "RollOff", 1.0f);
				asc.MinGain      = TryDeserializeNode<f32>(audioSourceComponent, "MinGain", 0.0f);
				asc.MaxGain      = TryDeserializeNode<f32>(audioSourceComponent, "MaxGain", 1.0f);
				asc.MinDistance  = TryDeserializeNode<f32>(audioSourceComponent, "MinDistance", 0.3f);
				asc.MaxDistance  = TryDeserializeNode<f32>(audioSourceComponent, "MaxDistance", 100.0f);
				// asc.ConeInnerAngle = TryDeserializeNode<f32>(audioSourceComponent, "ConeInnerAngle",
				// glm::radians(360.0f)); asc.ConeOuterAngle = TryDeserializeNode<f32>(audioSourceComponent,
				// "ConeOuterAngle", glm::radians(360.0f)); asc.ConeOuterGain =
				// TryDeserializeNode<f32>(audioSourceComponent, "ConeOuterGain", 0.0f);
				asc.DopplerFactor = TryDeserializeNode<f32>(audioSourceComponent, "DopplerFactor", 1.0f);
			}

			if (YAML::Node audioListenerComponent = entity["Entity"]["AudioListenerComponent"])
			{
				// AudioListenerComponent& alc = deserialized.AddComponent<AudioListenerComponent>();

				// alc.ConeInnerAngle = TryDeserializeNode<f32>(audioListenerComponent, "ConeInnerAngle",
				// glm::radians(360.0f)); alc.ConeOuterAngle = TryDeserializeNode<f32>(audioListenerComponent,
				// "ConeOuterAngle", glm::radians(360.0f)); alc.ConeOuterGain =
				// TryDeserializeNode<f32>(audioListenerComponent, "ConeOuterGain", 0.0f);
			}
		}
	}

} // namespace SW
