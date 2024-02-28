#include "PropertiesPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "imgui.h"
#include "Managers/SelectionManager.hpp"
#include "GUI/GUI.hpp"
#include "Core/ECS/Components.hpp"
#include "GUI/Icons.hpp"
#include "Core/AssetManager.hpp"
#include "Core/ECS/Entity.hpp"

namespace SW {

	static std::unordered_map<size_t, std::string> s_ComponentNames;

	template<typename Component>
	static void AddComponentName(const std::string& name)
	{
		const size_t id = entt::type_id<Component>().hash();
		s_ComponentNames[id] = name;
	}

	PropertiesPanel::PropertiesPanel(SceneViewportPanel* sceneViewportPanel)
		: Panel("Properties", SW_ICON_INFORMATION, true), m_SceneViewportPanel(sceneViewportPanel)
	{
		AddComponentName<TagComponent>(SW_ICON_TAG "  Tag");
		AddComponentName<TransformComponent>(SW_ICON_VECTOR_LINE "  Transform");
		AddComponentName<SpriteComponent>(SW_ICON_IMAGE_SIZE_SELECT_ACTUAL "  Sprite");
		AddComponentName<CircleComponent>(SW_ICON_CHECKBOX_BLANK_CIRCLE "  Circle");
		AddComponentName<CameraComponent>(SW_ICON_CAMERA "  Camera");

		AddComponentName<RigidBody2DComponent>(SW_ICON_SOCCER "  Rigid Body 2D");
		AddComponentName<BoxCollider2DComponent>(SW_ICON_CHECKBOX_BLANK_OUTLINE "  Box Collider 2D");
		AddComponentName<CircleCollider2DComponent>(SW_ICON_CHECKBOX_BLANK_CIRCLE_OUTLINE "  Circle Collider 2D");
		AddComponentName<BuoyancyEffector2DComponent>(SW_ICON_WATER "  Buoyancy Effector 2D");
		
		AddComponentName<DistanceJoint2DComponent>(SW_ICON_VECTOR_LINE "  Distance Joint 2D");
		AddComponentName<RevolutionJoint2DComponent>(SW_ICON_ANGLE_ACUTE "  Revolution Joint 2D");
		AddComponentName<PrismaticJoint2DComponent>(SW_ICON_VIEW_AGENDA "  Prismatic Joint 2D");
	}

	void PropertiesPanel::OnUpdate(Timestep dt)
	{

	}

	template<typename T, typename Fn>
		requires std::is_invocable_v<Fn, T&>
	static void DrawComponent(Entity entity, Fn fn, bool removable = true)
	{
		if (!entity.HasComponent<T>())
			return;

		static constexpr ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_FramePadding;

		T& component = entity.GetComponent<T>();

		const f32 lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lineHeight * 0.25f);

		const size_t id = entt::type_id<T>().hash();
		const bool open = ImGui::TreeNodeEx(reinterpret_cast<void*>(id), treeFlags, "%s", s_ComponentNames[id].c_str());

		bool removeComponent = false;

		if (removable) {
			ImGui::PushID(static_cast<int>(id));

			const f32 frameHeight = ImGui::GetFrameHeight();

			ImGui::SameLine(ImGui::GetContentRegionMax().x - frameHeight * 1.2f);

			if (ImGui::Button(SW_ICON_SETTINGS, ImVec2{ frameHeight * 1.2f, frameHeight }))
				ImGui::OpenPopup("ComponentSettings");

			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			ImGui::PopID();
		}

