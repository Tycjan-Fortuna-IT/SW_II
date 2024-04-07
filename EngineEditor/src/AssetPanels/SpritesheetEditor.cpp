#include "SpritesheetEditor.hpp"

#include <fstream>

#include "GUI/Editor/EditorResources.hpp"
#include "Asset/AssetLoader.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Utils/SerializationUtils.hpp"

namespace SW {

	SpritesheetEditor::SpritesheetEditor(const char* name, const char* icon)
		: AssetEditorPanel(name, icon)
	{
		EventSystem::Register(EVENT_CODE_MOUSE_WHEEL, nullptr, [this](Event event, void* sender, void* listener) -> bool {
			const f32 yOffset = event.Payload.f32[1];

			if (!m_IsCanvasHovered)
				return false;

			if (m_Spritesheet) {
				const f32 offset = yOffset / 10.f;
				if ((*m_Spritesheet)->ViewZoom + offset > 0.5f && (*m_Spritesheet)->ViewZoom + offset < 15.f)
					(*m_Spritesheet)->ViewZoom += offset;
			}
				

			return false;
		});
	}

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
		//const AssetMetaData& metadata = AssetManager::GetAssetMetaData((*m_Spritesheet)->GetHandle());
		
		//AssetLoader::Serialize(metadata);
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
			GUI::DrawFloatingPointProperty((*m_Spritesheet)->GridSize, "Grid Size", nullptr, 16.f, 1000.f);
			GUI::DrawVector2ControlProperty((*m_Spritesheet)->CenterOffset, "Center Offset");
			GUI::DrawBooleanProperty((*m_Spritesheet)->ShowImageBorders, "Show Image Borders");
			GUI::DrawFolderPickerProperty((*m_Spritesheet)->ExportPath, "Export Path");

			AssetHandle handle = (*m_Spritesheet)->GetSpritesheetTexture() ? (*m_Spritesheet)->GetSpritesheetTexture()->GetHandle() : 0u;
			if (GUI::DrawAssetDropdownProperty<Texture2D>(handle, "Spritesheet")) {
				Texture2D** texture = handle ? AssetManager::GetAssetRaw<Texture2D>(handle) : nullptr;

				(*m_Spritesheet)->SetSpritesheetTexture(texture);
			}

			(*m_Spritesheet)->ViewPos = glm::vec2{ viewOrigin.x, viewOrigin.y };

			GUI::EndProperties();

			RenderSpriteCards((*m_Spritesheet)->GridSize);

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
				f32 gridScale = (*m_Spritesheet)->GridSize;

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

				const ImVec2 texSize = { (f32)spritesheetTexture->GetWidth(), (f32)spritesheetTexture->GetHeight() };

				ImGui::Image(
					GUI::GetTextureID(spritesheetTexture->GetTexHandle()), texSize,
					{ 0, 1 }, { 1, 0 }
				);


				if ((*m_Spritesheet)->ShowImageBorders) {
					for (const SpriteData& sprite : (*m_Spritesheet)->Sprites) {
						DrawRectOnCanvas(drawList, sprite.Position - 1.f, sprite.Size + 1.f);
					}

					DrawRectOnCanvas(drawList, glm::vec2{ -1.f, -1.f }, glm::vec2{ texSize.x + 1.f, texSize.y + 1.f });
				}

