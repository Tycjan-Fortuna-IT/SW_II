#include "SpritesheetEditor.hpp"

#include <fstream>

#include "GUI/Editor/EditorResources.hpp"
#include "Asset/AssetLoader.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "GUI/GUI.hpp"
#include "Core/Utils/SerializationUtils.hpp"
#include "GUI/Appearance.hpp"

namespace SW {

	constexpr f32 MIN_ZOOM = 0.1f;
	constexpr f32 MAX_ZOOM = 15.0f;
	constexpr f32 MIN_GRID_SIZE = 16.f;
	constexpr f32 MAX_GRID_SIZE = 4000.f;

	SpritesheetEditor::SpritesheetEditor(const char* name, const char* icon)
		: AssetEditorPanel(name, icon)
	{

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

	}

	void SpritesheetEditor::Render()
	{
		const glm::vec2 vo = (*m_Spritesheet)->ViewPos;

		ImVec2 viewOrigin = { vo.x, vo.y };
		m_Canvas.SetView(viewOrigin, (*m_Spritesheet)->ViewZoom);
		(*m_Spritesheet)->ViewPos = glm::vec2{ viewOrigin.x, viewOrigin.y };

		ImGuiIO& io = ImGui::GetIO();

		ImVec2 availableRegion = ImGui::GetContentRegionAvail();

		static ImVec2 drawStartPoint;
		static bool isDragging = false;

        constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit
            | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

		ImGui::PushFont(GUI::Appearance::GetFonts().BigBoldFont);
		ImGui::Text(
			"Currently editing: %s [%i x %i]",
			m_Name.c_str(), 
			(*m_Spritesheet)->GetSpritesheetTexture()->GetWidth(), 
			(*m_Spritesheet)->GetSpritesheetTexture()->GetHeight()
		);
		ImGui::PopFont();

		GenerateFromGridPopup(&m_OpenGenerateFromGridModal);

		if (ImGui::BeginTable("MainViewTable", 2, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.f);
			if (GUI::Layout::BeginHeaderCollapse("General Options", ImGuiTreeNodeFlags_DefaultOpen)) {
				GUI::Properties::BeginProperties("##spritesheet_editor_general_options");

				GUI::Properties::ScalarSliderProperty(&(*m_Spritesheet)->GridSize, "Grid Size", nullptr, MIN_GRID_SIZE, MAX_GRID_SIZE);
				GUI::Properties::Vector2InputProperty(&(*m_Spritesheet)->CenterOffset, "Center Offset");
				GUI::Properties::DrawFolderPickerProperty(&(*m_Spritesheet)->ExportPath, "Export Path");

				AssetHandle handle = (*m_Spritesheet)->GetSpritesheetTexture() ? (*m_Spritesheet)->GetSpritesheetTexture()->GetHandle() : 0u;
				if (GUI::Properties::AssetSearchProperty<Texture2D>(&handle, "Spritesheet")) {
					Texture2D** texture = handle ? AssetManager::GetAssetRaw<Texture2D>(handle) : nullptr;

					(*m_Spritesheet)->SetSpritesheetTexture(texture);
				}

				GUI::Properties::EndProperties();

				GUI::Layout::EndHeaderCollapse();
			}

			if (GUI::Layout::BeginHeaderCollapse("Advanced Options")) {
				GUI::Properties::BeginProperties("##spritesheet_editor_advanced_options");

				if (GUI::Properties::ScalarSliderProperty(&(*m_Spritesheet)->ViewZoom, "View Zoom", nullptr, MIN_ZOOM, MAX_ZOOM)) {
					m_Canvas.SetView(viewOrigin, (*m_Spritesheet)->ViewZoom);
				}
				GUI::Properties::CheckboxProperty(&(*m_Spritesheet)->ShowImageBorders, "Show Image Borders");
				GUI::Properties::CheckboxProperty(&(*m_Spritesheet)->ShowGridCoords, "Show Grid Coordinates");

				GUI::Properties::EndProperties();

				GUI::Layout::EndHeaderCollapse();
			}
			ImGui::PopStyleVar();

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
				} else if (!isDragging && ImGui::IsItemHovered() && io.MouseWheel) {
					// proper zooming finally
					const ImVec2 mousePos = io.MousePos;

					const ImGuiEx::CanvasView oldView = m_Canvas.View();
					const f32 newViewScale = glm::clamp(m_Canvas.ViewScale() * powf(1.1f, io.MouseWheel), MIN_ZOOM, MAX_ZOOM);

					m_Canvas.SetView(viewOrigin, newViewScale);
					const ImGuiEx::CanvasView newView = m_Canvas.View();

					const ImVec2 screenPosition = m_Canvas.FromLocal(mousePos, oldView);
					const ImVec2 canvasPosition = m_Canvas.ToLocal(screenPosition, newView);
					const ImVec2 originOffset = (canvasPosition - mousePos) * newViewScale;

					m_Canvas.SetView(viewOrigin + originOffset, newViewScale);
					(*m_Spritesheet)->ViewZoom = newViewScale;
					(*m_Spritesheet)->ViewPos = glm::vec2{ viewOrigin.x + originOffset.x, viewOrigin.y + originOffset.y };
				}

				ImRect viewRect = m_Canvas.ViewRect();

				glm::vec2 offset = (*m_Spritesheet)->CenterOffset;
				const ImVec2 lineOffset = { offset.x, offset.y };

				ImVec2 loffset = m_Canvas.ViewOrigin() * (1.0f / (*m_Spritesheet)->ViewZoom) + lineOffset;
				ImVec2 viewPos = m_Canvas.ViewRect().Min + lineOffset;
				ImVec2 viewSize = m_Canvas.ViewRect().GetSize();
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				f32 gridScale = (*m_Spritesheet)->GridSize;

				{
					ImRect rect = m_Canvas.ViewRect();

					drawList->AddRectFilled(rect.Min, rect.Max, GUI::Theme::BackgroundDark);
				}

				Texture2D* spritesheetTexture = (*m_Spritesheet)->GetSpritesheetTexture();
				if (!spritesheetTexture)
					spritesheetTexture = EditorResources::MissingAssetIcon;

				const ImVec2 texSize = { (f32)spritesheetTexture->GetWidth(), (f32)spritesheetTexture->GetHeight() };

				ImGui::Image(
					GUI::GetTextureID(spritesheetTexture->GetTexHandle()), texSize,
					{ 0, 1 }, { 1, 0 }
				);

				// in the middle, a dot
				drawList->AddCircleFilled(
					ImVec2(0.f, 0.f),
					2.0f, GUI::Theme::MissingMesh
				);

				for (f32 x = fmodf(loffset.x, gridScale); x < viewSize.x; x += gridScale) {
					if (x < 0) continue;

					drawList->AddLine(
						ImVec2(x - offset.x - 0.5f, 0.0f - offset.y) + viewPos,
						ImVec2(x - offset.x - 0.5f, viewSize.y - offset.y) + viewPos, GUI::Theme::Selection,
						2.f
					);

					if ((*m_Spritesheet)->ShowGridCoords) {
						for (f32 y = fmodf(loffset.y, gridScale); y < viewSize.y; y += gridScale) {
							if (y < 0) continue;

							const int gridX = (int)std::round(x - loffset.x);
							const int gridY = (int)std::round(y - loffset.y);

							drawList->AddText(
								ImVec2(x - offset.x - 0.5f, y - offset.y - 0.5f) + viewPos,
								GUI::Theme::TextBrighter,
								(std::to_string(gridX) + ", " + std::to_string(gridY)).c_str()
							);
						}
					}
				}

				for (f32 y = fmodf(loffset.y, gridScale); y < viewSize.y; y += gridScale) {
					if (y < 0) continue;

					drawList->AddLine(
						ImVec2(0.0f - offset.x, y - offset.y - 0.5f) + viewPos,
						ImVec2(viewSize.x - offset.x, y - offset.y - 0.5f) + viewPos, GUI::Theme::Selection,
						2.f
					);
				}

				if ((*m_Spritesheet)->ShowImageBorders) {
					DrawRectOnCanvas(drawList, glm::vec2{ -1.f, -1.f }, glm::vec2{ texSize.x + 1.f, texSize.y + 1.f }, GUI::Theme::NiceBlue);

					for (const SpriteData& sprite : (*m_Spritesheet)->Sprites) {
						DrawRectOnCanvas(drawList, sprite.Position, sprite.Size - 1.f, GUI::Theme::InvalidPrefab);
					}
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

		AssetMetaData metadata = AssetManager::GetAssetMetaData(handle);
		m_Name = metadata.Path.filename().string();
	}

	void SpritesheetEditor::DrawRectOnCanvas(ImDrawList* drawList, const glm::vec2& position, const glm::vec2& size, u32 color) const
	{
		const glm::vec2 offset = (*m_Spritesheet)->CenterOffset;

		const ImVec2 topLeft = { position.x + offset.x, position.y + offset.y };
		const ImVec2 topRight = { position.x + offset.x + size.x, position.y + offset.y };
		const ImVec2 bottomRight = { position.x + offset.x + size.x, position.y + offset.y + size.y };
		const ImVec2 bottomLeft = { position.x + offset.x, position.y + offset.y + size.y };

		drawList->AddLine(topLeft, topRight, color);
		drawList->AddLine(topRight, bottomRight, color);
		drawList->AddLine(bottomRight, bottomLeft, color);
		drawList->AddLine(bottomLeft, topLeft, color);
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

		if (ImGui::Button(SW_ICON_PLUS " Add"))
			AddNewSprite();
		
		ImGui::SameLine();

		if (ImGui::Button(SW_ICON_SAVE " Save")) {
			const AssetMetaData& metadata = AssetManager::GetAssetMetaData((*m_Spritesheet)->GetHandle());

			AssetLoader::Serialize(metadata);
		}

		// generate from grid
		if ((*m_Spritesheet)->GetSpritesheetTexture()) {
			ImGui::SameLine();

			if (ImGui::Button(SW_ICON_GRID " Generate from grid"))
				m_OpenGenerateFromGridModal = true;
		}
		
		if (!(*m_Spritesheet)->ExportPath.empty() && (*m_Spritesheet)->GetSpritesheetTexture()) {
			ImGui::SameLine();
		
			if (ImGui::Button(SW_ICON_EXPORT " Export")) {
				ExportSprites();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button(SW_ICON_DELETE " Clear")) {
			(*m_Spritesheet)->Sprites.clear();
		}

		const f32 availableWidth = ImGui::GetContentRegionAvail().x - 16.f;

		GUI::Widgets::SearchInput(&m_SearchString);

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
				ImGui::PushID(i);

				SpriteData& sprite = sprites[i];

				if (!m_SearchString.empty() && sprite.Name.find(m_SearchString)) {
					ImGui::PopID();
					continue;
				}

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
					GUI::Properties::BeginProperties(uidc);

					GUI::Properties::SingleLineTextInputProperty<128>(&sprite.Name, "Sprite name");
					GUI::Properties::Vector2InputProperty(&sprite.Position, "Position", nullptr, 0.f, 0.f, FLT_MAX, "%1.f");
					GUI::Properties::Vector2InputProperty(&sprite.Size, "Size", nullptr, (*m_Spritesheet)->GridSize, 1.f, 1024.f, "%1.f");
					glm::vec2 position = sprite.Position + (*m_Spritesheet)->CenterOffset;
					GUI::Properties::DrawImagePartProperty(
						(*m_Spritesheet)->GetSpritesheetTexture(), "Sprite", nullptr, position, sprite.Size, glm::vec4(1.0f),
						(*m_Spritesheet)->ShowImageBorders
					);

					GUI::Properties::EndProperties();

					ImGui::TreePop();
				}

				ImGui::PopID();
			}
			
			if (indexToRemove != -1) {
				(*m_Spritesheet)->Sprites.erase((*m_Spritesheet)->Sprites.begin() + indexToRemove);
			}

			ImGui::EndTable();
		}
	}

	void SpritesheetEditor::AddNewSprite()
	{
		SpriteData data(Random::CreateTag());
		data.Size = { (*m_Spritesheet)->GridSize, (*m_Spritesheet)->GridSize };

		(*m_Spritesheet)->Sprites.emplace_back(data);
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

	void SpritesheetEditor::GenerateFromGridPopup(bool* opened)
	{
		if (*opened)
			ImGui::OpenPopup(SW_ICON_GRID " Generate sprites from a grid");

		if (ImGui::BeginPopupModal(SW_ICON_GRID " Generate sprites from a grid", nullptr)) {
			ImGui::Text("Use this tool to generate sprites from a grid.");
			ImGui::Separator();

			const f32 currentGridSize = (*m_Spritesheet)->GridSize;

			GUI::Properties::BeginProperties("##spritesheet_editor_generate_from_grid_options");

			const Texture2D* texture = (*m_Spritesheet)->GetSpritesheetTexture();
			const f32 texWidth = (f32)texture->GetWidth();
			const f32 texHeight = (f32)texture->GetHeight();

			static glm::vec2 gridSize = { (*m_Spritesheet)->GridSize, (*m_Spritesheet)->GridSize };
			GUI::Properties::Vector2InputProperty(&gridSize, "Grid Size", 
				"Grid size used to generate sprites, can be different from the current grid size.", currentGridSize, MIN_GRID_SIZE, MAX_GRID_SIZE, "%1.f");

			static glm::vec2 textureOffset = { 0.f, 0.f };
			GUI::Properties::Vector2InputProperty(&textureOffset, "Texture Offset", 
				"Offset in texture, used to start cutting sprites from a different position.", 0.f, 0.f, FLT_MAX, "%1.f");

			static f32 cutSizeX = (f32)texture->GetWidth();
			GUI::Properties::ScalarInputProperty(&cutSizeX, "Cut Size X", 
				"Cut size in X axis, part or whole spritesheet texture.", 16.f, 32.f, 0.f, (f32)texture->GetWidth(), "%1.f");

			static f32 cutSizeY = (f32)texture->GetHeight();
			GUI::Properties::ScalarInputProperty(&cutSizeY, "Cut Size Y", 
				"Cut size in Y axis, part or whole spritesheet texture.", 16.f, 32.f, 0.f, (f32)texture->GetHeight(), "%1.f");

			GUI::Properties::EndProperties();

			if (ImGui::Button("Generate", ImVec2(120, 0))) {
				*opened = false;

				const glm::vec2 offset = (*m_Spritesheet)->CenterOffset;

				for (f32 x = offset.x + textureOffset.x; x < offset.x + cutSizeX + textureOffset.x; x += gridSize.x) {
					for (f32 y = offset.y + textureOffset.y; y < offset.y + cutSizeY + textureOffset.y; y += gridSize.y) {
						SpriteData data(Random::CreateTag());
						data.Position = { x, y };
						data.Size = gridSize;

						(*m_Spritesheet)->Sprites.emplace_back(data);
					}
				}

				(*m_Spritesheet)->GridSize = gridSize.x;

				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				*opened = false;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

}
