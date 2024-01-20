#include "PropertiesPanel.hpp"

#include <icons/IconsMaterialDesignIcons.h>

#include "Core/Utils/Utils.hpp"
#include "imgui.h"
#include "Managers/SelectionManager.hpp"
#include "GUI/GUI.hpp"
#include "Core/ECS/Components.hpp"

namespace SW {

	PropertiesPanel::PropertiesPanel(Scene* context)
		: Panel("Properties", String::FromChar8T(ICON_MDI_INFORMATION), true), m_Scene(context) {}

	void PropertiesPanel::OnUpdate(Timestep dt)
	{

	}

	void PropertiesPanel::OnRender()
	{
		if (OnBegin()) {
			if (!SelectionManager::IsSelected()) {
				OnEnd(); return;
			}

			Entity entity = m_Scene->GetEntityByID(SelectionManager::GetSelectionID());

			static const std::string transformLabel = 
				(std::string)String::FromChar8T(ICON_MDI_VECTOR_LINE) + "  Transform";

			static const std::string spriteLabel = 
				(std::string)String::FromChar8T(ICON_MDI_IMAGE_SIZE_SELECT_ACTUAL) + "  Sprite";

			if (GUI::Button("{} Add", { 90.f, 30.f }, String::FromChar8T(ICON_MDI_PLUS))) {
				ImGui::OpenPopup("AddComponent_Popup");
			}


			if (entity.HasComponent<TagComponent>()) {
				TagComponent& tc = entity.GetComponent<TagComponent>();

				GUI::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(0.f, 6.f));

				char buffer[256];

				memcpy(buffer, tc.Tag.c_str(), std::min(sizeof(buffer), tc.Tag.size() + 1));

				ImGui::SameLine();

				if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
					tc.Tag = buffer;
			}

			if (ImGui::BeginPopup("AddComponent_Popup")) {
				if (ImGui::BeginMenu("Components")) {
					if (ImGui::MenuItem("Tag Component")) {

						ImGui::CloseCurrentPopup();
					}

					if (ImGui::MenuItem("Transform Component")) {

						ImGui::CloseCurrentPopup();
					}

					if (ImGui::MenuItem("Sprite Component")) {
						entity.AddComponent<SpriteComponent>();

						ImGui::CloseCurrentPopup();
					}

					ImGui::EndMenu();
				}
				
				ImGui::EndPopup();
			}

			if (ImGui::CollapsingHeader(transformLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
				TransformComponent& tc = entity.GetComponent<TransformComponent>();

				GUI::DisplayVector3Slider(tc.Position, "Position: ", 0.0f);
				GUI::DisplayVector3Slider(tc.Rotation, "Rotation: ", 0.0f);
				GUI::DisplayVector3Slider(tc.Scale, "Scale: ", 0.0f);
			}

			if (entity.HasComponent<SpriteComponent>()) {
				if (ImGui::CollapsingHeader(spriteLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					SpriteComponent& sc = entity.GetComponent<SpriteComponent>();

					GUI::DisplayColorPicker(sc.Color, "Color");
				}
			}

			OnEnd();
		}
	}

}