		if (open) {
			fn(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<T>();
	}

	void PropertiesPanel::OnRender()
	{
		if (OnBegin()) {
			if (!SelectionManager::IsSelected()) {
				OnEnd(); return;
			}

			Entity entity = m_SceneViewportPanel->GetCurrentScene()->GetEntityByID(SelectionManager::GetSelectionID());

			if (GUI::Button("{} Add", { 90.f, 30.f }, SW_ICON_PLUS)) {
				ImGui::OpenPopup("AddComponent_Popup");
			}

			{
				ImGui::SameLine();

				IDComponent& id = entity.GetComponent<IDComponent>();

				ImGui::Text(SW_ICON_KEY "  ID");

				ImGui::SameLine();

				ImGui::TextDisabled(std::to_string(id.ID).c_str());
			}

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
				GUI::DrawVector4ColorPickerProperty(component.Color, "Color");
				GUI::DrawTextureProperty(&component.Texture, "Texture");
				GUI::DrawFloatingPointProperty(component.TilingFactor, "Tiling", nullptr, 1.f, 10.f);
				GUI::EndProperties();
			}, true);

			DrawComponent<CircleComponent>(entity, [](CircleComponent& component) {
				GUI::BeginProperties("##circle_property");
				GUI::DrawVector4ColorPickerProperty(component.Color, "Color");
				GUI::DrawFloatingPointProperty(component.Thickness, "Thickness", nullptr, 0.f);
				GUI::DrawFloatingPointProperty(component.Fade, "Fade", nullptr, 0.f);
				GUI::EndProperties();
			}, true);

			DrawComponent<CameraComponent>(entity, [](CameraComponent& component) {
				GUI::BeginProperties("##camera_property");
				GUI::DrawBooleanProperty(component.Primary, "Primary");
				GUI::EndProperties();
			}, true);

			DrawComponent<RigidBody2DComponent>(entity, [](RigidBody2DComponent& component) {
				GUI::BeginProperties("##rigid_body_2d_property");
				GUI::DrawSelectableProperty(component.Type, {
					GUI::SelectOption<PhysicBodyType>{ "Static", PhysicBodyType::Static },
					GUI::SelectOption<PhysicBodyType>{ "Kinematic", PhysicBodyType::Kinematic },
					GUI::SelectOption<PhysicBodyType>{ "Dynamic", PhysicBodyType::Dynamic }
				}, "Body Type");
				if (component.Type == PhysicBodyType::Dynamic) {
					GUI::DrawFloatingPointProperty(component.GravityScale, "Gravity Scale");
					GUI::DrawFloatingPointProperty(component.Friction, "Friction", nullptr, 0.f, 1.f);
					GUI::DrawFloatingPointProperty(component.Restitution, "Restitution", nullptr, 0.f, 1.f);
					GUI::DrawFloatingPointProperty(component.RestitutionThreshold, "Restitution Threshold", nullptr);
					GUI::DrawBooleanProperty(component.AllowSleep, "Allow Sleep");
				}
				GUI::EndProperties();
			}, true);

			DrawComponent<BoxCollider2DComponent>(entity, [](BoxCollider2DComponent& component) {
				GUI::BeginProperties("##box_collider_2d_property");
				GUI::DrawVector2ControlProperty(component.Size, "Size", nullptr, 0.5f);
				GUI::DrawVector2ControlProperty(component.Offset, "Offset");
				GUI::DrawFloatingPointProperty(component.Density, "Density", nullptr, 0.f);
				GUI::DrawBooleanProperty(component.IsSensor, "Is Sensor?", "Whether to react to the collision or just sense it.");
				GUI::EndProperties();
			}, true);

			DrawComponent<CircleCollider2DComponent>(entity, [](CircleCollider2DComponent& component) {
				GUI::BeginProperties("##circle_collider_2d_property");
				GUI::DrawFloatingPointProperty(component.Radius, "Radius", nullptr, 0.f);
				GUI::DrawVector2ControlProperty(component.Offset, "Offset");
				GUI::DrawFloatingPointProperty(component.Density, "Density", nullptr, 0.f);
				GUI::DrawBooleanProperty(component.IsSensor, "Is Sensor?", "Whether to react to the collision or just sense it.");
				GUI::EndProperties();
			}, true);

			DrawComponent<BuoyancyEffector2DComponent>(entity, [](BuoyancyEffector2DComponent& component) {
				GUI::BeginProperties("##buoancy_effector_2d_property");
				GUI::DrawFloatingPointProperty(component.DragMultiplier, "Drag Multiplier");
				GUI::DrawFloatingPointProperty(component.FlowMagnitude, "Flow Magnitude");
				f32 angle = glm::degrees(component.FlowAngle);
				GUI::DrawFloatingPointProperty(angle, "Flow Angle ");
				component.FlowAngle = glm::radians(angle);
				GUI::DrawFloatingPointProperty(component.Density, "Density", nullptr, 0.f);
				GUI::EndProperties();
			}, true);

			DrawComponent<DistanceJoint2DComponent>(entity, [this](DistanceJoint2DComponent& component) {
				GUI::BeginProperties("##distance_joint_2d_property");
				GUI::DrawEntityDropdownProperty(component.ConnectedEntityID, m_SceneViewportPanel->GetCurrentScene(), "Connected entity", "The joint will connect to this entity's rigid body");
				GUI::DrawVector2ControlProperty(component.OriginAnchor, "Origin Anchor", "The anchor point of this body");
				GUI::DrawVector2ControlProperty(component.ConnectedAnchor, "Connected Anchor", "The anchor point of the connected body");
				GUI::DrawFloatingPointProperty(component.BreakingForce, "Breaking Force", "The force acting on the joint to break it in Newtons", 0.f);
				GUI::DrawBooleanProperty(component.EnableCollision, "Enable collision", "Whether connected by joint bodies should collide with each other");
				GUI::DrawBooleanProperty(component.AutoLength, "Auto length", "Whether the distance should be automatically calculated");
				if (!component.AutoLength) {
					GUI::DrawFloatingPointProperty(component.Length, "Length", "The distance between the two bodies", 0.f);
				}
				GUI::DrawFloatingPointProperty(component.MinLength, "Min Length", "The minimum distance between the two bodies", 0.f);
				GUI::DrawFloatingPointProperty(component.MaxLength, "Max Length", "The maximum distance between the two bodies", component.MinLength);
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
					GUI::DrawFloatingPointProperty(component.MotorSpeed, "Motor Speed", "The speed of the motor in radians per second", 0.f);
					GUI::DrawFloatingPointProperty(component.MaxMotorTorque, "Max Motor Torque", "The maximum torque of the motor in Newtons", 0.f);
				}
				GUI::DrawFloatingPointProperty(component.BreakingTorque, "Breaking Torque", "The torque acting on the joint to break it in Newtons", 0.f);
				GUI::DrawFloatingPointProperty(component.BreakingForce, "Breaking Force", "The force acting on the joint to break it in Newtons", 0.f);

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
				}
				GUI::DrawFloatingPointProperty(component.LowerTranslation, "Lower Translation");
				GUI::DrawFloatingPointProperty(component.UpperTranslation, "Upper Translation");
				GUI::DrawBooleanProperty(component.EnableMotor, "Enable Motor", "Whether the joint should have a motor");
				if (component.EnableMotor) {
					GUI::DrawFloatingPointProperty(component.MotorSpeed, "Motor Speed", "The speed of the motor in radians per second", 0.f);
					GUI::DrawFloatingPointProperty(component.MaxMotorForce, "Max Motor Force", "The maximum force of the motor in Newtons", 0.f);
				}

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

			if (!entity.HasComponent<CameraComponent>()) {
				if (ImGui::MenuItemEx("Camera Component", SW_ICON_CAMERA)) {
					SceneCamera camera(m_SceneViewportPanel->GetViewportAspectRatio());

					entity.AddComponent<CameraComponent>(camera);

					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenuEx("3D", SW_ICON_PACKAGE_VARIANT_CLOSED)) {

			ImGui::EndMenu();
		}
	}

}
