#include "PropertiesPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "imgui.h"
#include "Managers/SelectionManager.hpp"
#include "GUI/GUI.hpp"
#include "Core/ECS/Components.hpp"
#include "GUI/Icons.hpp"

namespace SW {

	PropertiesPanel::PropertiesPanel(Scene* context)
		: Panel("Properties", SW_ICON_INFORMATION, true), m_Scene(context) {}

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

			if (ImGui::CollapsingHeader(SW_ICON_VECTOR_LINE "  Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				TransformComponent& tc = entity.GetComponent<TransformComponent>();

				GUI::DisplayVector3Slider(tc.Position, "Position: ", 0.0f);
				GUI::DisplayVector3Slider(tc.Rotation, "Rotation: ", 0.0f);
				GUI::DisplayVector3Slider(tc.Scale, "Scale: ", 0.0f);
			}

			if (entity.HasComponent<SpriteComponent>()) {
				if (ImGui::CollapsingHeader(SW_ICON_IMAGE_SIZE_SELECT_ACTUAL "  Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
					SpriteComponent& sc = entity.GetComponent<SpriteComponent>();

					GUI::DisplayColorPicker(sc.Color, "Color");
				}
			}

			OnEnd();
		}
	}

}
