#include "AssetPanel.hpp"

#include "GUI/Icons.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "GUI/Appearance.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "GUI/Popups.hpp"
#include "Asset/AssetManager.hpp"
#include "Asset/AssetDirectoryTree.hpp"
#include "Core/Renderer/Renderer2D.hpp"
#include "Asset/Sprite.hpp"
#include "Asset/Thumbnail.hpp"
#include "Asset/Spritesheet.hpp"
#include "GUI/Editor/EditorResources.hpp"
#ifdef TESTBED
	#include "../../EngineEditor/src/AssetPanels/AssetEditorPanelManager.hpp" // TODO - remove (because of Testbed)
#else
	#include "AssetPanels/AssetEditorPanelManager.hpp"
#endif
#include "Asset/Animation2D.hpp"
#include "Asset/Prefab.hpp"
#include "GUI/GUI.hpp"

namespace SW {

	AssetPanel::AssetPanel()
		: Panel("Assets", SW_ICON_FOLDER_STAR, true), m_AssetTree(new AssetDirectoryTree())
	{
		EventSystem::Register(EVENT_CODE_PROJECT_LOADED, [this](UNUSED Event event) -> bool {
			m_AssetsDirectory = ProjectContext::Get()->GetAssetDirectory();

			m_AssetTree->TraverseDirectoryAndMapAssets();
			m_SelectedItem = m_AssetTree->GetRootItem();

			return false;
		});

		EventSystem::Register(EVENT_CODE_ASSET_DIR_CONTENT_CHANGED, [this](UNUSED Event event) -> bool {
			LoadDirectoryEntries();

			return true;
		});
	}

	AssetPanel::~AssetPanel()
	{
		delete m_AssetTree;
	}

	void AssetPanel::OnUpdate(Timestep dt)
	{
		m_CurrentTime += dt;
	}