				m_Canvas.End();
			}

			m_IsCanvasHovered = ImGui::IsItemHovered();

			ImGui::EndTable();
		}
	}

	void SpritesheetEditor::SetAssetHandle(AssetHandle handle)
	{
		m_Spritesheet = AssetManager::GetAssetRaw<Spritesheet>(handle);
	}

	void SpritesheetEditor::DrawRectOnCanvas(ImDrawList* drawList, const glm::vec2& position, const glm::vec2& size) const
	{
		const glm::vec2 offset = (*m_Spritesheet)->CenterOffset;

		const ImVec2 topLeft = { position.x + offset.x, position.y + offset.y };
		const ImVec2 topRight = { position.x + offset.x + size.x, position.y + offset.y };
		const ImVec2 bottomRight = { position.x + offset.x + size.x, position.y + offset.y + size.y };
		const ImVec2 bottomLeft = { position.x + offset.x, position.y + offset.y + size.y };

		drawList->AddLine(topLeft, topRight, GUI::Theme::NiceBlue);
		drawList->AddLine(topRight, bottomRight, GUI::Theme::NiceBlue);
		drawList->AddLine(bottomRight, bottomLeft, GUI::Theme::NiceBlue);
		drawList->AddLine(bottomLeft, topLeft, GUI::Theme::NiceBlue);
	}

	void SpritesheetEditor::RenderSpriteCards(f32 vscale)
	{
		constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_NoPadOuterX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerH
			| ImGuiTableFlags_ScrollY;

		constexpr ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_FramePadding;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 16.f);

		if (ImGui::Button(SW_ICON_PLUS "Add"))
			AddNewSprite();
		
		ImGui::SameLine();

		if (ImGui::Button(SW_ICON_SAVE "Save"))
			OnClose(); // serialize
		
		if (!(*m_Spritesheet)->ExportPath.empty() && (*m_Spritesheet)->GetSpritesheetTexture()) {
			ImGui::SameLine();
		
			if (ImGui::Button(SW_ICON_EXPORT "Export")) {
				ExportSprites();
			}
		}

		ImGui::SameLine();

		static GUI::TextFilter spriteFilter;

		const f32 availableWidth = ImGui::GetContentRegionAvail().x - 16.f;

		ImGui::SetNextItemWidth(availableWidth);
		spriteFilter.OnRender("  " SW_ICON_MAGNIFY "  Search ... ");

		if (ImGui::BeginTable("SideViewTable", 1, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			int indexToRemove = -1;

			if (!(*m_Spritesheet)->GetSpritesheetTexture()) {
				GUI::MoveMousePosX(16.f);
				ImGui::TextUnformatted("Choose texture to proceed!");
				ImGui::EndTable();
				return;
			}

			std::vector<SpriteData>& sprites = (*m_Spritesheet)->Sprites;
			for (int i = 0; i < sprites.size(); i++) {
				SpriteData& sprite = sprites[i];

				if (!spriteFilter.FilterPass(sprite.Name))
					continue;

				ImGui::TableNextRow();
				ImGui::TableNextColumn();

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
					GUI::DrawVector2ControlProperty(sprite.Position, "Position", nullptr, 0.f, 0.f, FLT_MAX, "%1.f");
					GUI::DrawVector2ControlProperty(sprite.Size, "Size", nullptr, (*m_Spritesheet)->GridSize, 1.f, 1024.f, "%1.f");
					glm::vec2 position = sprite.Position + (*m_Spritesheet)->CenterOffset;
					GUI::DrawImagePartProperty(
						(*m_Spritesheet)->GetSpritesheetTexture(), "Sprite", nullptr, position, sprite.Size, glm::vec4(1.0f),
						(*m_Spritesheet)->ShowImageBorders
					);
					
					GUI::EndProperties();

					ImGui::TreePop();
				}
			}
			
			if (indexToRemove != -1) {
				(*m_Spritesheet)->Sprites.erase((*m_Spritesheet)->Sprites.begin() + indexToRemove);
			}

			ImGui::EndTable();
		}
	}

	void SpritesheetEditor::AddNewSprite()
	{
		(*m_Spritesheet)->Sprites.emplace_back(SpriteData{ Random::CreateTag() });
	}

	void SpritesheetEditor::ExportSprites() const
	{
		const Texture2D* texture = (*m_Spritesheet)->GetSpritesheetTexture();
		const f32 texWidth = (f32)texture->GetWidth();
		const f32 texHeight = (f32)texture->GetHeight();
		const glm::vec2 offset = (*m_Spritesheet)->CenterOffset;

		for (const SpriteData& sprite : (*m_Spritesheet)->Sprites) {
			const f32 x = sprite.Position.x + offset.x; // 0 -> texWidth
			const f32 y = sprite.Position.y + offset.y; // 0 -> texHeight
			const f32 width = sprite.Size.x;
			const f32 height = sprite.Size.y;

			const glm::vec2 leftDown = { x / texWidth, 1.0f - y / texHeight };
			const glm::vec2 rightDown = { (x + width) / texWidth, 1.0f - y / texHeight };
			const glm::vec2 upRight = { (x + width) / texWidth, 1.0f - (y + height) / texHeight };
			const glm::vec2 upLeft = { x / texWidth, 1.0f - (y + height) / texHeight };

			SW_TRACE("Exporting {} sprite", sprite.Name);

			YAML::Emitter output;

			output << YAML::BeginMap;

			output << YAML::Key << "Sprite" << YAML::Value;
			output << YAML::BeginMap;
			output << YAML::Key << "SpritesheetTextureHandle" << YAML::Value << (*m_Spritesheet)->GetSpritesheetTexture()->GetHandle();
			output << YAML::Key << "TexCordLeftDown" << YAML::Value << leftDown;
			output << YAML::Key << "TexCordRightDown" << YAML::Value << rightDown;
			output << YAML::Key << "TexCordUpRight" << YAML::Value << upRight;
			output << YAML::Key << "TexCordUpLeft" << YAML::Value << upLeft;
			output << YAML::EndMap;

			output << YAML::EndMap;

			std::filesystem::path path = ProjectContext::Get()->GetAssetDirectory() / (*m_Spritesheet)->ExportPath / (sprite.Name + ".sw_sprite");

			std::ofstream fout(path);
			fout << output.c_str();
		}
	}

}
