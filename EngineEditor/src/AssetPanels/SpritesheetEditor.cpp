#include "SpritesheetEditor.hpp"

#include "GUI/Editor/EditorResources.hpp"
#include "Core/Asset/AssetLoader.hpp"

namespace SW {

	SpritesheetEditor::SpritesheetEditor(const char* name, const char* icon)
		: AssetEditorPanel(name, icon) {}

	void SpritesheetEditor::OnUpdate(Timestep ts)
	{

	}

	void SpritesheetEditor::OnWindowStylePush()
	{

	}

	void SpritesheetEditor::OnWindowStylePop()
	{

	}

	void SpritesheetEditor::OnOpen()
	{

	}

	void SpritesheetEditor::OnClose()
	{
		const AssetMetaData& metadata = AssetManager::GetAssetMetaData((*m_Spritesheet)->GetHandle());
		
		AssetLoader::Serialize(metadata);
	}

	void SpritesheetEditor::Render()
	{
		const glm::vec2 vo = (*m_Spritesheet)->GetViewPos();

		ImVec2 viewOrigin = { vo.x, vo.y };
		m_Canvas.SetView(viewOrigin, (*m_Spritesheet)->GetViewZoom());

		ImGuiIO& io = ImGui::GetIO();

		ImVec2 availableRegion = ImGui::GetContentRegionAvail();

		static ImVec2 drawStartPoint;
		static bool isDragging = false;

		constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_NoPadOuterX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerH
			| ImGuiTableFlags_ScrollY;

		if (ImGui::BeginTable("MainViewTable", 2, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			GUI::BeginProperties("##spritesheet_editor_properties");
			
			f32 viewZoom = (*m_Spritesheet)->GetViewZoom();
			if (GUI::DrawFloatingPointProperty(viewZoom, "View Zoom", nullptr, 0.5f, 15.0f)) {
				(*m_Spritesheet)->SetViewZoom(viewZoom);
			}

			f32 gridScale = (*m_Spritesheet)->GetGridScale();
			if (GUI::DrawFloatingPointProperty(gridScale, "Grid Scale", nullptr, 16.f, 1000.f)) {
				(*m_Spritesheet)->SetGridScale(gridScale);
			}

			glm::vec2 offset = (*m_Spritesheet)->GetCenterOffset();
			if (GUI::DrawVector2ControlProperty(offset, "Center Offset")) {
				(*m_Spritesheet)->SetCenterOffset(offset);
			}

			Asset* asset = (*m_Spritesheet)->GetSpritesheetTexture();
			if (GUI::DrawAssetDropdownProperty<Texture2D>(&asset, "Spritesheet")) {
				Texture2D** texture = asset ? AssetManager::GetAssetRaw<Texture2D>(asset->GetHandle()) : nullptr;

				(*m_Spritesheet)->SetSpritesheetTexture(texture);
			}

			(*m_Spritesheet)->SetViewPos({ viewOrigin.x, viewOrigin.y });

			GUI::EndProperties();

			RenderSpriteCards(gridScale);
			
			ImGui::TableNextColumn();

			if (m_Canvas.Begin("##mycanvas", ImGui::GetContentRegionAvail())) {
				if ((isDragging || ImGui::IsItemHovered()) && ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f)) {
					if (!isDragging) {
						isDragging = true;
						drawStartPoint = viewOrigin;
					}

					const ImVec2 nvp = drawStartPoint + ImGui::GetMouseDragDelta(1, 0.0f) * viewZoom;

					(*m_Spritesheet)->SetViewPos({nvp.x, nvp.y});
				} else if (isDragging) {
					isDragging = false;
				}

				ImRect viewRect = m_Canvas.ViewRect();

				const ImVec2 lineOffset = { offset.x, offset.y };

				ImVec2 loffset = m_Canvas.ViewOrigin() * (1.0f / viewZoom) - lineOffset;
				ImVec2 viewPos = m_Canvas.ViewRect().Min + lineOffset;
				ImVec2 viewSize = m_Canvas.ViewRect().GetSize();
				ImDrawList* drawList = ImGui::GetWindowDrawList();

				drawList->AddRectFilled(viewPos, viewPos + viewSize, GUI::Theme::SelectionMuted);

				for (f32 x = fmodf(loffset.x, gridScale); x < viewSize.x; x += gridScale)
					drawList->AddLine(ImVec2(x - offset.x, 0.0f - offset.y) + viewPos, ImVec2(x - offset.x, viewSize.y - offset.y) + viewPos, GUI::Theme::Selection);
				for (f32 y = fmodf(loffset.y, gridScale); y < viewSize.y; y += gridScale)
					drawList->AddLine(ImVec2(0.0f - offset.x, y - offset.y) + viewPos, ImVec2(viewSize.x - offset.x, y - offset.y) + viewPos, GUI::Theme::Selection);

				/*if (viewRect.Max.x > 0.0f)
					GUI::DrawScale(ImVec2(0.0f, 0.0f), ImVec2(viewRect.Max.x, 0.0f), gridScale, 16.0f, 0.6f);
				if (viewRect.Min.x < 0.0f)
					GUI::DrawScale(ImVec2(0.0f, 0.0f), ImVec2(viewRect.Min.x, 0.0f), gridScale, 16.0f, 0.6f, -1.0f);
				if (viewRect.Max.y > 0.0f)
					GUI::DrawScale(ImVec2(0.0f, 0.0f), ImVec2(0.0f, viewRect.Max.y), gridScale, 16.0f, 0.6f);
				if (viewRect.Min.y < 0.0f)
					GUI::DrawScale(ImVec2(0.0f, 0.0f), ImVec2(0.0f, viewRect.Min.y), gridScale, 16.0f, 0.6f, -1.0f);*/

				Texture2D* spritesheetTexture = (*m_Spritesheet)->GetSpritesheetTexture();
				if (!spritesheetTexture)
					spritesheetTexture = EditorResources::MissingAssetIcon;

				ImGui::Image(
					GUI::GetTextureID(spritesheetTexture->GetTexHandle()), { (f32)spritesheetTexture->GetWidth(), (f32)spritesheetTexture->GetHeight() }, { 0, 1 }, { 1, 0 }
				);

				for (const Sprite* sprite : (*m_Spritesheet)->GetSprites()) {
					DrawSpriteRectOnCanvas(drawList, sprite);
				}

				m_Canvas.End();
			}

			ImGui::EndTable();
		}
	}