	void AssetPanel::OnRender()
	{
		GUI::ScopedColor PanelBackground(ImGuiCol_WindowBg, GUI::Colors::Darken(GUI::Appearance::GetColors().ChildBackground, 0.02f));
		GUI::ScopedStyle CellPadding(ImGuiStyleVar_CellPadding, ImVec2{ 0.0f, 0.0f });

		if (OnBegin()) {
			if (ProjectContext::HasContext()) {
				DrawHeader();

				ImGui::Separator();

				constexpr ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_ContextMenuInBody;

				if (ImGui::BeginTable("MainViewTable", 2, flags, ImGui::GetContentRegionAvail())) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					DrawSideView();

					ImGui::TableNextColumn();

					DrawBody();

					ImGui::EndTable();
				}
			}
			else {
				ImGui::Text("No project selected...");
			}

			if (GUI::Popups::DrawDeleteFilePopup(m_FilesystemEntryToDelete, &m_OpenDeleteWarningModal))
				LoadDirectoryEntries();

			if (m_SelectedItem && GUI::Popups::DrawAddNewFilePopup(m_AssetsDirectory / m_SelectedItem->Path, &m_OpenNewFileModal))
				LoadDirectoryEntries();

			if (GUI::Popups::DrawDeleteFileToRenamePopup(m_FilesystemEntryToRename, &m_RenameEntryModal))
				LoadDirectoryEntries();

			OnEnd();
		}
	}

	void AssetPanel::LoadDirectoryEntries()
	{
		const std::filesystem::path oldSelectedPath = m_SelectedItem->Path;

		m_AssetTree->RefetchChanges();

		AssetSourceItem* newSelectedItem = m_AssetTree->FindChildItemByPath(m_AssetTree->GetRootItem(), oldSelectedPath);

		if (newSelectedItem)
			m_SelectedItem = newSelectedItem;
		else
			m_SelectedItem = m_AssetTree->GetRootItem();
	}

	void AssetPanel::DrawHeader()
	{
		if (ImGui::Button(SW_ICON_COGS, { 34.f, 34.f }))
			ImGui::OpenPopup("SettingsPopup");

		ImGui::SameLine();

		if (ImGui::BeginPopup("SettingsPopup")) {
			GUI::Properties::BeginProperties("##thumbnail_size");
			GUI::Properties::ScalarSliderProperty<int>(&m_ThumbnailSize, "Thumbnail Size", nullptr, 150, 400);
			if (GUI::Properties::ButtonProperty(SW_ICON_DELETE, "Clear thumbnail cache", nullptr, { 34.f, 34.f })) {
				m_Cache.Clear();

				LoadDirectoryEntries();
			}

			GUI::Properties::EndProperties();

			ImGui::EndPopup();
		}

		if (ImGui::Button(SW_ICON_REFRESH, { 34.f, 34.f })) {
			LoadDirectoryEntries();
		}

		ImGui::SameLine();

		bool atAssetsDir = m_SelectedItem == m_AssetTree->GetRootItem();

		if (atAssetsDir) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		ImGui::SameLine();

		if (ImGui::Button(SW_ICON_ARROW_LEFT_BOLD, { 34.f, 34.f })) {
			m_SelectedItem = m_SelectedItem->Parent;
			LoadDirectoryEntries();
		}

		if (atAssetsDir) {
			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
		}

		ImGui::SameLine();

		std::filesystem::path currentDirCopy = ProjectContext::Get()->GetAssetDirectory() / m_SelectedItem->Path;
		std::vector<std::filesystem::path> paths;

		while (currentDirCopy != m_AssetsDirectory.parent_path()) {
			paths.push_back(currentDirCopy);
			currentDirCopy = currentDirCopy.parent_path();
		}

		if (m_SelectedItem == m_AssetTree->GetRootItem())
			paths.erase(paths.begin(), paths.begin() + 1);

		{
			for (auto it = paths.rbegin(); it != paths.rend(); ++it) {
				ImGui::SameLine();

				const std::string filename = std::format("{} {}", SW_ICON_FOLDER, it->filename().string());

				if (ImGui::Button(filename.c_str(), { ImGui::CalcTextSize(filename.c_str()).x * 1.5f, 34.f })) {
					const std::filesystem::path path = std::filesystem::relative(*it, ProjectContext::Get()->GetAssetDirectory());

					m_QueuedSelectedItem = m_AssetTree->FindChildItemByPath(m_AssetTree->GetRootItem(), path);
				}
			}
		}
	}

	void AssetPanel::DrawDirectoryTreeViewRecursive(AssetSourceItem* item)
	{
		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_SpanFullWidth;

		for (AssetSourceItem* child : item->Children) {
			ImGuiTreeNodeFlags nodeFlags = treeNodeFlags;

			const std::filesystem::path& entryPath = m_AssetsDirectory / child->Path;

			const bool entryIsFile = child->IsFile();

			if (entryIsFile)
				nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			const bool selected = m_SelectedItem == child;

			if (selected) {
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
				ImGui::PushStyleColor(ImGuiCol_Header, GUI::Theme::SelectionHalfMuted);
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, GUI::Theme::SelectionHalfMuted);
			} else {
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, GUI::Theme::Background);
			}

			if (child->IsParentOfDescendant(m_SelectedItem)) // recursive open
				ImGui::SetNextItemOpen(true);

			bool opened = ImGui::TreeNodeEx(entryPath.string().c_str(), nodeFlags, "");

			ImGui::PopStyleColor(selected ? 2 : 1);

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay)) {
				ImGui::BeginTooltip();

				ImGui::Text("Handle: %llu", child->Handle);
				ImGui::Text("Last Modified: %llu", child->ModificationTime);
				ImGui::Text("Type: %s", Asset::GetStringifiedAssetType(child->Type));
				ImGui::Text("Path: %s", child->Path.string().c_str());

				ImGui::EndTooltip();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (child->IsDirectory())
					FileSystem::RevealFolderInFileExplorer(entryPath);
				else
					FileSystem::OpenFolderAndSelectItem(entryPath);
			}

			if (!entryIsFile && ImGui::IsItemClicked()) {
				m_SelectedItem = child;
			}

			std::string filename = entryPath.filename().string();

			const f32 charWidth = ImGui::CalcTextSize("A").x;
			const f32 colWidth = ImGui::GetColumnWidth();
			const f32 maxChars = colWidth / charWidth - 4;
			if (ImGui::CalcTextSize(filename.c_str()).x > maxChars * charWidth) {
				filename = filename.substr(0, (size_t)maxChars) + "...";
			}

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::TextBrighter);
			ImGui::TextUnformatted(child->Icon);
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::TextUnformatted(filename.c_str());

			if (!entryIsFile) {
				if (opened) {
					DrawDirectoryTreeViewRecursive(child);

					ImGui::TreePop();
				}
			}
		}
	}

	void AssetPanel::DrawSideView()
	{
		constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_NoPadOuterX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerH
			| ImGuiTableFlags_ScrollY;

		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_SpanFullWidth
			| ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::BeginTable("SideViewTable", 1, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGuiTreeNodeFlags nodeFlags = treeNodeFlags;

			const bool selected = m_SelectedItem == m_AssetTree->GetRootItem();

			if (selected) {
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
				ImGui::PushStyleColor(ImGuiCol_Header, GUI::Theme::SelectionHalfMuted);
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, GUI::Theme::SelectionHalfMuted);
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, GUI::Theme::Background);
			}

			const bool opened = ImGui::TreeNodeEx("AssetsDir", nodeFlags, "");

			if (ImGui::IsItemClicked()) {
				m_SelectedItem = m_AssetTree->GetRootItem();
			}

			ImGui::PopStyleColor(selected ? 2 : 1);

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::TextBrighter);
			ImGui::TextUnformatted(opened ? SW_ICON_FOLDER_OPEN : SW_ICON_FOLDER);
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::TextUnformatted("assets");

			if (opened) {
				DrawDirectoryTreeViewRecursive(m_AssetTree->GetRootItem());

				ImGui::TreePop();
			}

			ImGui::EndTable();
		}
	}

	void AssetPanel::DrawBody()
	{
		bool refreshDirectory = false;

		constexpr f32 padding = 4.0f;
		const f32 scaledThumbnailSize = (f32)m_ThumbnailSize * ImGui::GetIO().FontGlobalScale;
		const f32 scaledThumbnailSizeX = scaledThumbnailSize * 0.65f;
		const f32 cellSize = scaledThumbnailSizeX + 2 * padding;

		constexpr f32 overlayPaddingY = 6.0f * padding;
		constexpr f32 thumbnailPadding = overlayPaddingY * 0.5f;
		const f32 thumbnailSize = scaledThumbnailSizeX - thumbnailPadding;

		const ImVec2 backgroundThumbnailSize = { scaledThumbnailSizeX + padding * 2, scaledThumbnailSize };

		const f32 panelWidth = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize;
		int columnCount = static_cast<int>(panelWidth / cellSize);

		if (!columnCount) // Edge case
			return;

		constexpr ImGuiTableFlags flags = ImGuiTableFlags_ContextMenuInBody
			| ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_PadOuterX
			| ImGuiTableFlags_BordersOuterV
			| ImGuiTableFlags_SizingFixedFit;

		GUI::ScopedStyle CellPadding(ImGuiStyleVar_CellPadding, ImVec2(4.f, 1.f));

		ImTextureID whiteTexId = GUI::GetTextureID(Renderer2D::WhiteTexture->GetTexHandle());

		bool isAnyItemHovered = false;

		if (ImGui::BeginTable("BodyTable", columnCount, flags)) {
			for (int i = 0; i < (int)m_SelectedItem->Children.size(); i++) {
				AssetSourceItem* item = m_SelectedItem->Children[(size_t)i];

				ImGui::TableNextColumn();

				ImGui::PushID(i);

				std::string filename = item->Path.stem().string();

				f32 charWidth = ImGui::CalcTextSize("A").x;
				int maxCharsPerLine = static_cast<int>(scaledThumbnailSizeX / charWidth);

				if (ImGui::CalcTextSize(filename.c_str()).x > scaledThumbnailSizeX) {
					size_t maxChars = (size_t)maxCharsPerLine * 2;
					if (filename.size() > maxChars) {
						filename = filename.substr(0, maxChars) + "...";
					}
				}

				const char* filenameEnd = filename.data() + filename.size();
				const ImVec2 cursorPos = ImGui::GetCursorPos();

				// Foreground Image
				ImGui::SetCursorPos({ cursorPos.x + padding, cursorPos.y + padding });
				ImGui::SetItemAllowOverlap();

				const ImVec2 imageSize = { backgroundThumbnailSize.x - padding * 2.0f - 3.f, backgroundThumbnailSize.y - padding * 2.0f };
				const ImVec2 imagePos = ImGui::GetCursorScreenPos();

				const ImVec2 min = imagePos;
				const ImVec2 max = { imagePos.x + imageSize.x, imagePos.y + imageSize.y };

				const ImVec4 backgroundColor = ImGui::ColorConvertU32ToFloat4(GUI::Theme::SelectionMuted);
				const ImVec4 borderColor = m_IsTableHovered && ImGui::IsMouseHoveringRect(min, max) ?
					ImGui::ColorConvertU32ToFloat4(GUI::Theme::Selection) : ImGui::ColorConvertU32ToFloat4(GUI::Theme::PropertyField);

				ImGui::Image(whiteTexId, imageSize, { 0, 0 }, { 1, 1 }, backgroundColor, borderColor);

				DrawItemOperationsPopup(item);

				if (ImGui::IsItemHovered()) {
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						HandleItemOnDoubleClick(item, &refreshDirectory);

					isAnyItemHovered = true;
				}

				if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay)) {
					ImGui::BeginTooltip();

					ImGui::Text("Handle: %llu", item->Handle);
					ImGui::Text("Last Modified: %llu", item->ModificationTime);
					ImGui::Text("Type: %s", Asset::GetStringifiedAssetType(item->Type));
					ImGui::Text("Path: %s", item->Path.string().c_str());

					ImGui::EndTooltip();
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					const char* type = Asset::GetStringifiedAssetType(item->Type);

					ImGui::SetDragDropPayload(type, &item->Handle, sizeof(item->Handle));

					ImGui::Text("Handle: %llu", item->Handle);
					ImGui::Text("Last Modified: %llu", item->ModificationTime);
					ImGui::Text("Type: %s", type);
					ImGui::Text("Path: %s", item->Path.string().c_str());
					ImGui::Spacing();

					ImGui::EndDragDropSource();
				}

				if (!item->Thumbnail) {

					// ---- !IMPORTANT! ----
					/*
						Actually and edge case, when asset was just created e.g. importing a FontSource and creating with it
						a Font, it does not have the correct modification time, since it happened instantly as for the disk save
						takes some time and updates the modification time in a deffered way.
					*/
					const u64 modificationTime = FileSystem::GetLastWriteTime(ProjectContext::Get()->GetAssetDirectory() / item->Path);

					if (item->Type == AssetType::Sprite) {
						Sprite** spriteAsset = AssetManager::GetAssetRaw<Sprite>(item->Handle);
						Texture2D* texture = (*spriteAsset)->GetTexture();

						Thumbnail thumbnail; // I keep an eye you - std::roundf!
						thumbnail.Width = std::roundf(abs((*spriteAsset)->TexCordUpRight.x - (*spriteAsset)->TexCordLeftDown.x) * (f32)texture->GetWidth());
						thumbnail.Height = std::roundf(abs((*spriteAsset)->TexCordUpRight.y - (*spriteAsset)->TexCordLeftDown.y) * (f32)texture->GetHeight());
						thumbnail.TexCoordMin = { (*spriteAsset)->TexCordLeftDown.x, (*spriteAsset)->TexCordLeftDown.y };
						thumbnail.TexCoordMax = { (*spriteAsset)->TexCordUpRight.x, (*spriteAsset)->TexCordUpRight.y };
						thumbnail.Texture = (*spriteAsset)->GetTextureRaw();

						item->Thumbnail = thumbnail;
					} else if (item->Type == AssetType::Texture2D) {
						Texture2D** texture = m_Cache.GetTextureThumbnail(item->Path, item->Handle, modificationTime);

						Thumbnail thumbnail;
						thumbnail.Width = (f32)(*texture)->GetWidth();
						thumbnail.Height = (f32)(*texture)->GetHeight();
						thumbnail.Texture = texture;

						item->Thumbnail = thumbnail;
					} else if (item->Type == AssetType::Spritesheet) {
						Spritesheet** spritesheetAsset = AssetManager::GetAssetRaw<Spritesheet>(item->Handle);
						Texture2D** texture = (*spritesheetAsset)->GetSpritesheetTextureRaw();
						
						if (!texture) {
							texture = &EditorResources::MissingAssetIcon;

						} else {
							const AssetMetaData& metadata = AssetManager::GetAssetMetaData((*texture)->GetHandle());

							texture = m_Cache.GetTextureThumbnail(metadata.Path, (*texture)->GetHandle(), metadata.ModificationTime);
						}

						Thumbnail thumbnail;
						thumbnail.Width = (f32)(*texture)->GetWidth();
						thumbnail.Height = (f32)(*texture)->GetHeight();
						thumbnail.Texture = texture;

						item->Thumbnail = thumbnail;
					} else if (item->Type == AssetType::Font) {
						Texture2D** texture = m_Cache.GetFontAtlasThumbnail(item->Path, item->Handle, modificationTime);

						Thumbnail thumbnail;
						thumbnail.Width = (f32)(*texture)->GetWidth();
						thumbnail.Height = (f32)(*texture)->GetHeight();
						thumbnail.Texture = texture;

						item->Thumbnail = thumbnail;
					} else if (item->Type == AssetType::Animation2D) {
						Texture2D** texture = &EditorResources::MissingAssetIcon;

						Thumbnail thumbnail;
						thumbnail.Width = (f32)(*texture)->GetWidth();
						thumbnail.Height = (f32)(*texture)->GetHeight();
						thumbnail.Texture = texture;

						item->Thumbnail = thumbnail;
					}

				}
				
				if (item->Type == AssetType::Animation2D) {
					Animation2D** animation = AssetManager::GetAssetRaw<Animation2D>(item->Handle);
					Texture2D** texture = nullptr;

					int framesCount = (int)(*animation)->Sprites.size();

					if (framesCount) {
						item->Thumbnail.CurrentFrame = (int)(m_CurrentTime * (*animation)->Speed) % framesCount;

						if (item->Thumbnail.CurrentFrame >= framesCount) {
							item->Thumbnail.CurrentFrame = 0;
						}

						Sprite** sprite = (*animation)->Sprites[(size_t)item->Thumbnail.CurrentFrame];

						texture = (*sprite)->GetTextureRaw();
						item->Thumbnail.Width = std::roundf(abs((*sprite)->TexCordUpRight.x - (*sprite)->TexCordLeftDown.x) * (f32)(*texture)->GetWidth());
						item->Thumbnail.Height = std::roundf(abs((*sprite)->TexCordUpRight.y - (*sprite)->TexCordLeftDown.y) * (f32)(*texture)->GetHeight());
						
						item->Thumbnail.TexCoordMin = {
							(*animation)->ReverseAlongX ? (*sprite)->TexCordUpRight.x : (*sprite)->TexCordLeftDown.x,
							(*animation)->ReverseAlongY ? (*sprite)->TexCordUpRight.y : (*sprite)->TexCordLeftDown.y
						};

						item->Thumbnail.TexCoordMax = {
							(*animation)->ReverseAlongX ? (*sprite)->TexCordLeftDown.x : (*sprite)->TexCordUpRight.x,
							(*animation)->ReverseAlongY ? (*sprite)->TexCordLeftDown.y : (*sprite)->TexCordUpRight.y
						};
					} else {
						texture = &EditorResources::MissingAssetIcon;
					}
						
					item->Thumbnail.Texture = texture;
				}

				// Thumbnail Image
				const ImVec2 thumbSize = { item->Thumbnail.Width, item->Thumbnail.Height };
				const ImTextureID texId = item->Thumbnail ? GUI::GetTextureID(*item->Thumbnail.Texture) : 0;
				const ImVec2 uv0 = item->Thumbnail.TexCoordMin;
				const ImVec2 uv1 = item->Thumbnail.TexCoordMax;
				//const f32 aspectRatio = thumbSize.x / thumbSize.y;

				ImVec2 displaySize = { thumbnailSize, thumbnailSize };
				//const f32 minSpace = 0.0f;
				//const f32 maxSpace = thumbnailSize * 0.25f;

				//const f32 leftSpaceX = std::clamp(abs((256.f - thumbSize.x) * 0.25f), minSpace, maxSpace);
				//const f32 leftSpaceY = std::clamp(abs((256.f - thumbSize.y) * 0.25f), minSpace, maxSpace);

				ImGui::SetCursorPos({ cursorPos.x + thumbnailPadding * 0.8f, cursorPos.y + thumbnailPadding * 0.8f });
				ImGui::SetItemAllowOverlap();

				//if (thumbSize.x != thumbSize.y) {
				//	if (aspectRatio > 1.0f) { // Image is wider than it is tall
				//		displaySize = { thumbnailSize, thumbnailSize / aspectRatio };
				//		GUI::MoveMousePosY(leftSpaceY);
				//	} else { // Image is taller than it is wide
				//		displaySize = { thumbnailSize * aspectRatio, thumbnailSize };
				//		GUI::MoveMousePosX(leftSpaceX);
				//	}
				//}

				// Display the image
				ImGui::Image(texId, displaySize, uv0, uv1);

				// Type Color frame
				const ImVec2 typeColorFrameSize = { scaledThumbnailSizeX - 7.f, scaledThumbnailSizeX * 0.05f };
				ImGui::SetCursorPos({ cursorPos.x + padding * 1.5f, cursorPos.y + scaledThumbnailSizeX + padding });
				ImGui::Image(whiteTexId, typeColorFrameSize, { 0, 0 }, { 1, 1 }, ImGui::ColorConvertU32ToFloat4(item->Color));

				const ImVec2 rectMin = ImGui::GetItemRectMin();
				const ImVec2 rectSize = ImGui::GetItemRectSize();
				const ImRect clipRect = ImRect({ rectMin.x + padding * 1.0f, rectMin.y + padding * ((f32)m_ThumbnailSize / 100.f)},
					{ rectMin.x + rectSize.x, rectMin.y + scaledThumbnailSizeX - GUI::Appearance::GetFonts().SmallFont->FontSize - padding * 4.0f });
				GUI::Widgets::ClippedText(clipRect.Min, clipRect.Max, filename.c_str(), filenameEnd, nullptr, { 0, 0 }, nullptr, clipRect.GetSize().x);

				ImGui::SetCursorPos({ cursorPos.x + padding * 2.0f, cursorPos.y + backgroundThumbnailSize.y - GUI::Appearance::GetFonts().DefaultBoldFont->FontSize - padding * 2.0f });
				ImGui::BeginDisabled();
				ImGui::PushFont(GUI::Appearance::GetFonts().DefaultBoldFont);
				ImGui::TextUnformatted(Asset::GetStringifiedAssetType(item->Type));
				ImGui::PopFont();
				ImGui::EndDisabled();

				ImGui::PopID();
			}

			ImGui::EndTable();

			m_IsTableHovered = ImGui::IsItemHovered();

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity")) {
					Entity entity = *static_cast<Entity*>(payload->Data);

					// Creating a prefab from the entity.
					std::filesystem::path newFilePath = 
						FileSystem::GetUniqueFilename(ProjectContext::Get()->GetAssetDirectory() / m_SelectedItem->Path / (entity.GetTag() + ".sw_prefab"));

					AssetManager::CreateNew<Prefab>(std::filesystem::relative(newFilePath, ProjectContext::Get()->GetAssetDirectory()), entity);

					LoadDirectoryEntries();
				}
				ImGui::EndDragDropTarget();
			}

			if (!isAnyItemHovered)
				DrawAssetPanelPopup();

			if (refreshDirectory) {
				refreshDirectory = false;
				LoadDirectoryEntries();
			}

			if (m_QueuedSelectedItem) {
				m_SelectedItem = m_QueuedSelectedItem;
				m_QueuedSelectedItem = nullptr;
			}
		}
	}

	void AssetPanel::DrawItemOperationsPopup(const AssetSourceItem* item)
	{
		if (ImGui::BeginPopupContextItem("DirectoryEntryPopupMenu")) {
			if (ImGui::MenuItemEx("Rename", SW_ICON_RENAME_BOX)) {
				m_RenameEntryModal = true;
				m_FilesystemEntryToRename = m_AssetsDirectory / item->Path;
			}

			if (ImGui::MenuItemEx("Show in explorer", SW_ICON_MAGNIFY)) {
				std::filesystem::path toOpen = m_AssetsDirectory / item->Path;

				if (item->IsDirectory())
					FileSystem::RevealFolderInFileExplorer(toOpen);
				else
					FileSystem::OpenFolderAndSelectItem(toOpen);
			}

			if (!item->IsDirectory()) {
				if (ImGui::MenuItemEx("Open externally", SW_ICON_BOOK_OPEN)) {
					FileSystem::OpenExternally(m_AssetsDirectory / item->Path);
				}
			}

			if (ImGui::MenuItemEx("Delete", SW_ICON_DELETE)) {
				m_OpenDeleteWarningModal = true;
				m_FilesystemEntryToDelete = m_AssetsDirectory / item->Path;
			}

			ImGui::EndPopup();
		}
	}

	void AssetPanel::HandleItemOnDoubleClick(AssetSourceItem* item, bool* refreshDirectory)
	{
		if (item->IsDirectory()) {
			m_SelectedItem = item;
			*refreshDirectory = true;

			return;
		}

		if (item->Type == AssetType::Spritesheet || item->Type == AssetType::Animation2D) {
			AssetEditorPanelManager::OpenEditor(item->Handle, item->Type);
		} else {
			FileSystem::OpenExternally(m_AssetsDirectory / item->Path);
		}
	}

	void AssetPanel::DrawAssetPanelPopup()
	{
		if (ImGui::BeginPopupContextItem("AssetPanelPopupMenu")) {
			if (ImGui::MenuItemEx("New file", SW_ICON_NEW_BOX))
				m_OpenNewFileModal = true;

			ImGui::EndPopup();
		}
	}

}
