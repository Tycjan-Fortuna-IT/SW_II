#include "SceneHierarchyPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "Core/ECS/EntityRegistry.hpp"
#include "Managers/SelectionManager.hpp"
#include "GUI/Icons.hpp"
#include "SceneViewportPanel.hpp"
#include "Core/Project/ProjectContext.hpp"

namespace SW {

	SceneHierarchyPanel::SceneHierarchyPanel(SceneViewportPanel* sceneViewportPanel)
		: Panel("Scene Hierarchy", SW_ICON_VIEW_LIST, true), m_SceneViewportPanel(sceneViewportPanel)
	{
		EventSystem::Register(EVENT_CODE_KEY_PRESSED, nullptr, [this](Event event, void* sender, void* listener) -> bool {
			KeyCode code = (KeyCode)event.Payload.u16[0];

			return OnKeyPressed(code);
		});
	}

	void SceneHierarchyPanel::OnUpdate(Timestep dt)
	{

	}

	void SceneHierarchyPanel::OnRender()
	{
		PROFILE_FUNCTION();

		GUI::ScopedStyle CellPadding(ImGuiStyleVar_CellPadding, ImVec2(0, 0));

		if (OnBegin(ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar)) {
			if (m_SceneViewportPanel->IsSceneLoaded()) {
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

				constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_ContextMenuInBody
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

					for (auto&& [handle, idc, tc, rsc] : view.each()) {
						if (!rsc.ParentID) {
							const Entity entity = { handle,  m_SceneViewportPanel->GetCurrentScene() };
							RenderEntityNode(entity, idc.ID, tc, rsc);
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
							if (!m_DraggedEntityTarget.IsChildOf(m_DraggedEntity)) {
								m_DraggedEntity.SetParent(m_DraggedEntityTarget);
							}

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
				ImGui::Text("No scene selected...");
			}

			OnEnd();
		}
	}

	ImRect SceneHierarchyPanel::RenderEntityNode(Entity entity, u64 id, TagComponent& tc, const RelationshipComponent& rsc, u32 depth)
	{
		if (!entity || !m_SearchFilter.FilterPass(tc.Tag))
			return { 0,0,0,0 };

		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		const bool selected = SelectionManager::GetSelectionID() == id;

		ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth
			| ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_AllowOverlap;

		if (selected) {
			ImGui::PushStyleColor(ImGuiCol_Header, ImGui::ColorConvertU32ToFloat4(GUI::Theme::TabActive));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::ColorConvertU32ToFloat4(GUI::Theme::TabActive));
		}

		const u64 childrenSize = rsc.ChildrenIDs.size();
		if (childrenSize == 0) {
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		}

		const bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(id), flags, "%s %s", SW_ICON_CUBE_OUTLINE, tc.Tag.c_str());

		if (selected)
			ImGui::PopStyleColor(2);

		const bool additionalReq = SelectionManager::IsSelected() ? ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) : true;

