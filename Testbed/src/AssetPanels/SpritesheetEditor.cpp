#include "SpritesheetEditor.hpp"

#include <imgui_canvas.h>

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
		m_Sprites.emplace_back(SpriteData{ "Sprite1" });
		m_Sprites.emplace_back(SpriteData{ "Sprite2" });
		//m_Sprites.emplace_back(SpriteData{ "Sprite3" });
	}

	void SpritesheetEditor::OnClose()
	{

	}

	static void DrawRectOnCanvas(
		ImDrawList* drawList, const glm::vec2& position, const glm::vec2& scale
	) {
		ImVec2 topLeft = ImVec2(position.x, position.y);
		ImVec2 topRight = ImVec2(position.x + scale.x, position.y);
		ImVec2 bottomRight = ImVec2(position.x + scale.x, position.y + scale.y);
		ImVec2 bottomLeft = ImVec2(position.x, position.y + scale.y);

		drawList->AddLine(topLeft, topRight, GUI::Theme::NiceBlue);
		drawList->AddLine(topRight, bottomRight, GUI::Theme::NiceBlue);
		drawList->AddLine(bottomRight, bottomLeft, GUI::Theme::NiceBlue);
		drawList->AddLine(bottomLeft, topLeft, GUI::Theme::NiceBlue);
	}

	void SpritesheetEditor::Render()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImVec2 availableRegion = ImGui::GetContentRegionAvail();

		static ImGuiEx::Canvas canvas;
		static ImVec2 drawStartPoint;
		static bool isDragging = false;

		ImVec2 viewOrigin = canvas.ViewOrigin();
		f32 viewScale = canvas.ViewScale();

		constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_NoPadOuterX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerH
			| ImGuiTableFlags_ScrollY;

		if (ImGui::BeginTable("MainViewTable", 2, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			GUI::BeginProperties("##spritesheet_editor_properties");
			if (GUI::DrawFloatingPointProperty(viewScale, "View Zoom", nullptr, 0.5f, 15.0f)) {
				canvas.SetView(viewOrigin, viewScale);
			}
			static f32 scale = 100.f;
			GUI::DrawFloatingPointProperty(scale, "Scale", nullptr, 16.f, 1000.f);
			static glm::vec2 sOffset = glm::vec2(0.f);
			GUI::DrawVector2ControlProperty(sOffset, "Offset from center");

			GUI::EndProperties();

			RenderSpriteCards(scale);

			ImGui::TableNextColumn();

			if (canvas.Begin("##mycanvas", ImGui::GetContentRegionAvail())) {
				if ((isDragging || ImGui::IsItemHovered()) && ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f)) {
					if (!isDragging) {
						isDragging = true;
						drawStartPoint = viewOrigin;
					}

					canvas.SetView(drawStartPoint + ImGui::GetMouseDragDelta(1, 0.0f) * viewScale, viewScale);
				} else if (isDragging) {
					isDragging = false;
				}

				ImRect viewRect = canvas.ViewRect();

				const ImVec2 lineOffset = { sOffset.x, sOffset.y };

				ImVec2 offset = canvas.ViewOrigin() * (1.0f / canvas.ViewScale()) - lineOffset;
				ImVec2 viewPos = canvas.ViewRect().Min + lineOffset;
				ImVec2 viewSize = canvas.ViewRect().GetSize();
				ImDrawList* drawList = ImGui::GetWindowDrawList();

				drawList->AddRectFilled(viewPos, viewPos + viewSize, GUI::Theme::Background);

				for (f32 x = fmodf(offset.x, scale); x < viewSize.x; x += scale)
					drawList->AddLine(ImVec2(x - sOffset.x, 0.0f - sOffset.y) + viewPos, ImVec2(x - sOffset.x, viewSize.y - sOffset.y) + viewPos, GUI::Theme::Selection);
				for (f32 y = fmodf(offset.y, scale); y < viewSize.y; y += scale)
					drawList->AddLine(ImVec2(0.0f - sOffset.x, y - sOffset.y) + viewPos, ImVec2(viewSize.x - sOffset.x, y - sOffset.y) + viewPos, GUI::Theme::Selection);

				if (viewRect.Max.x > 0.0f)
					GUI::DrawScale(ImVec2(0.0f, 0.0f), ImVec2(viewRect.Max.x, 0.0f), scale, 16.0f, 0.6f);
				if (viewRect.Min.x < 0.0f)
					GUI::DrawScale(ImVec2(0.0f, 0.0f), ImVec2(viewRect.Min.x, 0.0f), scale, 16.0f, 0.6f, -1.0f);
				if (viewRect.Max.y > 0.0f)
					GUI::DrawScale(ImVec2(0.0f, 0.0f), ImVec2(0.0f, viewRect.Max.y), scale, 16.0f, 0.6f);
				if (viewRect.Min.y < 0.0f)
					GUI::DrawScale(ImVec2(0.0f, 0.0f), ImVec2(0.0f, viewRect.Min.y), scale, 16.0f, 0.6f, -1.0f);

				Texture2D* spritesheetTexture = m_Spritesheet->GetSpritesheetTexture();

				ImGui::Image(
					GUI::GetTextureID(spritesheetTexture->GetHandle()), { (f32)spritesheetTexture->GetWidth(), (f32)spritesheetTexture->GetHeight() }, { 0, 1 }, { 1, 0 }
				);

				for (SpriteData& spriteData : m_Sprites) {
					DrawRectOnCanvas(drawList, spriteData.Position, spriteData.Scale * scale);
				}

				canvas.End();
			}

			ImGui::EndTable();
		}
	}

	void SpritesheetEditor::SetAsset(Asset* asset)
	{
		m_Spritesheet = (Spritesheet*)asset;
	}

	// offset from left right corner
	static void DrawImagePartProperty(
		Texture2D* wholeImage, const char* label, const char* tooltip = nullptr,
		glm::vec2 offset = glm::vec2(0.0f), glm::vec2 size = glm::vec2(0.0f), glm::vec4 tint = glm::vec4(1.0f),
		f32 additionalScale = 1.f
	) {
		f32 width = (f32)wholeImage->GetWidth();
		f32 height = (f32)wholeImage->GetHeight();

		GUI::BeginPropertyGrid(label, tooltip, false);

		ImVec2 uv0 = ImVec2(offset.x / width, (height - offset.y) / height);
		ImVec2 uv1 = ImVec2((offset.x + size.x * additionalScale) / width, (height - offset.y - (size.y * additionalScale)) / height);

		ImVec2 imageSize = ImGui::GetContentRegionAvail();
		
		f32 originalImageWidth = size.x * additionalScale;
		f32 originalImageHeight = size.y * additionalScale;

		f32 imageAspectRatio = originalImageWidth / originalImageHeight;
		f32 availableAspectRatio = imageSize.x / imageSize.y;

		if (imageAspectRatio > availableAspectRatio)
			imageSize.y = imageSize.x / imageAspectRatio;
		else
			imageSize.x = imageSize.y * imageAspectRatio;
		

		ImGui::Image(
			GUI::GetTextureID(wholeImage->GetHandle()), imageSize, uv0, uv1, { tint.r, tint.g, tint.b, tint.a }
		);

		GUI::EndPropertyGrid();
	}

	static void DrawLateSubmitSingleLineTextInputProperty(

	) {
		// a button edit / submit, submit uploads the changed value
	}

	void SpritesheetEditor::RenderSpriteCards(f32 scale)
	{
		constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_NoPadOuterX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerH
			| ImGuiTableFlags_ContextMenuInBody
			| ImGuiTableFlags_ScrollY;

		constexpr ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_FramePadding;

		ImGui::Button("Add new sprite");

		static GUI::TextFilter spriteFilter;

		f32 leftPadding = 10.0f;
		ImGui::Dummy(ImVec2(leftPadding, 0));
		ImGui::SameLine();
		spriteFilter.OnRender("  " SW_ICON_MAGNIFY "  Search ... ");

		if (ImGui::BeginTable("SideViewTable", 1, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			int indexToRemove = -1;

			for (int i = 0; i < m_Sprites.size(); i++) {
				SpriteData& spriteData = m_Sprites[i];

				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				const char* spriteName = spriteData.Name.c_str();
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
					GUI::DrawSingleLineTextInputProperty(spriteData.Name, "Sprite name");
					GUI::DrawVector2ControlProperty(spriteData.Position, "Position");
					GUI::DrawVector2ControlProperty(spriteData.Scale, "Scale", nullptr, 1.f, 1.f, 20.f);
					GUI::DrawVector4ColorPickerProperty(spriteData.Tint, "Tint");
					DrawImagePartProperty(
						m_Spritesheet->GetSpritesheetTexture(), "Sprite", nullptr, spriteData.Position, spriteData.Scale, spriteData.Tint, scale
					);
					GUI::EndProperties();

					ImGui::TreePop();
				}
			}
			
			if (indexToRemove != -1) {
				m_Sprites.erase(m_Sprites.begin() + indexToRemove);
			}

			ImGui::EndTable();
		}
	}

}
