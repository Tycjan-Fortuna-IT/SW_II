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
		const glm::vec2 vo = (*m_Spritesheet)->ViewPos;

		ImVec2 viewOrigin = { vo.x, vo.y };
		m_Canvas.SetView(viewOrigin, (*m_Spritesheet)->ViewZoom);

		ImGuiIO& io = ImGui::GetIO();

		ImVec2 availableRegion = ImGui::GetContentRegionAvail();

		static ImVec2 drawStartPoint;
		static bool isDragging = false;

		constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_NoPadOuterX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerH
			| ImGuiTableFlags_ScrollY;

		f32 wz = (*m_Spritesheet)->ViewZoom;

		if (ImGui::BeginTable("MainViewTable", 2, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			GUI::BeginProperties("##spritesheet_editor_properties");

			GUI::DrawFloatingPointProperty((*m_Spritesheet)->ViewZoom, "View Zoom", nullptr, 0.5f, 15.0f);
			GUI::DrawFloatingPointProperty((*m_Spritesheet)->GridScale, "Grid Scale", nullptr, 16.f, 1000.f);
			GUI::DrawVector2ControlProperty((*m_Spritesheet)->CenterOffset, "Center Offset");

			static bool displayImageBorders = false;
			GUI::DrawBooleanProperty(displayImageBorders, "Show Image Borders");

			Asset* asset = (*m_Spritesheet)->GetSpritesheetTexture();
			if (GUI::DrawAssetDropdownProperty<Texture2D>(&asset, "Spritesheet")) {
				Texture2D** texture = asset ? AssetManager::GetAssetRaw<Texture2D>(asset->GetHandle()) : nullptr;

				(*m_Spritesheet)->SetSpritesheetTexture(texture);
			}

			(*m_Spritesheet)->ViewPos = glm::vec2{ viewOrigin.x, viewOrigin.y };

			GUI::EndProperties();

			RenderSpriteCards((*m_Spritesheet)->GridScale);
			
			ImGui::TableNextColumn();

			if (m_Canvas.Begin("##mycanvas", ImGui::GetContentRegionAvail())) {
				if ((isDragging || ImGui::IsItemHovered()) && ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f)) {
					if (!isDragging) {
						isDragging = true;
						drawStartPoint = viewOrigin;
					}

					const ImVec2 nvp = drawStartPoint + ImGui::GetMouseDragDelta(1, 0.0f) * (*m_Spritesheet)->ViewZoom;

					(*m_Spritesheet)->ViewPos = glm::vec2{ nvp.x, nvp.y };
				} else if (isDragging) {
					isDragging = false;
				}

				ImRect viewRect = m_Canvas.ViewRect();

				glm::vec2 offset = (*m_Spritesheet)->CenterOffset;
				const ImVec2 lineOffset = { offset.x, offset.y };

				ImVec2 loffset = m_Canvas.ViewOrigin() * (1.0f / wz) + lineOffset;
				ImVec2 viewPos = m_Canvas.ViewRect().Min + lineOffset;
				ImVec2 viewSize = m_Canvas.ViewRect().GetSize();
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				f32 gridScale = (*m_Spritesheet)->GridScale;

				{
					ImRect rect = m_Canvas.ViewRect();

					drawList->AddRectFilled(rect.Min, rect.Max, GUI::Theme::BackgroundDark);
				}

				for (f32 x = fmodf(loffset.x, gridScale); x < viewSize.x; x += gridScale) {
					if (x < 0) continue;
				
					drawList->AddLine(ImVec2(x - offset.x, 0.0f - offset.y) + viewPos, ImVec2(x - offset.x, viewSize.y - offset.y) + viewPos, GUI::Theme::TextBrighter);
				}

				for (f32 y = fmodf(loffset.y, gridScale); y < viewSize.y; y += gridScale) {
					if (y < 0) continue;

					drawList->AddLine(ImVec2(0.0f - offset.x, y - offset.y) + viewPos, ImVec2(viewSize.x - offset.x, y - offset.y) + viewPos, GUI::Theme::TextBrighter);
				}

				Texture2D* spritesheetTexture = (*m_Spritesheet)->GetSpritesheetTexture();
				if (!spritesheetTexture)
					spritesheetTexture = EditorResources::MissingAssetIcon;

				const ImVec4 borderCol = displayImageBorders ?
					ImGui::ColorConvertU32ToFloat4(GUI::Theme::Accent) : ImVec4(1.f, 1.f, 1.f, 1.f);

				ImGui::Image(
					GUI::GetTextureID(spritesheetTexture->GetTexHandle()), { (f32)spritesheetTexture->GetWidth() - 1.f, (f32)spritesheetTexture->GetHeight() - 1.f },
					{ 0, 1 }, { 1, 0 }, ImVec4(1.f, 1.f, 1.f, 1.f), borderCol
				);

				for (const Sprite& sprite : (*m_Spritesheet)->Sprites) {
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

	void SpritesheetEditor::DrawSpriteRectOnCanvas(ImDrawList* drawList, const Sprite& sprite) const
	{
		const glm::vec2 position = sprite.Position;
		const glm::vec2 scale = sprite.Scale;
		const glm::vec2 offset = (*m_Spritesheet)->CenterOffset;
		const f32 gridSize = (*m_Spritesheet)->GridScale;

		const ImVec2 topLeft = { position.x + offset.x, position.y + offset.y };
		const ImVec2 topRight = { position.x + offset.x + scale.x * gridSize, position.y + offset.y };
		const ImVec2 bottomRight = { position.x + offset.x + scale.x * gridSize, position.y + offset.y + scale.y * gridSize };
		const ImVec2 bottomLeft = { position.x + offset.x, position.y + offset.y + scale.y * gridSize };

		drawList->AddLine(topLeft, topRight, GUI::Theme::MissingMesh);
		drawList->AddLine(topRight, bottomRight, GUI::Theme::MissingMesh);
		drawList->AddLine(bottomRight, bottomLeft, GUI::Theme::MissingMesh);
		drawList->AddLine(bottomLeft, topLeft, GUI::Theme::MissingMesh);
	}

	void SpritesheetEditor::RenderSpriteCards(f32 vscale)
	{
		constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_NoPadOuterX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerH
			| ImGuiTableFlags_ScrollY;

		constexpr ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_FramePadding;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 16.f);
		if (ImGui::Button("Add new sprite"))
			AddNewSprite();
		
		ImGui::SameLine();

		if (ImGui::Button("Save"))
			OnClose(); // serialize

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

			std::vector<Sprite>& sprites = (*m_Spritesheet)->Sprites;
			for (int i = 0; i < sprites.size(); i++) {
				Sprite& sprite = sprites[i];

				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				if (!spriteFilter.FilterPass(sprite.Name))
					continue;

				const char* spriteName = sprite.Name.c_str();
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

					GUI::DrawSingleLineTextInputProperty(sprite.Name, "Sprite name");
					GUI::DrawVector2ControlProperty(sprite.Position, "Position");
					GUI::DrawVector2ControlProperty(sprite.Scale, "Scale", nullptr, 1.f, 1.f, 20.f);
					GUI::DrawVector4ColorPickerProperty(sprite.Tint, "Tint");
					glm::vec2 position = sprite.Position + (*m_Spritesheet)->CenterOffset;
					GUI::DrawImagePartProperty(
						(*m_Spritesheet)->GetSpritesheetTexture(), "Sprite", nullptr, position, sprite.Scale, sprite.Tint, vscale
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

	void SpritesheetEditor::AddNewSprite()
	{
		(*m_Spritesheet)->Sprites.emplace_back(Sprite{ Random::CreateTag() });
	}

}