	void SpritesheetEditor::SetAssetHandle(AssetHandle handle)
	{
		m_Spritesheet = AssetManager::GetAssetRaw<Spritesheet>(handle);
	}

	void SpritesheetEditor::DrawSpriteRectOnCanvas(ImDrawList* drawList, const Sprite* sprite) const
	{
		const glm::vec2 position = sprite->Position;
		const glm::vec2 scale = sprite->Scale * (*m_Spritesheet)->GetViewZoom();
		const glm::vec2 offset = (*m_Spritesheet)->GetCenterOffset();

		ImVec2 topLeft = ImVec2(position.x + offset.x, position.y + offset.y);
		ImVec2 topRight = ImVec2(position.x + scale.x + offset.x, position.y + offset.y);
		ImVec2 bottomRight = ImVec2(position.x + scale.x + offset.x, position.y + scale.y + offset.y);
		ImVec2 bottomLeft = ImVec2(position.x + offset.x, position.y + scale.y + offset.y);

		drawList->AddLine(topLeft, topRight, GUI::Theme::SelectionHalfMuted);
		drawList->AddLine(topRight, bottomRight, GUI::Theme::SelectionHalfMuted);
		drawList->AddLine(bottomRight, bottomLeft, GUI::Theme::SelectionHalfMuted);
		drawList->AddLine(bottomLeft, topLeft, GUI::Theme::SelectionHalfMuted);
	}

	void SpritesheetEditor::RenderSpriteCards(f32 scale)
	{
		constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_NoPadOuterX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerH
			| ImGuiTableFlags_ContextMenuInBody
			| ImGuiTableFlags_ScrollY;

		constexpr ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_FramePadding;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 16.f);
		if (ImGui::Button("Save spritesheet")) {

		}
		ImGui::SameLine();
		if (ImGui::Button("Add new sprite")) {

		}
		static GUI::TextFilter spriteFilter;

		const f32 availableWidth = ImGui::GetContentRegionAvail().x - 16.f - 16.f;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 16.f);
		ImGui::SetNextItemWidth(availableWidth);
		spriteFilter.OnRender("  " SW_ICON_MAGNIFY "  Search ... ");

		if (ImGui::BeginTable("SideViewTable", 1, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			int indexToRemove = -1;

			if (!(*m_Spritesheet)->GetSpritesheetTexture()) {
				ImGui::EndTable();
				return;
			}

			const std::vector<Sprite*> sprites = (*m_Spritesheet)->GetSprites();

			for (int i = 0; i < sprites.size(); i++) {
				Sprite* sprite = sprites[i];

				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				if (!spriteFilter.FilterPass(sprite->Name))
					continue;

				const char* spriteName = sprite->Name.c_str();
				const std::string uid = std::to_string(i);
				const char* uidc = uid.c_str();
				
				bool opened = ImGui::TreeNodeEx(uidc, treeFlags, "");

				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::Selection);
				ImGui::TextUnformatted(SW_ICON_TEXTURE);
				ImGui::PopStyleColor();
				ImGui::SameLine();
				ImGui::TextUnformatted(spriteName);

				const f32 frameHeight = ImGui::GetFrameHeight();

				ImGui::SameLine(ImGui::GetContentRegionMax().x - frameHeight * 1.2f);

				if (ImGui::Button(SW_ICON_SETTINGS, ImVec2{ frameHeight * 1.2f, frameHeight }))
					ImGui::OpenPopup("RemoveSprite");

				if (ImGui::BeginPopup("RemoveSprite")) {
					if (ImGui::MenuItemEx("Remove", SW_ICON_DELETE)) {
						indexToRemove = i;
					}

					ImGui::EndPopup();
				}

				if (opened) {
					GUI::BeginProperties(uidc);
					GUI::DrawSingleLineTextInputProperty(sprite->Name, "Sprite name");
					GUI::DrawVector2ControlProperty(sprite->Position, "Position");
					GUI::DrawVector2ControlProperty(sprite->Scale, "Scale", nullptr, 1.f, 1.f, 20.f);
					GUI::DrawVector4ColorPickerProperty(sprite->Tint, "Tint");
					GUI::DrawImagePartProperty(
						(*m_Spritesheet)->GetSpritesheetTexture(), "Sprite", nullptr, sprite->Position, sprite->Scale, sprite->Tint, scale
					);
					GUI::EndProperties();

					ImGui::TreePop();
				}
			}
			
			if (indexToRemove != -1) {
				//m_Sprites.erase(m_Sprites.begin() + indexToRemove);
			}

			ImGui::EndTable();
		}
	}

}
