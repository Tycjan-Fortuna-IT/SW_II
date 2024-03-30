#include "PropertiesPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "imgui.h"
#include "Managers/SelectionManager.hpp"
#include "GUI/GUI.hpp"
#include "Core/ECS/Components.hpp"
#include "GUI/Icons.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Scene/SceneCamera.hpp"
#include "Core/Scripting/ScriptingCore.hpp"
#include "Core/Scripting/ScriptStorage.hpp"
#include "Coral/Attribute.hpp"

namespace SW {

	PropertiesPanel::PropertiesPanel(SceneViewportPanel* sceneViewportPanel)
		: Panel("Properties", SW_ICON_INFORMATION, true), m_SceneViewportPanel(sceneViewportPanel)
	{
		AddComponentName<TagComponent>(SW_ICON_TAG "  Tag");
		
		AddComponentName<TransformComponent>(SW_ICON_VECTOR_LINE "  Transform");
		AddComponentName<SpriteComponent>(SW_ICON_IMAGE_SIZE_SELECT_ACTUAL "  Sprite");
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

		m_ComponentCopyScene = new Scene("");
		m_ComponentCopyEntity = m_ComponentCopyScene->CreateEntity();
	}

	PropertiesPanel::~PropertiesPanel()
	{
		delete m_ComponentCopyScene;
	}

	void PropertiesPanel::OnUpdate(Timestep dt)
	{

	}

