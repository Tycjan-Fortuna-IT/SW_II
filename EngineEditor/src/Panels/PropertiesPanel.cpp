#include "PropertiesPanel.hpp"

#include "Managers/SelectionManager.hpp"
#include "Core/ECS/Components.hpp"
#include "GUI/Icons.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Scene/SceneCamera.hpp"
#include "Core/Scripting/ScriptingCore.hpp"
#include "Core/Scripting/ScriptStorage.hpp"
#include "Asset/Sprite.hpp"
#include "Asset/Prefab.hpp"
#include "GUI/GUI.hpp"
#include "Audio/Sound.hpp"

namespace SW {

	PropertiesPanel::PropertiesPanel(SceneViewportPanel* sceneViewportPanel)
		: Panel("Properties", SW_ICON_INFORMATION, true), m_SceneViewportPanel(sceneViewportPanel)
	{
		AddComponentName<TagComponent>(SW_ICON_TAG "  Tag");
		
		AddComponentName<TransformComponent>(SW_ICON_VECTOR_LINE "  Transform");
		AddComponentName<SpriteComponent>(SW_ICON_IMAGE_SIZE_SELECT_ACTUAL "  Sprite");
		AddComponentName<AnimatedSpriteComponent>(SW_ICON_VIDEO "  Animated Sprite");
		AddComponentName<CircleComponent>(SW_ICON_CHECKBOX_BLANK_CIRCLE "  Circle");
		AddComponentName<TextComponent>(SW_ICON_FORMAT_TEXT "  Text");
		
		AddComponentName<CameraComponent>(SW_ICON_CAMERA "  Camera");

		AddComponentName<ScriptComponent>(SW_ICON_LANGUAGE_CSHARP " Script");

		AddComponentName<RigidBody2DComponent>(SW_ICON_SOCCER "  Rigid Body 2D");
		AddComponentName<BoxCollider2DComponent>(SW_ICON_CHECKBOX_BLANK_OUTLINE "  Box Collider 2D");
		AddComponentName<CircleCollider2DComponent>(SW_ICON_CHECKBOX_BLANK_CIRCLE_OUTLINE "  Circle Collider 2D");
		AddComponentName<PolygonCollider2DComponent>(SW_ICON_VECTOR_POLYGON " Polygon Collider 2D");
		AddComponentName<BuoyancyEffector2DComponent>(SW_ICON_WATER "  Buoyancy Effector 2D");

		AddComponentName<DistanceJoint2DComponent>(SW_ICON_VECTOR_LINE "  Distance Joint 2D");
		AddComponentName<RevolutionJoint2DComponent>(SW_ICON_ANGLE_ACUTE "  Revolution Joint 2D");
		AddComponentName<PrismaticJoint2DComponent>(SW_ICON_VIEW_AGENDA "  Prismatic Joint 2D");
		AddComponentName<SpringJoint2DComponent>(SW_ICON_ARROW_EXPAND " Spring Joint 2D");
		AddComponentName<WheelJoint2DComponent>(SW_ICON_CAR " Wheel Joint 2D");

		AddComponentName<AudioSourceComponent>(SW_ICON_VOLUME_HIGH "  Audio Source");
		AddComponentName<AudioListenerComponent>(SW_ICON_MICROPHONE "  Audio Listener");

		m_ComponentCopyScene = new Scene();
		m_ComponentCopyEntity = m_ComponentCopyScene->CreateEntity();
	}

	PropertiesPanel::~PropertiesPanel()
	{
		delete m_ComponentCopyScene;
	}