		if (!ImGui::IsItemToggledOpen() && ImGui::IsItemClicked(ImGuiMouseButton_Left) && additionalReq) {
			if (selected) {
				SelectionManager::Deselect();
			} else {
				SelectionManager::SelectByID(id);
			}
		}

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItemEx("Rename", SW_ICON_RENAME_BOX, "F2"))
				m_RenamingEntity = entity;

			if (ImGui::MenuItemEx("Duplicate", SW_ICON_CONTENT_DUPLICATE, "Ctrl+D")) {

				std::unordered_map<u64, Entity> duplicatedEntities;

				m_SceneViewportPanel->GetCurrentScene()->DuplicateEntity(entity, duplicatedEntities);
			}

			if (ImGui::MenuItemEx("Delete", SW_ICON_DELETE, "Del")) {
				if (SelectionManager::GetSelectionID() == id)
					SelectionManager::Deselect();

				m_EntityToDelete = entity;
			}

			ImGui::Separator();

			DrawEntityCreateMenu(m_SceneViewportPanel->GetCurrentScene(), entity);

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
				ImGui::TextUnformatted(tc.Tag.c_str());
				ImGui::EndDragDropSource();
			}
		}

		if (entity == m_RenamingEntity) {
			static bool renaming = false;

			if (!renaming) {
				renaming = true;
				ImGui::SetKeyboardFocusHere();
			}

			char buffer[256];

			memcpy(buffer, tc.Tag.c_str(), std::min(sizeof(buffer), tc.Tag.size() + 1));

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tc.Tag = buffer;

			if (ImGui::IsItemDeactivated()) {
				renaming = false;

				m_RenamingEntity = {};
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
					const ImRect childRect = RenderEntityNode(child, childId, tc, rsc, depth + 1);
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

	void SceneHierarchyPanel::DrawEntityCreateMenu(Scene* scene, Entity toParent)
	{
		Entity entity = {};

		if (ImGui::MenuItemEx("Empty Entity", SW_ICON_CUBE_OUTLINE)) {
			entity = scene->CreateEntity("Entity");

			if (toParent) entity.SetParent(toParent);
		}

		if (ImGui::BeginMenuEx("2D", SW_ICON_ARRANGE_BRING_FORWARD)) {

			if (ImGui::MenuItemEx("Sprite", SW_ICON_IMAGE_SIZE_SELECT_ACTUAL)) {
				entity = scene->CreateEntity("Sprite");
				entity.AddComponent<SpriteComponent>();

				if (toParent) entity.SetParent(toParent);

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItemEx("Circle", SW_ICON_CHECKBOX_BLANK_CIRCLE)) {
				entity = scene->CreateEntity("Circle");
				entity.AddComponent<CircleComponent>();

				if (toParent) entity.SetParent(toParent);

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItemEx("Text", SW_ICON_FORMAT_TEXT)) {
				entity = scene->CreateEntity("Text");
				entity.AddComponent<TextComponent>();

				if (toParent) entity.SetParent(toParent);

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItemEx("Box Collider", SW_ICON_CHECKBOX_BLANK_OUTLINE)) {
				entity = scene->CreateEntity("Box Collider");
				entity.AddComponent<SpriteComponent>();
				entity.AddComponent<RigidBody2DComponent>();
				entity.AddComponent<BoxCollider2DComponent>();

				if (toParent) entity.SetParent(toParent);

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItemEx("Circle Collider", SW_ICON_CHECKBOX_BLANK_CIRCLE_OUTLINE)) {
				entity = scene->CreateEntity("Circle Collider");
				entity.AddComponent<CircleComponent>();
				entity.AddComponent<RigidBody2DComponent>();
				entity.AddComponent<CircleCollider2DComponent>();

				if (toParent) entity.SetParent(toParent);

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItemEx("Camera", SW_ICON_CAMERA)) {
				entity = scene->CreateEntity("Camera");

				SceneCamera camera(m_SceneViewportPanel->GetViewportAspectRatio());

				entity.AddComponent<CameraComponent>(camera);

				if (toParent) entity.SetParent(toParent);

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenuEx("3D", SW_ICON_PACKAGE_VARIANT_CLOSED)) {

			ImGui::EndMenu();
		}
	}

	bool SceneHierarchyPanel::OnKeyPressed(KeyCode code)
	{
		if (!SelectionManager::IsSelected())
			return false;

		Entity entity = m_SceneViewportPanel->GetCurrentScene()->GetEntityByID(SelectionManager::GetSelectionID());

		const bool ctrl = Input::IsKeyDown(KeyCode::LeftControl) || Input::IsKeyDown(KeyCode::RightControl);

		switch (code) {
			case KeyCode::F2:
			{
				m_RenamingEntity = entity;
				break;
			}
			case KeyCode::D:
			{
				if (ctrl) {
					std::unordered_map<u64, Entity> duplicatedEntities;

					m_SceneViewportPanel->GetCurrentScene()->DuplicateEntity(entity, duplicatedEntities);
				}
				break;
			}
			case KeyCode::Delete:
			{
				m_EntityToDelete = entity;
				SelectionManager::Deselect();
				break;
			}
		}

		return false;
	}

}
