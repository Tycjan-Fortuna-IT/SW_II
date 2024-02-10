#include "SceneHierarchyPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "Core/ECS/EntityRegistry.hpp"
#include "Managers/SelectionManager.hpp"
#include "GUI/Icons.hpp"
#include "SceneViewportPanel.hpp"

namespace SW {

	SceneHierarchyPanel::SceneHierarchyPanel(SceneViewportPanel* sceneViewportPanel)
		: Panel("Scene Hierarchy", SW_ICON_VIEW_LIST, true), m_SceneViewportPanel(sceneViewportPanel) {}

	void SceneHierarchyPanel::OnUpdate(Timestep dt)
	{

	}

	void SceneHierarchyPanel::OnRender()
	{
		GUI::ScopedStyle CellPadding(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

		if (OnBegin(ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar)) {
			const f32 lineHeight = ImGui::GetTextLineHeight();
			const ImVec2 padding = ImGui::GetStyle().FramePadding;

			Scene* currentScene = m_SceneViewportPanel->GetCurrentScene();

			m_SearchFilter.OnRender("###HierarchyFilter");

			ImGui::SameLine();

			if (GUI::Button("{} Add", { 90.f, 30.f }, SW_ICON_PLUS)) {
				ImGui::OpenPopup("AddEntity_Popup");
			}

			if (ImGui::BeginPopup("AddEntity_Popup")) {
				if (ImGui::MenuItemEx("Empty Entity", SW_ICON_CUBE_OUTLINE)) {
					currentScene->CreateEntity("Entity");
				}

				if (ImGui::BeginMenu("2D")) {
					if (ImGui::MenuItemEx("Sprite", SW_ICON_IMAGE_SIZE_SELECT_ACTUAL)) {
						Entity entity = currentScene->CreateEntity("Sprite");
						entity.AddComponent<SpriteComponent>();
						
						ImGui::CloseCurrentPopup();
					}

					if (ImGui::MenuItemEx("Circle", SW_ICON_CHECKBOX_BLANK_CIRCLE)) {
						Entity entity = currentScene->CreateEntity("Circle");
						entity.AddComponent<CircleComponent>();

						ImGui::CloseCurrentPopup();
					}

					if (ImGui::MenuItemEx("Camera", SW_ICON_CAMERA)) {
						Entity entity = currentScene->CreateEntity("Camera");
						
						SceneCamera camera(m_SceneViewportPanel->GetViewportAspectRatio());

						entity.AddComponent<CameraComponent>(camera);

						ImGui::CloseCurrentPopup();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("3D")) {

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}

			constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg
				| ImGuiTableFlags_ContextMenuInBody
				| ImGuiTableFlags_BordersInner
				| ImGuiTableFlags_ScrollY;

			if (ImGui::BeginTable("HierarchyTable", 3, tableFlags)) {
				ImGui::TableSetupColumn(" Label", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoClip);
				ImGui::TableSetupColumn(" Type", ImGuiTableColumnFlags_WidthFixed, lineHeight * 3.0f);
				ImGui::TableSetupColumn("  " SW_ICON_EYE, ImGuiTableColumnFlags_WidthFixed, lineHeight * 2.0f);

				ImGui::TableSetupScrollFreeze(0, 1);

				ImGui::TableNextRow(ImGuiTableRowFlags_Headers, ImGui::GetFrameHeight());

				for (int column = 0; column < 3; ++column) {
					ImGui::TableSetColumnIndex(column);
					const char* column_name = ImGui::TableGetColumnName(column);
					ImGui::PushID(column);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding.y);
					ImGui::TableHeader(column_name);
					ImGui::PopID();
				}

				const auto& view = m_SceneViewportPanel->GetCurrentScene()->GetRegistry().GetEntitiesWith<IDComponent, TagComponent>();

				for (auto&& [handle, idc, tc] : view.each()) {
					const Entity entity = { handle,  m_SceneViewportPanel->GetCurrentScene() };

					RenderEntityNode(entity, idc, tc);
				}

				ImGui::EndTable();
			}

			OnEnd();
		}
	}

	void SceneHierarchyPanel::RenderEntityNode(Entity entity, const IDComponent& idc, const TagComponent& tc)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		const std::string tag = tc.Tag;

		if (!m_SearchFilter.FilterPass(tag)) {
			return;
		}

		const bool selected = SelectionManager::GetSelectionID() == idc.ID;

		ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth
			| ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAllColumns
			| ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		if (selected) {
			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(Color::DarkGray));
			ImGui::PushStyleColor(ImGuiCol_Header, Color::DarkGray);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Color::DarkGray);
		}

		const bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(idc.ID), flags, "%s %s", SW_ICON_CUBE_OUTLINE, tag.c_str());

		if (selected)
			ImGui::PopStyleColor(2);

		if (!ImGui::IsItemToggledOpen() && ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			if (selected) {
				SelectionManager::Deselect();
			} else {
				SelectionManager::SelectByID(idc.ID);
			}
		}

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItemEx("Delete", SW_ICON_DELETE, "Del")) {
				if (SelectionManager::GetSelectionID() == idc.ID) {
					SelectionManager::Deselect();

					m_SceneViewportPanel->GetCurrentScene()->DestroyEntity(entity);
				}
			}

			ImGui::EndPopup();
		}

		ImGui::TableNextColumn();
		ImGui::TextUnformatted("  Entity");
		ImGui::TableNextColumn();
		ImGui::Text("  %s ", true ? SW_ICON_EYE : SW_ICON_EYE_OFF);
	}

}