	void PropertiesPanel::OnRender()
	{
		PROFILE_SCOPE();

		constexpr f32 MaxFloatValue = FLT_MAX / 2.f;

		if (OnBegin()) {
			if (!SelectionManager::IsSelected()) {
				OnEnd(); return;
			}

			Entity entity = m_SceneViewportPanel->GetCurrentScene()->TryGetEntityByID(SelectionManager::GetSelectionID());

			if (!entity) {
				OnEnd(); return;
			}

			if (ImGui::Button(SW_ICON_PLUS " Add", { 90.f, 30.f })) {
				ImGui::OpenPopup("AddComponent_Popup");
			}

			ImGui::SameLine();

			IDComponent& id = entity.GetComponent<IDComponent>();

			ImGui::Text(SW_ICON_KEY "  ID");

			ImGui::SameLine();

			ImGui::TextDisabled(std::to_string(id.ID).c_str());

			if (ImGui::BeginPopup("AddComponent_Popup")) {
				DrawAddComponentMenu(entity);

				ImGui::EndPopup();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 4.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 2.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.f);

			DrawComponent<TagComponent>(entity, [](TagComponent& component) {
				GUI::Properties::BeginProperties("##tag_property");
				GUI::Properties::SingleLineTextInputProperty<64>(&component.Tag, "Tag ");
				GUI::Properties::EndProperties();
			}, false);

			DrawComponent<TransformComponent>(entity, [](TransformComponent& component) {
				GUI::Properties::BeginProperties("##transform_property");
				GUI::Properties::Vector3InputProperty(&component.Position, "Position ", "Position of the entity");

				glm::vec3 rotation = glm::degrees(component.Rotation);
				GUI::Properties::Vector3InputProperty(&rotation, "Rotation ", "Rotation of the entity in degrees");
				component.Rotation = glm::radians(rotation);

				GUI::Properties::Vector3InputProperty(&component.Scale, "Scale ", "Scale of the entity", 1.f);
				GUI::Properties::EndProperties();
			}, false);

			DrawComponent<SpriteComponent>(entity, [](SpriteComponent& component) {
				GUI::Properties::BeginProperties("##sprite_property");
				GUI::Properties::Vector4ColorPickerProperty(&component.Color, "Color", "Color of the sprite");

				AssetHandle handle = component.Handle;
				if (GUI::Properties::AssetSearchProperty<Sprite>(&handle, "Sprite", "Sprite to be used (transparency is supported)")) {
					component.Handle = handle;
				}
				GUI::Properties::ScalarInputProperty<f32>(&component.TilingFactor, "Tiling", "Tiling factor of the texture (how many times the texture should be repeated)", 1.f, 5.f, 0.f, 20.f);
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<AnimatedSpriteComponent>(entity, [](AnimatedSpriteComponent& component) {
				GUI::Properties::BeginProperties("##sprite_property");

				AssetHandle handle = component.DefaultAnimation ? (*component.DefaultAnimation)->GetHandle() : 0u;
				if (GUI::Properties::AssetSearchProperty<Animation2D>(&handle, "Default", "Default animation to be used if nothing is being played (if not specified - nothing will be visible)")) {
					component.DefaultAnimation = AssetManager::GetAssetRaw<Animation2D>(handle);
					component.CurrentAnimation = component.DefaultAnimation;
				}
				GUI::Properties::AssetDropdownTableMapProperty<std::string, Animation2D>(&component.Animations, "Animations");

				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<CircleComponent>(entity, [](CircleComponent& component) {
				GUI::Properties::BeginProperties("##circle_property");
				GUI::Properties::Vector4ColorPickerProperty(&component.Color, "Color", "Color of the circle");
				GUI::Properties::ScalarSliderProperty<f32>(&component.Thickness, "Thickness", "Decreasing thickness will start to create a ring inside", 0.f, 1.f);
				GUI::Properties::ScalarSliderProperty<f32>(&component.Fade, "Fade", "How much the circle starts to fade from outside", 0.f, 4.f);
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<TextComponent>(entity, [](TextComponent& component) {
				GUI::Properties::BeginProperties("##text_property");

				AssetHandle handle = component.Handle;
				if (GUI::Properties::AssetSearchProperty<Font>(&handle, "Font", "Font to be used")) {
					component.Handle = handle;
				}
				if (component.Handle) {
					GUI::Properties::MultiLineTextInputProperty<500>(&component.TextString, "Text", "Text to display (max 500 characters)");
					GUI::Properties::Vector4ColorPickerProperty(&component.Color, "Color", "Color of the text");
					GUI::Properties::ScalarDragProperty<f32>(&component.Kerning, "Kerning", "The space between the characters", 1.f, 0.f);
					GUI::Properties::ScalarDragProperty<f32>(&component.LineSpacing, "Line Spacing", "The space between the lines", 1.f, 0.f);
				}
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<CameraComponent>(entity, [](CameraComponent& component) {
				GUI::Properties::BeginProperties("##camera_property");
				GUI::Properties::CheckboxProperty(&component.Primary, "Primary", "Whether this camera is the primary camera (only one camera can be primary)");

				ProjectionType type = component.Camera.GetProjectionType();

				if (
					GUI::Properties::SelectableProperty<ProjectionType>(&type, {
						{ "Perspective", ProjectionType::Perspective },
						{ "Orthographic", ProjectionType::Orthographic },
					}, "Projection Type")
				) {
					component.Camera.SetProjectionType(type);
				}

				if (type == ProjectionType::Orthographic) {

					f32 orthoSize = component.Camera.GetOrthographicSize();
					if (GUI::Properties::ScalarDragProperty<f32>(&orthoSize, "Size", "Size of the camera", 1.f, 0.f))
						component.Camera.SetOrthographicSize(orthoSize);

					f32 orthoNear = component.Camera.GetOrthographicNearClip();
					if (GUI::Properties::ScalarDragProperty<f32>(&orthoNear, "Near Clip", "The near clip of the camera", 1.f, 0.f))
						component.Camera.SetOrthographicNearClip(orthoNear);

					f32 orthoFar = component.Camera.GetOrthographicFarClip();
					if (GUI::Properties::ScalarDragProperty<f32>(&orthoFar, "Far Clip", "The far clip of the camera", 1.f, 0.f))
						component.Camera.SetOrthographicFarClip(orthoFar);

				} else if (type == ProjectionType::Perspective) {

					f32 verticalFov = glm::degrees(component.Camera.GetPerspectiveVerticalFOV());
					if (GUI::Properties::ScalarSliderProperty<f32>(&verticalFov, "Vertical FOV", "Vertical field of view of the camera in degrees", 0.f, 180.f))
						component.Camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

					f32 perspectiveNear = component.Camera.GetPerspectiveNearClip();
					if (GUI::Properties::ScalarDragProperty<f32>(&perspectiveNear, "Near Clip", "The near clip of the camera", 1.f, 0.f))
						component.Camera.SetPerspectiveNearClip(perspectiveNear);

					f32 perspectiveFar = component.Camera.GetPerspectiveFarClip();
					if (GUI::Properties::ScalarDragProperty<f32>(&perspectiveFar, "Far Clip", "The far clip of the camera", 1.f, 0.f))
						component.Camera.SetPerspectiveFarClip(perspectiveFar);

				}

				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<ScriptComponent>(entity, [this, id](ScriptComponent& component) {
				GUI::Properties::BeginProperties("##script_property");
				ScriptingCore& scriptingCore = ScriptingCore::Get();

				static bool once = false;

				const std::unordered_map<u64, ScriptMetadata>& allScripts = scriptingCore.GetAllScripts();

				std::vector<GUI::Components::SelectOption<u64>> scripts;

				scripts.push_back({ "Invalid or no script", 0 });

				for (auto& [scriptId, scriptMetaData] : allScripts) {
					scripts.push_back({ scriptMetaData.FullName, scriptId });
				}

				ScriptStorage& storage = m_SceneViewportPanel->GetCurrentScene()->GetScriptStorage();

				u64 currentId = component.ScriptID;

				if (GUI::Properties::SelectableProperty<u64>(&component.ScriptID, scripts, "Scripts")) {
					if (scriptingCore.IsValidScript(component.ScriptID)) {
						if (currentId != component.ScriptID) {
							if (currentId != 0) {
								storage.ShutdownEntityStorage(currentId, id.ID);
							}
							storage.InitializeEntityStorage(component.ScriptID, id.ID);
						}
					} else {
						storage.ShutdownEntityStorage(currentId, id.ID);
						component.ScriptID = 0;
					}
				}

				if (component.ScriptID) {
					std::unordered_map<u32, FieldStorage> fieldStorages = storage.EntityStorage.at(id.ID).Fields;
					const ScriptMetadata& scriptMetadata = scriptingCore.GetScriptMetadata(component.ScriptID);

					Coral::Type& reflectionHelper = scriptingCore.GetCoreAssembly()->Assembly->GetType("SW.ReflectionHelper");

					Coral::String assemblyName = Coral::String::New(scriptingCore.GetAppAssembly()->Assembly->GetName());
					Coral::String className = Coral::String::New(scriptMetadata.FullName);
					Coral::String serializeAttrName = Coral::String::New("SerializeFieldAttribute");

					for (auto& [fieldId, fieldStorage] : fieldStorages) {
						const FieldMetadata& fieldMetadata = scriptMetadata.Fields.at(fieldId);
						Coral::String fieldName = Coral::String::New(fieldMetadata.Name);

						bool isSerializable = (bool)reflectionHelper.InvokeStaticMethod<Coral::Bool32>(
							"HasFieldAttribute", assemblyName, className, fieldName, serializeAttrName
						);

						if (!isSerializable)
							continue;

						if (fieldStorage.IsArray()) {
							ASSERT(false, "Arrays not yet supported!");
						} else {
							DataType fieldType = fieldStorage.GetType();

							switch (fieldType) {
								case DataType::Byte: ASSERT(false, "Byte not yet supported!");
									break;
								case DataType::Short: ASSERT(false, "Short not yet supported!");
									break;
								case DataType::UShort: ASSERT(false, "UShort not yet supported!");
									break;
								case DataType::Int: ASSERT(false, "Int not yet supported!");
									break;
								case DataType::UInt: ASSERT(false, "UInt not yet supported!");
									break;
								case DataType::Long: ASSERT(false, "Long not yet supported!");
									break;
								case DataType::ULong: ASSERT(false, "ULong not yet supported!");
									break;
								case DataType::Float:
								{
									f32 value = fieldStorage.GetValue<f32>();
									
									if (GUI::Properties::ScalarInputProperty<f32>(&value, fieldStorage.GetName().data())) {
										fieldStorage.SetValue(value);
									}

									break;
								}
								case DataType::Double: ASSERT(false, "Double not yet supported!");
									break;
								case DataType::Bool: ASSERT(false, "Bool not yet supported!");
									break;
								case DataType::Entity:
								{
									if (!m_SceneViewportPanel->GetCurrentScene()->IsPlaying()) {
										u64 value = fieldStorage.GetValue<u64>();

										if (GUI::Properties::EntityDropdownProperty(&value, m_SceneViewportPanel->GetCurrentScene(), std::string(fieldStorage.GetName()).c_str(), "Entity to be used")) {
											fieldStorage.SetValue(value);
										}
									}
								} break;
								case DataType::Prefab:
								{
									if (!m_SceneViewportPanel->GetCurrentScene()->IsPlaying()) {
										AssetHandle value = fieldStorage.GetValue<u64>();
										
										if (GUI::Properties::AssetSearchProperty<Prefab>(&value, std::string(fieldStorage.GetName()).c_str(), "Prefab to be used")) {
											fieldStorage.SetValue(value);
										}
									}
								} break;
								default:
									SW_WARN("{}", (int)fieldType);
									break;
							}
						}
					}
				}

				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<RigidBody2DComponent>(entity, [](RigidBody2DComponent& component) {
				GUI::Properties::BeginProperties("##rigid_body_2d_property");
				GUI::Properties::SelectableProperty<PhysicBodyType>(&component.Type, {
					{ "Static", PhysicBodyType::Static },
					{ "Kinematic", PhysicBodyType::Kinematic },
					{ "Dynamic", PhysicBodyType::Dynamic }
				}, "Body Type", "The type of the body, static bodies are immovable, kinematic bodies are movable but not affected by forces, dynamic bodies are movable and affected by forces");
				if (component.Type == PhysicBodyType::Kinematic || component.Type == PhysicBodyType::Dynamic) {
					GUI::Properties::CheckboxProperty(&component.AutoMass, "Auto mass", "Whether the body's mass should be determined automatically or based on set mass");
					if (!component.AutoMass) {
						GUI::Properties::ScalarDragProperty<f32>(&component.Mass, "Mass", "The mass of the body", 1.f, 0.f);
					}
				}
				if (component.Type == PhysicBodyType::Dynamic) {
					GUI::Properties::ScalarInputProperty<f32>(&component.GravityScale, "Gravity Scale", "The gravity scale of the body", 1, 5);
					GUI::Properties::ScalarSliderProperty<f32>(&component.Friction, "Friction", "The friction of the body (how much it resists movement)", 0.f, 1.f);
					GUI::Properties::ScalarSliderProperty<f32>(&component.Restitution, "Restitution", "The restitution of the body (how much it bounces)", 0.f, 1.f);
					GUI::Properties::ScalarSliderProperty<f32>(&component.RestitutionThreshold, "Restitution Threshold", "The velocity threshold for the restitution", 0.f, 1.f);
					GUI::Properties::ScalarSliderProperty<f32>(&component.LinearDamping, "Linear Damping", "The linear damping of the body (how much it resists movement)", 0.f, 1.f);
					GUI::Properties::ScalarSliderProperty<f32>(&component.AngularDamping, "Angular Damping", "The angular damping of the body (how much it resists rotation)", 0.f, 1.f);
					GUI::Properties::CheckboxProperty(&component.FixedRotation, "Fixed Rotation", "Whether the body should not be rotated by the physics system");
					GUI::Properties::CheckboxProperty(&component.AllowSleep, "Allow Sleep", "Whether the body should be allowed to sleep");
					GUI::Properties::CheckboxProperty(&component.InitiallyAwake, "Initially Awake", "Whether the body should be allowed to sleep");
					GUI::Properties::CheckboxProperty(&component.IsBullet, "Is Bullet", "Is this a fast moving body that should be prevented from tunneling through other moving bodies?");
				}
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<BoxCollider2DComponent>(entity, [](BoxCollider2DComponent& component) {
				GUI::Properties::BeginProperties("##box_collider_2d_property");
				GUI::Properties::Vector2InputProperty(&component.Size, "Size", "Size of the collider", 0.5f);
				GUI::Properties::Vector2InputProperty(&component.Offset, "Offset", "Offset of the collider from origin");
				GUI::Properties::ScalarInputProperty<f32>(&component.Density, "Density", "Density of the collider (mass = density * area)", 1.f, 10.f, 0.f);
				GUI::Properties::CheckboxProperty(&component.IsSensor, "Is Sensor?", "Whether to react to the collision or just sense it.");
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<CircleCollider2DComponent>(entity, [](CircleCollider2DComponent& component) {
				GUI::Properties::BeginProperties("##circle_collider_2d_property");
				GUI::Properties::ScalarInputProperty<f32>(&component.Radius, "Radius", "Radius of the collider", 1.f, 5.f, 0.1f);
				GUI::Properties::Vector2InputProperty(&component.Offset, "Offset", "Offset of the collider from origin");
				GUI::Properties::ScalarInputProperty<f32>(&component.Density, "Density", "Density of the collider (mass = density * area)", 1.f, 10.f, 0.f);
				GUI::Properties::CheckboxProperty(&component.IsSensor, "Is Sensor?", "Whether to react to the collision or just sense it.");
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<PolygonCollider2DComponent>(entity, [](PolygonCollider2DComponent& component) {
				GUI::Properties::BeginProperties("##polygon_collider_2d_property");
				GUI::Properties::Vector2TableProperty(&component.Vertices, "Edges", "List of all of the edges of the polygon collider (minimum 3 edges!)");
				GUI::Properties::Vector2InputProperty(&component.Offset, "Offset", "Offset of the collider from origin");
				GUI::Properties::ScalarInputProperty<f32>(&component.Density, "Density", "Density of the collider (mass = density * area)", 1.f, 10.f, 0.f);
				GUI::Properties::CheckboxProperty(&component.IsSensor, "Is Sensor?", "Whether to react to the collision or just sense it.");
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<BuoyancyEffector2DComponent>(entity, [](BuoyancyEffector2DComponent& component) {
				GUI::Properties::BeginProperties("##buoancy_effector_2d_property");
				GUI::Properties::ScalarDragProperty<f32>(&component.DragMultiplier, "Drag Multiplier", "The drag multiplier of the effector (how much it resists movement)");
				GUI::Properties::ScalarDragProperty<f32>(&component.FlowMagnitude, "Flow Magnitude", "The magnitude of the flow force in Newtons");
				f32 angle = glm::degrees(component.FlowAngle);
				GUI::Properties::ScalarDragProperty<f32>(&angle, "Flow Angle", "The angle of the flow force in degrees");
				component.FlowAngle = glm::radians(angle);
				GUI::Properties::ScalarInputProperty<f32>(&component.Density, "Density", "Density of the collider (mass = density * area)", 1.f, 10.f, 0.f);
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<DistanceJoint2DComponent>(entity, [this](DistanceJoint2DComponent& component) {
				GUI::Properties::BeginProperties("##distance_joint_2d_property");
				GUI::Properties::EntityDropdownProperty(&component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::Properties::Vector2InputProperty(&component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::Properties::Vector2InputProperty(&component.ConnectedAnchor, "Connected Anchor", "The anchor point of the connected body");
				GUI::Properties::ScalarDragProperty<f32>(&component.BreakingForce, "Breaking Force", "The force acting on the joint to break it in Newtons", 1.f, 0.f, MaxFloatValue);
				GUI::Properties::CheckboxProperty(&component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::Properties::CheckboxProperty(&component.AutoLength, "Auto length", "Whether the distance should be automatically calculated");
				if (!component.AutoLength) {
					GUI::Properties::ScalarDragProperty<f32>(&component.Length, "Length", "The distance between the two bodies", 1.f, 0.f);
				}
				GUI::Properties::ScalarDragProperty<f32>(&component.MinLength, "Min Length", "The minimum distance between the two bodies", 1.f, 0.f, component.MaxLength);
				GUI::Properties::ScalarDragProperty<f32>(&component.MaxLength, "Max Length", "The maximum distance between the two bodies", 1.f, component.MinLength);
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<RevolutionJoint2DComponent>(entity, [this](RevolutionJoint2DComponent& component) {
				GUI::Properties::BeginProperties("##revolution_joint_2d_property");
				GUI::Properties::EntityDropdownProperty(&component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::Properties::Vector2InputProperty(&component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::Properties::CheckboxProperty(&component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::Properties::CheckboxProperty(&component.EnableLimit, "Limit Enabled", "Whether the joint should have a limit");
				if (component.EnableLimit) {
					f32 lowerAngle = glm::degrees(component.LowerAngle);
					GUI::Properties::ScalarDragProperty<f32>(&lowerAngle, "Lower Angle", "The lower angle for the joint limit (in degrees)");
					component.LowerAngle = glm::radians(lowerAngle);

					f32 upperAngle = glm::degrees(component.UpperAngle);
					GUI::Properties::ScalarDragProperty<f32>(&upperAngle, "Upper Angle", "The upper angle for the joint limit (in degrees)");
					component.UpperAngle = glm::radians(upperAngle);
				}
				GUI::Properties::CheckboxProperty(&component.EnableMotor, "Enable Motor", "Whether the joint should have a motor");
				if (component.EnableMotor) {
					GUI::Properties::ScalarDragProperty<f32>(&component.MotorSpeed, "Motor Speed", "The speed of the motor in radians per second");
					GUI::Properties::ScalarDragProperty<f32>(&component.MaxMotorTorque, "Max Motor Torque", "The maximum torque of the motor in Newtons");
				}
				GUI::Properties::ScalarDragProperty<f32>(&component.BreakingTorque, "Breaking Torque", "The torque acting on the joint to break it in Newtons", 1.f, 0.f);
				GUI::Properties::ScalarDragProperty<f32>(&component.BreakingForce, "Breaking Force", "The force acting on the joint to break it in Newtons", 1.f, 0.f);

				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<PrismaticJoint2DComponent>(entity, [this](PrismaticJoint2DComponent& component) {
				GUI::Properties::BeginProperties("##prismatic_joint_2d_property");
				GUI::Properties::EntityDropdownProperty(&component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::Properties::Vector2InputProperty(&component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::Properties::CheckboxProperty(&component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::Properties::CheckboxProperty(&component.EnableLimit, "Limit Enabled", "Whether the joint should have a limit");
				if (component.EnableLimit) {
					f32 angle = glm::degrees(component.Angle);
					GUI::Properties::ScalarDragProperty<f32>(&angle, "Angle", "The constrained angle between the bodies (in degrees)");
					component.Angle = glm::radians(angle);
					GUI::Properties::ScalarDragProperty<f32>(&component.LowerTranslation, "Lower Translation");
					GUI::Properties::ScalarDragProperty<f32>(&component.UpperTranslation, "Upper Translation");
				}
				GUI::Properties::CheckboxProperty(&component.EnableMotor, "Enable Motor", "Whether the joint should have a motor");
				if (component.EnableMotor) {
					GUI::Properties::ScalarDragProperty<f32>(&component.MotorSpeed, "Motor Speed", "The speed of the motor in radians per second", 1.f, 0.f);
					GUI::Properties::ScalarDragProperty<f32>(&component.MaxMotorForce, "Max Motor Force", "The maximum force of the motor in Newtons", 1.f, 0.f);
				}
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<SpringJoint2DComponent>(entity, [this](SpringJoint2DComponent& component) {
				GUI::Properties::BeginProperties("##spring_joint_2d_property");
				GUI::Properties::EntityDropdownProperty(&component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::Properties::Vector2InputProperty(&component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::Properties::Vector2InputProperty(&component.ConnectedAnchor, "Connected Anchor", "The anchor point of the connected body");
				GUI::Properties::ScalarDragProperty<f32>(&component.BreakingForce, "Breaking Force", "The force acting on the joint to break it in Newtons", 1.f, 0.f);
				GUI::Properties::CheckboxProperty(&component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::Properties::CheckboxProperty(&component.AutoLength, "Auto length", "Whether the distance should be automatically calculated");
				if (!component.AutoLength) {
					GUI::Properties::ScalarDragProperty<f32>(&component.Length, "Length", "The distance between the two bodies", 1.f, 0.f);
				}
				GUI::Properties::ScalarDragProperty<f32>(&component.MinLength, "Min Length", "The minimum distance between the two bodies", 1.f, 0.f, component.MaxLength);
				GUI::Properties::ScalarDragProperty<f32>(&component.MaxLength, "Max Length", "The maximum distance between the two bodies", 1.f, component.MinLength, MaxFloatValue);
				GUI::Properties::ScalarDragProperty<f32>(&component.Frequency, "Frequency", "The linear stiffness in N/m.");
				GUI::Properties::ScalarSliderProperty(&component.DampingRatio, "Damping Ratio", "The linear damping in N*s/m.", 0.0f, 1.0f);
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<WheelJoint2DComponent>(entity, [this](WheelJoint2DComponent& component) {
				GUI::Properties::BeginProperties("##wheel_joint_2d_property");
				GUI::Properties::EntityDropdownProperty(&component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::Properties::Vector2InputProperty(&component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::Properties::CheckboxProperty(&component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::Properties::CheckboxProperty(&component.EnableLimit, "Limit Enabled", "Whether the joint should have a limit");
				if (component.EnableLimit) {
					GUI::Properties::ScalarDragProperty<f32>(&component.LowerTranslation, "Lower Translation");
					GUI::Properties::ScalarDragProperty<f32>(&component.UpperTranslation, "Upper Translation");
				}
				GUI::Properties::CheckboxProperty(&component.EnableMotor, "Enable Motor", "Whether the joint should have a motor");
				if (component.EnableMotor) {
					GUI::Properties::ScalarDragProperty<f32>(&component.MotorSpeed, "Motor Speed", "The speed of the motor in radians per second");
					GUI::Properties::ScalarDragProperty<f32>(&component.MaxMotorTorque, "Max Motor Torque", "The maximum torque of the motor in Newtons");
				}
				GUI::Properties::ScalarDragProperty<f32>(&component.Frequency, "Frequency", "The linear stiffness in N/m.");
				GUI::Properties::ScalarDragProperty<f32>(&component.DampingRatio, "Damping Ratio", "The linear damping in N*s/m.", 1.f, 0.0f, 1.0f);
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<AudioSourceComponent>(entity, [&](AudioSourceComponent& component) {
				GUI::Properties::BeginProperties("##audio_source_property");
				
				GUI::Properties::AssetSearchProperty<Sound>(&component.Handle, "Clip", "The audio sound to be played");
				GUI::Properties::ScalarSliderProperty<f32>(&component.Volume, "Volume", "The volume of the audio source", 0.f, 2.f);
				GUI::Properties::ScalarSliderProperty<f32>(&component.Pitch, "Pitch", "The pitch of the audio source", 0.f, 2.f);
				GUI::Properties::CheckboxProperty(&component.Looping, "Looping", "Whether the audio source should loop");
				GUI::Properties::CheckboxProperty(&component.PlayOnCreate, "Play On Create", "Whether the audio source should play on entity creation");
				GUI::Properties::CheckboxProperty(&component.Is3D, "3D", "Whether the audio source should be 3D");

				if (component.Is3D) {
					GUI::Properties::SelectableProperty<AttenuationType>(&component.Attenuation, {
						{ "None", AttenuationType::None },
						{ "Linear", AttenuationType::Linear },
						{ "Inverse", AttenuationType::Inverse },
						{ "Exponential", AttenuationType::Exponential }
					}, "Attenuation Type", "The type of attenuation to be used for the audio source");

					GUI::Properties::ScalarSliderProperty<f32>(&component.RollOff, "Roll Off", "The roll off factor of the audio source (how quickly the sound fades)", 0.f, 10.f);
					GUI::Properties::ScalarSliderProperty<f32>(&component.MinGain, "Min Gain", "The minimum gain of the audio source", 0.f, 1.f);
					GUI::Properties::ScalarSliderProperty<f32>(&component.MaxGain, "Max Gain", "The maximum gain of the audio source", 0.f, 1.f);
					GUI::Properties::ScalarSliderProperty<f32>(&component.MinDistance, "Min Distance", "The minimum distance of the audio source (where the sound starts to fade)", 0.f, 1000.f);
					GUI::Properties::ScalarSliderProperty<f32>(&component.MaxDistance, "Max Distance", "The maximum distance of the audio source (where the sound is inaudible)", 0.f, 1000.f);
				
					f32 coneInnerAngle = glm::degrees(component.ConeInnerAngle);
					if (GUI::Properties::ScalarDragProperty<f32>(&coneInnerAngle, "Cone Inner Angle", "The inner angle of the cone in degrees", 1.f, 0.f, 360.f)) {
						component.ConeInnerAngle = glm::radians(coneInnerAngle);
					}

					f32 coneOuterAngle = glm::degrees(component.ConeOuterAngle);
					if (GUI::Properties::ScalarDragProperty<f32>(&coneOuterAngle, "Cone Outer Angle", "The outer angle of the cone in degrees", 1.f, 0.f, 360.f)) {
						component.ConeOuterAngle = glm::radians(coneOuterAngle);
					}

					GUI::Properties::ScalarSliderProperty<f32>(&component.ConeOuterGain, "Cone Outer Gain", "The gain of the outer cone", 0.f, 1.f);
					GUI::Properties::ScalarSliderProperty<f32>(&component.DopplerFactor, "Doppler Factor", "The Doppler factor of the audio source", 0.f, 10.f);
				}
				
				GUI::Properties::BeginPropertyGrid("Control", "Start, Stop, Pause, Resume the audio source");
				if (component.Handle) {
					if (ImGui::Button("Play")) {
						if (component.Instance && *component.Instance) {
							(*component.Instance)->Stop();
						}

						Sound* sound = *AssetManager::GetAssetRaw<Sound>(component.Handle);

						SoundSpecification spec;
						spec.Sound = sound;
						spec.Pitch = component.Pitch;
						spec.Volume = component.Volume;

						if (component.Is3D) {
							spec.Looping = component.Looping;
							spec.Is3D = component.Is3D;
							spec.Attenuation = component.Attenuation;
							spec.RollOff = component.RollOff;
							spec.MinGain = component.MinGain;
							spec.MaxGain = component.MaxGain;
							spec.MinDistance = component.MinDistance;
							spec.MaxDistance = component.MaxDistance;
							spec.ConeInnerAngle = component.ConeInnerAngle;
							spec.ConeOuterAngle = component.ConeOuterAngle;
							spec.ConeOuterGain = component.ConeOuterGain;
							spec.DopplerFactor = component.DopplerFactor;

							const glm::mat4 invertedTransform = entity.GetWorldSpaceTransformMatrix();
							const glm::vec3 forward = glm::normalize(glm::vec3(invertedTransform * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));

							TransformComponent& tc = entity.GetComponent<TransformComponent>();
							component.Instance = AudioEngine::PlaySound3D(spec, tc.Position, forward);
						} else {
							component.Instance = AudioEngine::PlaySound(spec);
						}
					}

					if (component.Instance && *component.Instance) {
						ImGui::SameLine();
						if (ImGui::Button("Stop")) {
							(*component.Instance)->Stop();
						}
					}
				}
				GUI::Properties::EndPropertyGrid();
				
				GUI::Properties::EndProperties();
			}, true);

			DrawComponent<AudioListenerComponent>(entity, [](AudioListenerComponent& component) {
				GUI::Properties::BeginProperties("##audio_listener_property");

				f32 coneInnerAngle = glm::degrees(component.ConeInnerAngle);
				if (GUI::Properties::ScalarDragProperty<f32>(&coneInnerAngle, "Cone Inner Angle", "The inner angle of the cone in degrees", 1.f, 0.f, 360.f)) {
					component.ConeInnerAngle = glm::radians(coneInnerAngle);
				}

				f32 coneOuterAngle = glm::degrees(component.ConeOuterAngle);
				if (GUI::Properties::ScalarDragProperty<f32>(&coneOuterAngle, "Cone Outer Angle", "The outer angle of the cone in degrees", 1.f, 0.f, 360.f)) {
					component.ConeOuterAngle = glm::radians(coneOuterAngle);
				}

				GUI::Properties::ScalarSliderProperty<f32>(&component.ConeOuterGain, "Cone Outer Gain", "The gain of the outer cone", 0.f, 1.f);

				GUI::Properties::EndProperties();
			}, true);

			ImGui::PopStyleVar(3);

			OnEnd();
		}
	}

	void PropertiesPanel::DrawAddComponentMenu(Entity entity)
	{
		if (ImGui::BeginMenuEx("2D", SW_ICON_ARRANGE_BRING_FORWARD)) {

			if (!entity.HasComponent<SpriteComponent>()) {
				if (ImGui::MenuItemEx("Sprite Component", SW_ICON_IMAGE_SIZE_SELECT_ACTUAL)) {
					entity.AddComponent<SpriteComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<AnimatedSpriteComponent>()) {
				if (ImGui::MenuItemEx("Animated Sprite Component", SW_ICON_VIDEO)) {
					entity.AddComponent<AnimatedSpriteComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<CircleComponent>()) {
				if (ImGui::MenuItemEx("Circle Component", SW_ICON_CHECKBOX_BLANK_CIRCLE)) {
					entity.AddComponent<CircleComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<TextComponent>()) {
				if (ImGui::MenuItemEx("Text Component", SW_ICON_FORMAT_TEXT)) {
					entity.AddComponent<TextComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<RigidBody2DComponent>()) {
				if (ImGui::MenuItemEx("Rigid Body 2D", SW_ICON_SOCCER)) {
					entity.AddComponent<RigidBody2DComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<BoxCollider2DComponent>()) {
				if (ImGui::MenuItemEx("Box Collider 2D", SW_ICON_CHECKBOX_BLANK_OUTLINE)) {
					entity.AddComponent<BoxCollider2DComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<CircleCollider2DComponent>()) {
				if (ImGui::MenuItemEx("Circle Collider 2D", SW_ICON_CHECKBOX_BLANK_CIRCLE_OUTLINE)) {
					entity.AddComponent<CircleCollider2DComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<PolygonCollider2DComponent>()) {
				if (ImGui::MenuItemEx("Polygon Collider 2D", SW_ICON_VECTOR_POLYGON)) {
					entity.AddComponent<PolygonCollider2DComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<BuoyancyEffector2DComponent>()) {
				if (ImGui::MenuItemEx("Buoyancy Effector 2D", SW_ICON_WATER)) {
					entity.AddComponent<BuoyancyEffector2DComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<DistanceJoint2DComponent>()) {
				if (ImGui::MenuItemEx("Distance Joint 2D", SW_ICON_VECTOR_LINE)) {
					entity.AddComponent<DistanceJoint2DComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<RevolutionJoint2DComponent>()) {
				if (ImGui::MenuItemEx("Revolution Joint 2D", SW_ICON_ANGLE_ACUTE)) {
					entity.AddComponent<RevolutionJoint2DComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<PrismaticJoint2DComponent>()) {
				if (ImGui::MenuItemEx("Prismatic Joint 2D", SW_ICON_VIEW_AGENDA)) {
					entity.AddComponent<PrismaticJoint2DComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<SpringJoint2DComponent>()) {
				if (ImGui::MenuItemEx("Spring Joint 2D", SW_ICON_ARROW_EXPAND)) {
					entity.AddComponent<SpringJoint2DComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<WheelJoint2DComponent>()) {
				if (ImGui::MenuItemEx("Wheel Joint 2D", SW_ICON_CAR)) {
					entity.AddComponent<WheelJoint2DComponent>();

					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<CameraComponent>()) {
				if (ImGui::MenuItemEx("Camera Component", SW_ICON_CAMERA)) {
					SceneCamera camera(m_SceneViewportPanel->GetViewportAspectRatio());

					entity.AddComponent<CameraComponent>(camera);

					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndMenu();
		}


		if (!entity.HasComponent<ScriptComponent>()) {
			if (ImGui::MenuItemEx("Script", SW_ICON_LANGUAGE_CSHARP)) {
				entity.AddComponent<ScriptComponent>();

				ImGui::CloseCurrentPopup();
			}
		}

		if (!entity.HasComponent<AudioSourceComponent>()) {
			if (ImGui::MenuItemEx("Audio Source", SW_ICON_VOLUME_HIGH)) {
				entity.AddComponent<AudioSourceComponent>();

				ImGui::CloseCurrentPopup();
			}
		}

		if (!entity.HasComponent<AudioListenerComponent>()) {
			if (ImGui::MenuItemEx("Audio Listener", SW_ICON_MICROPHONE)) {
				entity.AddComponent<AudioListenerComponent>();

				ImGui::CloseCurrentPopup();
			}
		}

		if (ImGui::BeginMenuEx("3D", SW_ICON_PACKAGE_VARIANT_CLOSED)) {

			ImGui::EndMenu();
		}
	}

}
