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
		AddComponentName<TransformComponent>(SW_ICON_VECTOR_LINE "  Transform");
		AddComponentName<SpriteComponent>(SW_ICON_IMAGE_SIZE_SELECT_ACTUAL "  Sprite");
		AddComponentName<CircleComponent>(SW_ICON_CHECKBOX_BLANK_CIRCLE "  Circle");
		AddComponentName<CameraComponent>(SW_ICON_CAMERA "  Camera");

		AddComponentName<RigidBody2DComponent>(SW_ICON_SOCCER "  Rigid Body 2D");
		AddComponentName<BoxCollider2DComponent>(SW_ICON_CHECKBOX_BLANK_OUTLINE "  Box Collider 2D");
		AddComponentName<CircleCollider2DComponent>(SW_ICON_CHECKBOX_BLANK_CIRCLE_OUTLINE "  Circle Collider 2D");
		AddComponentName<BuoyancyEffector2DComponent>(SW_ICON_WATER "  Buoyancy Effector 2D");
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

			if (entity.HasComponent<TagComponent>()) {
				TagComponent& tc = entity.GetComponent<TagComponent>();

				GUI::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(0.f, 6.f));

				char buffer[256];

				memcpy(buffer, tc.Tag.c_str(), std::min(sizeof(buffer), tc.Tag.size() + 1));

				const f32 oldMouseX = ImGui::GetCursorPosX();
				const f32 oldMouseY = ImGui::GetCursorPosY();
				const f32 textWidth = ImGui::CalcTextSize(SW_ICON_TAG "  Tag").x;
				const f32 margin = 15.f;

				ImGui::SetCursorPos(ImVec2(oldMouseX, oldMouseY + 6));
				ImGui::Text(SW_ICON_TAG "  Tag");

				ImGui::SetCursorPos(ImVec2(oldMouseX + textWidth + margin, oldMouseY));

				if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
					tc.Tag = buffer;
			}

			if (ImGui::BeginPopup("AddComponent_Popup")) {
				if (ImGui::BeginMenu("2D")) {
					if (ImGui::MenuItemEx("Sprite Component", SW_ICON_IMAGE_SIZE_SELECT_ACTUAL)) {
						if (!entity.HasComponent<SpriteComponent>()) {
							entity.AddComponent<SpriteComponent>();
						}

						ImGui::CloseCurrentPopup();
					}

					if (ImGui::MenuItemEx("Circle Component", SW_ICON_CHECKBOX_BLANK_CIRCLE)) {
						if (!entity.HasComponent<CircleComponent>()) {
							entity.AddComponent<CircleComponent>();
						}

						ImGui::CloseCurrentPopup();
					}

					if (ImGui::MenuItemEx("Rigid Body 2D", SW_ICON_SOCCER)) {
						if (!entity.HasComponent<RigidBody2DComponent>()) {
							entity.AddComponent<RigidBody2DComponent>();
						}
					}

					if (ImGui::MenuItemEx("Box Collider 2D", SW_ICON_CHECKBOX_BLANK_OUTLINE)) {
						if (!entity.HasComponent<BoxCollider2DComponent>()) {
							entity.AddComponent<BoxCollider2DComponent>();
						}
					}

					if (ImGui::MenuItemEx("Circle Collider 2D", SW_ICON_CHECKBOX_BLANK_CIRCLE_OUTLINE)) {
						if (!entity.HasComponent<CircleCollider2DComponent>()) {
							entity.AddComponent<CircleCollider2DComponent>();
						}

						ImGui::CloseCurrentPopup();
					}

					if (ImGui::MenuItemEx("Buoyancy Effector 2D", SW_ICON_WATER)) {
						if (!entity.HasComponent<BuoyancyEffector2DComponent>()) {
							entity.AddComponent<BuoyancyEffector2DComponent>();
						}

						ImGui::CloseCurrentPopup();
					}

					if (ImGui::MenuItemEx("Camera Component", SW_ICON_CAMERA)) {
						if (!entity.HasComponent<CameraComponent>()) {
							SceneCamera camera(m_SceneViewportPanel->GetViewportAspectRatio());

							entity.AddComponent<CameraComponent>(camera);
						}

						ImGui::CloseCurrentPopup();
					}

					ImGui::EndMenu();
				}
				
				if (ImGui::BeginMenu("3D")) {

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}

			ImGui::BeginChild("PropertiesBody");
						
			DrawComponent<TransformComponent>(entity, [](TransformComponent& component) {
				GUI::BeginProperties("##transform_property");
				GUI::DrawVector3ControlProperty(component.Position, "Position: ", "Position of the entity");
				
				glm::vec3 rotation = glm::degrees(component.Rotation);
				GUI::DrawVector3ControlProperty(rotation, "Rotation: ", "Rotation of the entity in degrees");
				component.Rotation = glm::radians(rotation);

				GUI::DrawVector3ControlProperty(component.Scale, "Scale: ", "Scale of the entity", 1.f);
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
				GUI::DrawFloatingPointProperty(angle, "Flow Angle: ");
				component.FlowAngle = glm::radians(angle);
				GUI::DrawFloatingPointProperty(component.Density, "Density", nullptr, 0.f);
				GUI::EndProperties();
			}, true);
			
			ImGui::EndChild();

			OnEnd();
		}
	}

}