	void PropertiesPanel::OnRender()
	{
		PROFILE_SCOPE();

		constexpr f32 MaxFloatValue = FLT_MAX / 2.f;

		if (OnBegin()) {
			if (!SelectionManager::IsSelected()) {
				OnEnd(); return;
			}

			Entity entity = m_SceneViewportPanel->GetCurrentScene()->GetEntityByID(SelectionManager::GetSelectionID());

			if (GUI::Button("{} Add", { 90.f, 30.f }, SW_ICON_PLUS)) {
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

			ImGui::BeginChild("PropertiesBody");

			DrawComponent<TagComponent>(entity, [](TagComponent& component) {
				GUI::BeginProperties("##tag_property");
				GUI::DrawSingleLineTextInputProperty<256>(component.Tag, "Tag ");
				GUI::EndProperties();
			}, false);

			DrawComponent<TransformComponent>(entity, [](TransformComponent& component) {
				GUI::BeginProperties("##transform_property");
				GUI::DrawVector3ControlProperty(component.Position, "Position ", "Position of the entity");

				glm::vec3 rotation = glm::degrees(component.Rotation);
				GUI::DrawVector3ControlProperty(rotation, "Rotation ", "Rotation of the entity in degrees");
				component.Rotation = glm::radians(rotation);

				GUI::DrawVector3ControlProperty(component.Scale, "Scale ", "Scale of the entity", 1.f);
				GUI::EndProperties();
			}, false);

			DrawComponent<SpriteComponent>(entity, [](SpriteComponent& component) {
				GUI::BeginProperties("##sprite_property");
				GUI::DrawVector4ColorPickerProperty(component.Color, "Color", "Color of the sprite");
				GUI::DrawTextureProperty(&component.Texture, "Texture", "Texture to be used (transparency is supported)");
				GUI::DrawFloatingPointProperty(component.TilingFactor, "Tiling", "Tiling factor of the texture (how many times the texture should be repeated)", 0.f, 10.f);
				GUI::EndProperties();
			}, true);

			DrawComponent<CircleComponent>(entity, [](CircleComponent& component) {
				GUI::BeginProperties("##circle_property");
				GUI::DrawVector4ColorPickerProperty(component.Color, "Color", "Color of the circle");
				GUI::DrawFloatingPointProperty(component.Thickness, "Thickness", "Decreasing thickness will start to create a ring inside", 0.f, 1.f);
				GUI::DrawFloatingPointProperty(component.Fade, "Fade", "How much the circle starts to fade from outside", 0.f, 4.f);
				GUI::EndProperties();
			}, true);

			DrawComponent<TextComponent>(entity, [](TextComponent& component) {
				GUI::BeginProperties("##text_property");
				GUI::DrawFontDropdownProperty(&component.Font, "Font", "Font to be used");
				if (component.Font) {
					GUI::DrawMultilineTextInputProperty(component.TextString, "Text", "Text to display (max 500 characters)");
					GUI::DrawVector4ColorPickerProperty(component.Color, "Color", "Color of the text");
					GUI::DrawFloatingPointProperty(component.Kerning, "Kerning", "The space between the characters", 0.f, MaxFloatValue);
					GUI::DrawFloatingPointProperty(component.LineSpacing, "Line Spacing", "The space between the lines", 0.f, MaxFloatValue);
				}
				GUI::EndProperties();
			}, true);

			DrawComponent<CameraComponent>(entity, [](CameraComponent& component) {
				GUI::BeginProperties("##camera_property");
				GUI::DrawBooleanProperty(component.Primary, "Primary", "Whether this camera is the primary camera (only one camera can be primary)");

				ProjectionType type = component.Camera.GetProjectionType();

				if (
					GUI::DrawSelectableProperty(type, {
						GUI::SelectOption<ProjectionType>{ "Orthographic", ProjectionType::Orthographic },
						GUI::SelectOption<ProjectionType>{ "Perspective", ProjectionType::Perspective },
						}, "Projection Type")
						) {
					component.Camera.SetProjectionType(type);
				}

				if (type == ProjectionType::Orthographic) {

					f32 orthoSize = component.Camera.GetOrthographicSize();
					if (GUI::DrawFloatingPointProperty(orthoSize, "Size", "Size of the camera", 0.f, MaxFloatValue))
						component.Camera.SetOrthographicSize(orthoSize);

					f32 orthoNear = component.Camera.GetOrthographicNearClip();
					if (GUI::DrawFloatingPointProperty(orthoNear, "Near Clip", "The near clip of the camera", 0.f, MaxFloatValue))
						component.Camera.SetOrthographicNearClip(orthoNear);

					f32 orthoFar = component.Camera.GetOrthographicFarClip();
					if (GUI::DrawFloatingPointProperty(orthoFar, "Far Clip", "The far clip of the camera", 0.f, MaxFloatValue))
						component.Camera.SetOrthographicFarClip(orthoFar);

				} else if (type == ProjectionType::Perspective) {

					f32 verticalFov = glm::degrees(component.Camera.GetPerspectiveVerticalFOV());
					if (GUI::DrawFloatingPointProperty(verticalFov, "Vertical FOV", "Vertical field of view of the camera in degrees", 0.f, 180.f))
						component.Camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

					f32 perspectiveNear = component.Camera.GetPerspectiveNearClip();
					if (GUI::DrawFloatingPointProperty(perspectiveNear, "Near Clip", "The near clip of the camera", 0.f, MaxFloatValue))
						component.Camera.SetPerspectiveNearClip(perspectiveNear);

					f32 perspectiveFar = component.Camera.GetPerspectiveFarClip();
					if (GUI::DrawFloatingPointProperty(perspectiveFar, "Far Clip", "The far clip of the camera", 0.f, MaxFloatValue))
						component.Camera.SetPerspectiveFarClip(perspectiveFar);

				}

				GUI::EndProperties();
			}, true);

			DrawComponent<ScriptComponent>(entity, [=](ScriptComponent& component) {
				GUI::BeginProperties("##script_property");
				ScriptingCore& scriptingCore = ScriptingCore::Get();

				static bool once = false;

				const std::unordered_map<u64, ScriptMetadata>& allScripts = scriptingCore.GetAllScripts();

				std::vector<GUI::SelectOption<u64>> scripts;

				scripts.push_back({ "Invalid or no script", 0 });

				for (auto& [scriptId, scriptMetaData] : allScripts) {
					scripts.push_back({ scriptMetaData.FullName, scriptId });
				}

				ScriptStorage& storage = m_SceneViewportPanel->GetCurrentScene()->GetScriptStorage();

				u64 currentId = component.ScriptID;

				if (GUI::DrawSelectableProperty(component.ScriptID, scripts, "Scripts")) {
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

						bool isSerializable = reflectionHelper.InvokeStaticMethod<Coral::Bool32>(
							"HasFieldAttribute", assemblyName, className, fieldName, serializeAttrName
						);

						if (!isSerializable)
							continue;

						if (fieldStorage.IsArray()) {
							ASSERT(false, "Arrays not yet supported!");
						} else {
							DataType fieldType = fieldStorage.GetType();

							switch (fieldType) {
								case SW::DataType::Byte: ASSERT(false, "Byte not yet supported!");
									break;
								case SW::DataType::Short: ASSERT(false, "Short not yet supported!");
									break;
								case SW::DataType::UShort: ASSERT(false, "UShort not yet supported!");
									break;
								case SW::DataType::Int: ASSERT(false, "Int not yet supported!");
									break;
								case SW::DataType::UInt: ASSERT(false, "UInt not yet supported!");
									break;
								case SW::DataType::Long: ASSERT(false, "Long not yet supported!");
									break;
								case SW::DataType::ULong: ASSERT(false, "ULong not yet supported!");
									break;
								case SW::DataType::Float:
								{
									f32 value = fieldStorage.GetValue<f32>();
									
									if (GUI::DrawFloatingPointProperty(value, fieldStorage.GetName().data())) {
										fieldStorage.SetValue(value);
									}

									break;
								}
								case SW::DataType::Double: ASSERT(false, "Double not yet supported!");
									break;
								case SW::DataType::Bool: ASSERT(false, "Bool not yet supported!");
									break;
								case SW::DataType::Entity: ASSERT(false, "Entity not yet supported!");
									break;
								default:
									SW_WARN("{}", (int)fieldType);
									break;
							}
						}
					}
				}

				GUI::EndProperties();
			}, true);

			DrawComponent<RigidBody2DComponent>(entity, [](RigidBody2DComponent& component) {
				GUI::BeginProperties("##rigid_body_2d_property");
				GUI::DrawSelectableProperty(component.Type, {
					GUI::SelectOption<PhysicBodyType>{ "Static", PhysicBodyType::Static },
					GUI::SelectOption<PhysicBodyType>{ "Kinematic", PhysicBodyType::Kinematic },
					GUI::SelectOption<PhysicBodyType>{ "Dynamic", PhysicBodyType::Dynamic }
				}, "Body Type", "The type of the body, static bodies are immovable, kinematic bodies are movable but not affected by forces, dynamic bodies are movable and affected by forces");
				if (component.Type == PhysicBodyType::Kinematic || component.Type == PhysicBodyType::Dynamic) {
					GUI::DrawBooleanProperty(component.AutoMass, "Auto mass", "Whether the body's mass should be determined automatically or based on set mass");
					if (!component.AutoMass) {
						GUI::DrawFloatingPointProperty(component.Mass, "Mass", "The mass of the body", 0.f, MaxFloatValue);
					}
				}
				if (component.Type == PhysicBodyType::Dynamic) {
					GUI::DrawFloatingPointProperty(component.GravityScale, "Gravity Scale", "The gravity scale of the body");
					GUI::DrawFloatingPointProperty(component.Friction, "Friction", "The friction of the body (how much it resists movement)", 0.f, 1.f);
					GUI::DrawFloatingPointProperty(component.Restitution, "Restitution", "The restitution of the body (how much it bounces)", 0.f, 1.f);
					GUI::DrawFloatingPointProperty(component.RestitutionThreshold, "Restitution Threshold", "The velocity threshold for the restitution", 0.f, 1.f);
					GUI::DrawFloatingPointProperty(component.LinearDamping, "Linear Damping", "The linear damping of the body (how much it resists movement)", 0.f, 1.f);
					GUI::DrawFloatingPointProperty(component.AngularDamping, "Angular Damping", "The angular damping of the body (how much it resists rotation)", 0.f, 1.f);
					GUI::DrawBooleanProperty(component.AllowSleep, "Allow Sleep", "Whether the body should be allowed to sleep");
					GUI::DrawBooleanProperty(component.InitiallyAwake, "Initially Awake", "Whether the body should be allowed to sleep");
					GUI::DrawBooleanProperty(component.IsBullet, "Is Bullet", "Is this a fast moving body that should be prevented from tunneling through other moving bodies?");
				}
				GUI::EndProperties();
			}, true);

			DrawComponent<BoxCollider2DComponent>(entity, [](BoxCollider2DComponent& component) {
				GUI::BeginProperties("##box_collider_2d_property");
				GUI::DrawVector2ControlProperty(component.Size, "Size", "Size of the collider", 0.5f);
				GUI::DrawVector2ControlProperty(component.Offset, "Offset", "Offset of the collider from origin");
				GUI::DrawFloatingPointProperty(component.Density, "Density", "Density of the collider (mass = density * area)", 0.f, MaxFloatValue);
				GUI::DrawBooleanProperty(component.IsSensor, "Is Sensor?", "Whether to react to the collision or just sense it.");
				GUI::EndProperties();
			}, true);

			DrawComponent<CircleCollider2DComponent>(entity, [](CircleCollider2DComponent& component) {
				GUI::BeginProperties("##circle_collider_2d_property");
				GUI::DrawFloatingPointProperty(component.Radius, "Radius", "Radius of the collider", 0.1f, MaxFloatValue);
				GUI::DrawVector2ControlProperty(component.Offset, "Offset", "Offset of the collider from origin");
				GUI::DrawFloatingPointProperty(component.Density, "Density", "Density of the collider (mass = density * area)", 0.f, MaxFloatValue);
				GUI::DrawBooleanProperty(component.IsSensor, "Is Sensor?", "Whether to react to the collision or just sense it.");
				GUI::EndProperties();
			}, true);

			DrawComponent<PolygonCollider2DComponent>(entity, [](PolygonCollider2DComponent& component) {
				GUI::BeginProperties("##polygon_collider_2d_property");
				GUI::DrawVector2TableList(component.Vertices, "Edges", "List of all of the edges of the polygon collider (minimum 3 edges!)");
				GUI::DrawVector2ControlProperty(component.Offset, "Offset", "Offset of the collider from origin");
				GUI::DrawFloatingPointProperty(component.Density, "Density", "Density of the collider (mass = density * area)", 0.f, MaxFloatValue);
				GUI::DrawBooleanProperty(component.IsSensor, "Is Sensor?", "Whether to react to the collision or just sense it.");
				GUI::EndProperties();
			}, true);

			DrawComponent<BuoyancyEffector2DComponent>(entity, [](BuoyancyEffector2DComponent& component) {
				GUI::BeginProperties("##buoancy_effector_2d_property");
				GUI::DrawFloatingPointProperty(component.DragMultiplier, "Drag Multiplier", "The drag multiplier of the effector (how much it resists movement)");
				GUI::DrawFloatingPointProperty(component.FlowMagnitude, "Flow Magnitude", "The magnitude of the flow force in Newtons");
				f32 angle = glm::degrees(component.FlowAngle);
				GUI::DrawFloatingPointProperty(angle, "Flow Angle", "The angle of the flow force in degrees");
				component.FlowAngle = glm::radians(angle);
				GUI::DrawFloatingPointProperty(component.Density, "Density", "Density of the collider (mass = density * area)", 0.f, MaxFloatValue);
				GUI::EndProperties();
			}, true);

			DrawComponent<DistanceJoint2DComponent>(entity, [this](DistanceJoint2DComponent& component) {
				GUI::BeginProperties("##distance_joint_2d_property");
				GUI::DrawEntityDropdownProperty(component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::DrawVector2ControlProperty(component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::DrawVector2ControlProperty(component.ConnectedAnchor, "Connected Anchor", "The anchor point of the connected body");
				GUI::DrawFloatingPointProperty(component.BreakingForce, "Breaking Force", "The force acting on the joint to break it in Newtons", 0.f, MaxFloatValue);
				GUI::DrawBooleanProperty(component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::DrawBooleanProperty(component.AutoLength, "Auto length", "Whether the distance should be automatically calculated");
				if (!component.AutoLength) {
					GUI::DrawFloatingPointProperty(component.Length, "Length", "The distance between the two bodies", 0.f, MaxFloatValue);
				}
				GUI::DrawFloatingPointProperty(component.MinLength, "Min Length", "The minimum distance between the two bodies", 0.f, component.MaxLength);
				GUI::DrawFloatingPointProperty(component.MaxLength, "Max Length", "The maximum distance between the two bodies", component.MinLength, MaxFloatValue);
				GUI::EndProperties();
			}, true);

			DrawComponent<RevolutionJoint2DComponent>(entity, [this](RevolutionJoint2DComponent& component) {
				GUI::BeginProperties("##revolution_joint_2d_property");
				GUI::DrawEntityDropdownProperty(component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::DrawVector2ControlProperty(component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::DrawBooleanProperty(component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::DrawBooleanProperty(component.EnableLimit, "Limit Enabled", "Whether the joint should have a limit");
				if (component.EnableLimit) {
					f32 lowerAngle = glm::degrees(component.LowerAngle);
					GUI::DrawFloatingPointProperty(lowerAngle, "Lower Angle", "The lower angle for the joint limit (in degrees)");
					component.LowerAngle = glm::radians(lowerAngle);

					f32 upperAngle = glm::degrees(component.UpperAngle);
					GUI::DrawFloatingPointProperty(upperAngle, "Upper Angle", "The upper angle for the joint limit (in degrees)");
					component.UpperAngle = glm::radians(upperAngle);
				}
				GUI::DrawBooleanProperty(component.EnableMotor, "Enable Motor", "Whether the joint should have a motor");
				if (component.EnableMotor) {
					GUI::DrawFloatingPointProperty(component.MotorSpeed, "Motor Speed", "The speed of the motor in radians per second");
					GUI::DrawFloatingPointProperty(component.MaxMotorTorque, "Max Motor Torque", "The maximum torque of the motor in Newtons");
				}
				GUI::DrawFloatingPointProperty(component.BreakingTorque, "Breaking Torque", "The torque acting on the joint to break it in Newtons", 0.f, MaxFloatValue);
				GUI::DrawFloatingPointProperty(component.BreakingForce, "Breaking Force", "The force acting on the joint to break it in Newtons", 0.f, MaxFloatValue);

				GUI::EndProperties();
			}, true);

			DrawComponent<PrismaticJoint2DComponent>(entity, [this](PrismaticJoint2DComponent& component) {
				GUI::BeginProperties("##prismatic_joint_2d_property");
				GUI::DrawEntityDropdownProperty(component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::DrawVector2ControlProperty(component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::DrawBooleanProperty(component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::DrawBooleanProperty(component.EnableLimit, "Limit Enabled", "Whether the joint should have a limit");
				if (component.EnableLimit) {
					f32 angle = glm::degrees(component.Angle);
					GUI::DrawFloatingPointProperty(angle, "Angle", "The constrained angle between the bodies (in degrees)");
					component.Angle = glm::radians(angle);
					GUI::DrawFloatingPointProperty(component.LowerTranslation, "Lower Translation");
					GUI::DrawFloatingPointProperty(component.UpperTranslation, "Upper Translation");
				}
				GUI::DrawBooleanProperty(component.EnableMotor, "Enable Motor", "Whether the joint should have a motor");
				if (component.EnableMotor) {
					GUI::DrawFloatingPointProperty(component.MotorSpeed, "Motor Speed", "The speed of the motor in radians per second", 0.f, MaxFloatValue);
					GUI::DrawFloatingPointProperty(component.MaxMotorForce, "Max Motor Force", "The maximum force of the motor in Newtons", 0.f, MaxFloatValue);
				}
				GUI::EndProperties();
			}, true);

			DrawComponent<SpringJoint2DComponent>(entity, [this](SpringJoint2DComponent& component) {
				GUI::BeginProperties("##spring_joint_2d_property");
				GUI::DrawEntityDropdownProperty(component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::DrawVector2ControlProperty(component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::DrawVector2ControlProperty(component.ConnectedAnchor, "Connected Anchor", "The anchor point of the connected body");
				GUI::DrawFloatingPointProperty(component.BreakingForce, "Breaking Force", "The force acting on the joint to break it in Newtons", 0.f);
				GUI::DrawBooleanProperty(component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::DrawBooleanProperty(component.AutoLength, "Auto length", "Whether the distance should be automatically calculated");
				if (!component.AutoLength) {
					GUI::DrawFloatingPointProperty(component.Length, "Length", "The distance between the two bodies", 0.f);
				}
				GUI::DrawFloatingPointProperty(component.MinLength, "Min Length", "The minimum distance between the two bodies", 0.f, component.MaxLength);
				GUI::DrawFloatingPointProperty(component.MaxLength, "Max Length", "The maximum distance between the two bodies", component.MinLength, MaxFloatValue);
				GUI::DrawFloatingPointProperty(component.Frequency, "Frequency", "The linear stiffness in N/m.");
				GUI::DrawFloatingPointProperty(component.DampingRatio, "Damping Ratio", "The linear damping in N*s/m.", 0.0f, 1.0f);
				GUI::EndProperties();
			}, true);

			DrawComponent<WheelJoint2DComponent>(entity, [this](WheelJoint2DComponent& component) {
				GUI::BeginProperties("##wheel_joint_2d_property");
				GUI::DrawEntityDropdownProperty(component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::DrawVector2ControlProperty(component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::DrawBooleanProperty(component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::DrawBooleanProperty(component.EnableLimit, "Limit Enabled", "Whether the joint should have a limit");
				if (component.EnableLimit) {
					GUI::DrawFloatingPointProperty(component.LowerTranslation, "Lower Translation");
					GUI::DrawFloatingPointProperty(component.UpperTranslation, "Upper Translation");
				}
				GUI::DrawBooleanProperty(component.EnableMotor, "Enable Motor", "Whether the joint should have a motor");
				if (component.EnableMotor) {
					GUI::DrawFloatingPointProperty(component.MotorSpeed, "Motor Speed", "The speed of the motor in radians per second");
					GUI::DrawFloatingPointProperty(component.MaxMotorTorque, "Max Motor Torque", "The maximum torque of the motor in Newtons");
				}
				GUI::DrawFloatingPointProperty(component.Frequency, "Frequency", "The linear stiffness in N/m.");
				GUI::DrawFloatingPointProperty(component.DampingRatio, "Damping Ratio", "The linear damping in N*s/m.", 0.0f, 1.0f);
				GUI::EndProperties();
			}, true);

			ImGui::EndChild();

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

		if (ImGui::BeginMenuEx("3D", SW_ICON_PACKAGE_VARIANT_CLOSED)) {

			ImGui::EndMenu();
		}
	}

}
