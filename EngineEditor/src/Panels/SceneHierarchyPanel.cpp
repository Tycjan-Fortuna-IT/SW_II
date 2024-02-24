#include "SceneHierarchyPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "Core/ECS/EntityRegistry.hpp"
#include "Managers/SelectionManager.hpp"
#include "GUI/Icons.hpp"
#include "SceneViewportPanel.hpp"
#include "Core/Project/ProjectContext.hpp"

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
			if (ProjectContext::HasContext()) {
				const f32 lineHeight = ImGui::GetTextLineHeight();
				const ImVec2 padding = ImGui::GetStyle().FramePadding;

				Scene* currentScene = m_SceneViewportPanel->GetCurrentScene();

				m_SearchFilter.OnRender("  " SW_ICON_MAGNIFY "  Search ... ");

				ImGui::SameLine();

				if (GUI::Button("{} Add", { 90.f, 30.f }, SW_ICON_PLUS)) {
					ImGui::OpenPopup("AddEntity_Popup");
				}

				if (ImGui::BeginPopup("AddEntity_Popup")) {
					DrawEntityCreateMenu(currentScene);

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

					const auto& view = m_SceneViewportPanel->GetCurrentScene()->GetRegistry()
						.GetEntitiesWith<IDComponent, TagComponent, RelationshipComponent>();

					//auto sortFn = [](const IDComponent& lhs, const IDComponent& rhs) -> bool {
					//	return lhs.ID < rhs.ID; // sorting from the lowest ID to highest ID
					//};

					//m_SceneViewportPanel->GetCurrentScene()->GetRegistry().GetRegistryHandle().sort<IDComponent>(sortFn);

					for (auto&& [handle, idc, tc, rsc] : view.each()) {
						if (!rsc.ParentID) {
							const Entity entity = { handle,  m_SceneViewportPanel->GetCurrentScene() };
							RenderEntityNode(entity, idc.ID, tc.Tag, rsc);
						}
					}

					if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
						DrawEntityCreateMenu(currentScene);

						ImGui::EndPopup();
					}

					
					ImGui::EndTable();

					if (m_DraggedEntity && m_DraggedEntityTarget) {
						if (m_DraggedEntity.IsChildOf(m_DraggedEntityTarget)) {
							m_DraggedEntity.RemoveParent();
						} else {
							m_DraggedEntity.SetParent(m_DraggedEntityTarget);
						}

						m_DraggedEntity = {};
						m_DraggedEntityTarget = {};
					}

					if (m_EntityToDelete) {
						m_SceneViewportPanel->GetCurrentScene()->DestroyEntity(m_EntityToDelete);
						m_EntityToDelete = {};
					}
				}

				const ImRect windowRect = { ImGui::GetItemRectMin(), ImGui::GetItemRectMax() };

				if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID)) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity")) {
						Entity toUnparent = *static_cast<Entity*>(payload->Data);
						toUnparent.RemoveParent();
					}

					ImGui::EndDragDropTarget();
				}

			} else {
				ImGui::Text("No project selected...");
			}

			OnEnd();
		}
	}

	ImRect SceneHierarchyPanel::RenderEntityNode(Entity entity, u64 id, const std::string& tag, const RelationshipComponent& rsc, u32 depth)
	{
		if (!entity || !m_SearchFilter.FilterPass(tag))
			return { 0,0,0,0 };

		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		const bool selected = SelectionManager::GetSelectionID() == id;

		ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth
			| ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_AllowOverlap;

		if (selected) {
			ImVec4 selectedColor = GUI::Colors::Darken(ImVec4(0.6666666865348816f, 0.686274528503418f, 0.0784313753247261f, 1.0f), 0.2f);

			ImGui::PushStyleColor(ImGuiCol_Header, selectedColor);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, selectedColor);
		}

		const u64 childrenSize = rsc.ChildrenIDs.size();
		if (childrenSize == 0) {
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}

		const bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(id), flags, "%s %s", SW_ICON_CUBE_OUTLINE, tag.c_str());

		if (selected)
			ImGui::PopStyleColor(2);

		if (!ImGui::IsItemToggledOpen() && ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			if (selected) {
				SelectionManager::Deselect();
			} else {
				SelectionManager::SelectByID(id);
			}
		}

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItemEx("Delete", SW_ICON_DELETE, "Del")) {
				if (SelectionManager::GetSelectionID() == id)
					SelectionManager::Deselect();

				m_EntityToDelete = entity;
			}

			ImGui::EndPopup();
		}

		ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();
		verticalLineStart.x -= 0.5f;
		verticalLineStart.y -= ImGui::GetFrameHeight() * 0.5f;

		{
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity")) {
					m_DraggedEntity = *static_cast<Entity*>(payload->Data);
					m_DraggedEntityTarget = entity;
				}

				ImGui::EndDragDropTarget();
			}

			if (ImGui::BeginDragDropSource()) {
				ImGui::SetDragDropPayload("Entity", &entity, sizeof(entity));
				ImGui::TextUnformatted(tag.c_str());
				ImGui::EndDragDropSource();
			}
		}

		const ImRect nodeRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

		if (opened) {
			static ImColor treeLineColor[2] = { ImColor(101, 173, 229), ImColor(239, 184, 57) };
			depth %= sizeof(treeLineColor) / sizeof(ImColor);
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			ImVec2 verticalLineEnd = verticalLineStart;
			constexpr f32 lineThickness = 1.5f;

			for (u64 childId : rsc.ChildrenIDs) {
				Scene* currentScene = m_SceneViewportPanel->GetCurrentScene();

				if (const Entity child = currentScene->GetEntityByID(childId)) {
					auto&& [tc, rsc] = child.GetAllComponents<TagComponent, RelationshipComponent>();

					const f32 HorizontalTreeLineSize = rsc.ChildrenIDs.empty() ? 18.0f : 9.0f;
					const ImRect childRect = RenderEntityNode(child, childId, tc.Tag, rsc, depth + 1);
					const f32 midpoint = (childRect.Min.y + childRect.Max.y) / 2.0f;

					drawList->AddLine(ImVec2(verticalLineStart.x, midpoint), ImVec2(verticalLineStart.x + HorizontalTreeLineSize, midpoint), treeLineColor[depth], lineThickness);
					
					verticalLineEnd.y = midpoint;
				}
			}

			drawList->AddLine(verticalLineStart, verticalLineEnd, treeLineColor[depth], lineThickness);
		}

		if (opened && childrenSize > 0)
			ImGui::TreePop();

		return nodeRect;
	}

	void SceneHierarchyPanel::DrawEntityCreateMenu(Scene* scene)
	{		
		if (ImGui::BeginMenu("Create new entity")) {

			if (ImGui::MenuItemEx("Empty Entity", SW_ICON_CUBE_OUTLINE)) {
				scene->CreateEntity("Entity");
			}

			if (ImGui::BeginMenuEx("2D", SW_ICON_ARRANGE_BRING_FORWARD)) {

				if (ImGui::MenuItemEx("Sprite", SW_ICON_IMAGE_SIZE_SELECT_ACTUAL)) {
					Entity entity = scene->CreateEntity("Sprite");
					entity.AddComponent<SpriteComponent>();

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItemEx("Circle", SW_ICON_CHECKBOX_BLANK_CIRCLE)) {
					Entity entity = scene->CreateEntity("Circle");
					entity.AddComponent<CircleComponent>();

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItemEx("Camera", SW_ICON_CAMERA)) {
					Entity entity = scene->CreateEntity("Camera");

					SceneCamera camera(m_SceneViewportPanel->GetViewportAspectRatio());

					entity.AddComponent<CameraComponent>(camera);

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenuEx("3D", SW_ICON_PACKAGE_VARIANT_CLOSED)) {

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
	}

}
