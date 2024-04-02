#include "SpritesheetEditor.hpp"

#include <imgui_canvas.h>

#include "GUI/Editor/EditorResources.hpp"

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

	}

	template <typename T>
		requires std::is_base_of_v<Asset, T>
	static bool DrawAssetDropdownProperty(
		Asset** asset, const char* label, const char* tooltip = nullptr
	) {
		bool changed = false;

		std::string tag = "none";

		if (*asset)
			tag = AssetManager::GetAssetMetaData((*asset)->GetHandle()).Path.filename().string();

		GUI::BeginPropertyGrid(label, tooltip, true);

		ImVec2 region = ImGui::GetContentRegionAvail();
		region.x -= 20.0f;
		region.y = ImGui::GetFrameHeight();

		ImVec2 pos = ImGui::GetCursorPos();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_Button]);

		ImGui::Button("##font_dropdown_property", region);

		ImGui::PopStyleColor();

		if (ImGui::BeginDragDropTarget()) {
			const char* payloadName = Asset::GetStringifiedAssetType(T::GetStaticType());

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadName)) {
				u64* handle = static_cast<u64*>(payload->Data);

				*asset = AssetManager::GetAssetRaw(*handle);

				changed = true;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });

		if (ImGui::Button("x", { 20.0f, region.y })) {
			*asset = nullptr;
			changed = true;
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();


		if (*asset) {
			ImVec4 selectedColor = GUI::Colors::Darken(ImVec4(0.6666666865348816f, 0.686274528503418f, 0.0784313753247261f, 1.0f), 0.05f);
			ImGui::PushStyleColor(ImGuiCol_Text, selectedColor);
		}

		ImVec2 padding = ImGui::GetStyle().FramePadding;
		ImGui::SetCursorPos({ pos.x + padding.x, pos.y + padding.y });
		ImGui::Text("%s", tag.c_str());

		if (*asset)
			ImGui::PopStyleColor();

		GUI::EndPropertyGrid();

		return changed;
	}

	void SpritesheetEditor::Render()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImVec2 availableRegion = ImGui::GetContentRegionAvail();

		static ImGuiEx::Canvas canvas;
		static ImVec2 drawStartPoint;
		static bool isDragging = false;

		ImVec2 viewOrigin = canvas.ViewOrigin();

		constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_NoPadOuterX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerH
			| ImGuiTableFlags_ScrollY;

		if (ImGui::BeginTable("MainViewTable", 2, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			GUI::BeginProperties("##spritesheet_editor_properties");
			
			f32 viewZoom = m_Spritesheet->GetViewZoom();
			if (GUI::DrawFloatingPointProperty(viewZoom, "View Zoom", nullptr, 0.5f, 15.0f)) {
				m_Spritesheet->SetViewZoom(viewZoom);
				canvas.SetView(viewOrigin, viewZoom);
			}

			f32 gridScale = m_Spritesheet->GetGridScale();
			if (GUI::DrawFloatingPointProperty(gridScale, "Grid Scale", nullptr, 16.f, 1000.f)) {
				m_Spritesheet->SetGridScale(gridScale);
			}

			glm::vec2 offset = m_Spritesheet->GetCenterOffset();
			if (GUI::DrawVector2ControlProperty(offset, "Center Offset")) {
				m_Spritesheet->SetCenterOffset(offset);
			}

			AssetHandle handle = m_Spritesheet->GetSpritesheetTextureHandle();
			Asset* asset = nullptr;
			if (handle) {
				asset = AssetManager::GetAssetRaw(handle);
			}
			if (DrawAssetDropdownProperty<Texture2D>(&asset, "Spritesheet")) {
				m_Spritesheet->SetSpritesheetTextureHandle(asset->GetHandle());
			}

			m_Spritesheet->SetViewPos({ viewOrigin.x, viewOrigin.y });

			GUI::EndProperties();

			RenderSpriteCards(gridScale);
			
			ImGui::TableNextColumn();

			if (canvas.Begin("##mycanvas", ImGui::GetContentRegionAvail())) {
				if ((isDragging || ImGui::IsItemHovered()) && ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f)) {
					if (!isDragging) {
						isDragging = true;
						drawStartPoint = viewOrigin;
					}

					canvas.SetView(drawStartPoint + ImGui::GetMouseDragDelta(1, 0.0f) * viewZoom, viewZoom);
				}
				else if (isDragging) {
					isDragging = false;
				}

				ImRect viewRect = canvas.ViewRect();

				const ImVec2 lineOffset = { offset.x, offset.y };

				ImVec2 loffset = canvas.ViewOrigin() * (1.0f / viewZoom) - lineOffset;
				ImVec2 viewPos = canvas.ViewRect().Min + lineOffset;
				ImVec2 viewSize = canvas.ViewRect().GetSize();
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

				Texture2D* spritesheetTexture = nullptr;
				if (m_Spritesheet->GetSpritesheetTextureHandle())
					spritesheetTexture = AssetManager::GetAssetRaw<Texture2D>(m_Spritesheet->GetSpritesheetTextureHandle());
				else
					spritesheetTexture = EditorResources::MissingAssetIcon;

				ImGui::Image(
					GUI::GetTextureID(spritesheetTexture->GetHandle()), { (f32)spritesheetTexture->GetWidth(), (f32)spritesheetTexture->GetHeight() }, { 0, 1 }, { 1, 0 }
				);

				for (const Sprite* sprite : m_Spritesheet->GetSprites()) {
					DrawSpriteRectOnCanvas(drawList, sprite);
				}

				canvas.End();
			}

			ImGui::EndTable();
		}
	}

	void SpritesheetEditor::SetAsset(Asset* asset)
	{
		m_Spritesheet = asset->AsRaw<Spritesheet>();
	}

	void SpritesheetEditor::DrawSpriteRectOnCanvas(ImDrawList* drawList, const Sprite* sprite) const
	{
		const glm::vec2 position = sprite->Position;
		const glm::vec2 scale = sprite->Scale * m_Spritesheet->GetViewZoom();
		const glm::vec2 offset = m_Spritesheet->GetCenterOffset();

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

			if (!m_Spritesheet->GetSpritesheetTextureHandle()) {
				ImGui::EndTable();
				return;
			}

			const std::vector<Sprite*> sprites = m_Spritesheet->GetSprites();

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
					Texture2D* texture = AssetManager::GetAssetRaw<Texture2D>(m_Spritesheet->GetSpritesheetTextureHandle());
					GUI::DrawImagePartProperty(
						texture, "Sprite", nullptr, sprite->Position, sprite->Scale, sprite->Tint, scale
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
