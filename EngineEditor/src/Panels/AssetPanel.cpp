#include "AssetPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "GUI/Icons.hpp"
#include "Core/Debug/LogSystem.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "GUI/GUI.hpp"
#include "GUI/Colors.hpp"
#include "GUI/Appearance.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "GUI/Popups.hpp"
#include "Core/Asset/AssetManager.hpp"
#include "Core/Asset/AssetDirectoryTree.hpp"
#include "Core/Renderer/Renderer2D.hpp"
#include "../../EngineEditor/src/AssetPanels/AssetEditorPanelManager.hpp" // FIXME

namespace SW {

	AssetPanel::AssetPanel(const char* name)
		: Panel(name, SW_ICON_FOLDER_STAR, true)
	{
		m_AssetTree = new AssetDirectoryTree();

		EventSystem::Register(EVENT_CODE_PROJECT_LOADED, nullptr, [this](Event event, void* sender, void* listener) -> bool {		
			m_AssetsDirectory = ProjectContext::Get()->GetAssetDirectory();

			m_AssetTree->TraverseDirectoryAndMapAssets(m_AssetsDirectory);
			m_SelectedItem = m_AssetTree->GetRootItem();

			return false;
		});
	}

	AssetPanel::~AssetPanel()
	{
		delete m_AssetTree;
	}

	void AssetPanel::OnUpdate(Timestep dt)
	{

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
			} else {
				ImGui::Text("No project selected...");
			}

			if (GUI::Popups::DrawDeleteFilePopup(m_FilesystemEntryToDelete, &m_OpenDeleteWarningModal))
				LoadDirectoryEntries();

			if (GUI::Popups::DrawAddNewFilePopup(m_AssetsDirectory / m_SelectedItem->Path, &m_OpenNewFileModal))
				LoadDirectoryEntries();

			if (GUI::Popups::DrawDeleteFileToRenamePopup(m_FilesystemEntryToRename, &m_RenameEntryModal))
				LoadDirectoryEntries();

			OnEnd();
		}
	}

	void AssetPanel::LoadDirectoryEntries()
	{
		const std::filesystem::path oldSelectedPath = m_SelectedItem->Path;

		m_AssetTree->RefetchChanges(m_AssetsDirectory);

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
			GUI::BeginProperties("##thumbnail_size");
			GUI::DrawIntegralProperty(m_ThumbnailSize, "Thumbnail Size", nullptr, 150, 400);
			GUI::EndProperties();
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

				const std::string filename = std::format("{}   {}", SW_ICON_FOLDER, it->filename().string());

				if (ImGui::Button(filename.c_str(), { ImGui::CalcTextSize(filename.c_str()).x * 1.5f, 34.f })) {
					const std::filesystem::path path = std::filesystem::relative(*it, ProjectContext::Get()->GetAssetDirectory());

					m_QueuedSelectedItem = m_AssetTree->FindChildItemByPath(m_AssetTree->GetRootItem(), path);
				}

				if (it + 1 != paths.rend()) {
					ImGui::SameLine();
					GUI::MoveMousePosY(8.f);
					ImGui::TextUnformatted(SW_ICON_DOTS_VERTICAL);
					GUI::MoveMousePosY(-8.f);
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

			bool opened = ImGui::TreeNodeEx(entryPath.string().c_str(), nodeFlags, "");

			ImGui::PopStyleColor(selected ? 2 : 1);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (child->IsDirectory())
					FileSystem::RevealFolderInFileExplorer(entryPath);
				else
					FileSystem::OpenFolderAndSelectItem(entryPath);
			}

			if (!entryIsFile && ImGui::IsItemClicked()) {
				m_SelectedItem = child;
			}

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::TextBrighter);
			ImGui::TextUnformatted(child->Icon);
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::TextUnformatted(entryPath.filename().string().c_str());

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
			} else {
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

		constexpr f32 padding = 6.0f;
		const f32 scaledThumbnailSize = m_ThumbnailSize * ImGui::GetIO().FontGlobalScale;
		const f32 scaledThumbnailSizeX = scaledThumbnailSize * 0.55f;
		const f32 cellSize = scaledThumbnailSizeX + 2 * padding - 5.f;

		constexpr f32 overlayPaddingY = 6.0f * padding;
		constexpr f32 thumbnailPadding = overlayPaddingY * 0.5f;
		const f32 thumbnailSize = scaledThumbnailSizeX - thumbnailPadding;

		const ImVec2 backgroundThumbnailSize = { scaledThumbnailSizeX + padding * 2, scaledThumbnailSize - 20.f };

		const f32 panelWidth = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize;
		int columnCount = static_cast<int>(panelWidth / cellSize);

		if (!columnCount) // Edge case
			return;

		ImGuiTableFlags flags = ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_PadOuterX | ImGuiTableFlags_SizingFixedFit;

		ImTextureID whiteTexId = GUI::GetTextureID(Renderer2D::WhiteTexture->GetTexHandle());

		bool isAnyItemHovered = false;

		if (ImGui::BeginTable("BodyTable", columnCount, flags)) {
			int i = 0;

			for (AssetSourceItem* child : m_SelectedItem->Children) {
				ImGui::PushID(i);

				std::string filename = child->Path.filename().string();

				if (ImGui::CalcTextSize(filename.c_str()).x > m_ThumbnailSize / 2.f) {
					filename = filename.substr(0, std::min<u64>(15, filename.size())) + "...";
				}

				const char* filenameEnd = filename.data() + filename.size();

				// TODO THUMBNAILS
				const ImTextureID textureId = child->Thumbnail ? GUI::GetTextureID(child->Thumbnail) : 0;

				ImGui::TableNextColumn();

				const ImVec2 cursorPos = ImGui::GetCursorPos();

				// Foreground Image
				ImGui::SetCursorPos({ cursorPos.x + padding, cursorPos.y + padding });
				ImGui::SetItemAllowOverlap();

				const ImVec2 imageSize = { backgroundThumbnailSize.x - padding * 2.0f - 3.f, backgroundThumbnailSize.y - padding * 2.0f };
				const ImVec2 imagePos = ImGui::GetCursorScreenPos();

				const ImVec2 min = imagePos;
				const ImVec2 max = { imagePos.x + imageSize.x, imagePos.y + imageSize.y };

				const ImVec4 backgroundColor = ImGui::ColorConvertU32ToFloat4(GUI::Theme::SelectionMuted);
				const ImVec4 borderColor = ImGui::IsMouseHoveringRect(min, max) ?
					ImGui::ColorConvertU32ToFloat4(GUI::Theme::Selection) : ImGui::ColorConvertU32ToFloat4(GUI::Theme::PropertyField);

				ImGui::Image(whiteTexId, imageSize, { 0, 0 }, { 1, 1 }, backgroundColor, borderColor);

				DrawItemOperationsPopup(child);
				
				if (ImGui::IsItemHovered())
					isAnyItemHovered = true;

				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();

					ImGui::Text("Handle: %llu", child->Handle);
					ImGui::Text("Last Modified: %llu", child->ModificationTime);
					ImGui::Text("Type: %s", Asset::GetStringifiedAssetType(child->Type));
					ImGui::Text("Path: %s", child->Path.string().c_str());

					ImGui::EndTooltip();

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						HandleItemOnDoubleClick(child, &refreshDirectory);
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					const char* type = Asset::GetStringifiedAssetType(child->Type);

					ImGui::SetDragDropPayload(type, &child->Handle, sizeof(child->Handle));
					
					ImGui::Text("Handle: %llu", child->Handle);
					ImGui::Text("Last Modified: %llu", child->ModificationTime);
					ImGui::Text("Type: %s", type);
					ImGui::Text("Path: %s", child->Path.string().c_str());
					ImGui::Spacing();

					const f32 tooltipSize = ImGui::GetFrameHeight() * 11.0f;
					ImGui::Image(textureId, { tooltipSize, tooltipSize }, { 0, 1 }, { 1, 0 });
					ImGui::EndDragDropSource();
				}

				// Thumbnail Image
				ImGui::SetCursorPos({ cursorPos.x + thumbnailPadding * 0.8f, cursorPos.y + thumbnailPadding * 0.8f });
				ImGui::SetItemAllowOverlap();
				ImGui::Image(textureId, { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

				// Type Color frame
				const ImVec2 typeColorFrameSize = { scaledThumbnailSizeX - 7.f, scaledThumbnailSizeX * 0.05f };
				ImGui::SetCursorPosX(cursorPos.x + padding * 1.5f);
				ImGui::Image(whiteTexId, typeColorFrameSize, { 0, 0 }, { 1, 1 }, ImGui::ColorConvertU32ToFloat4(child->Color));

				const ImVec2 rectMin = ImGui::GetItemRectMin();
				const ImVec2 rectSize = ImGui::GetItemRectSize();
				const ImRect clipRect = ImRect({ rectMin.x + padding * 1.0f, rectMin.y + padding * 2.0f },
					{ rectMin.x + rectSize.x, rectMin.y + scaledThumbnailSizeX - GUI::Appearance::GetFonts().SmallFont->FontSize - padding * 4.0f });
				GUI::ClippedText(clipRect.Min, clipRect.Max, filename.c_str(), filenameEnd, nullptr, { 0, 0 }, nullptr, clipRect.GetSize().x);

				ImGui::SetCursorPos({ cursorPos.x + padding * 2.0f, cursorPos.y + backgroundThumbnailSize.y - GUI::Appearance::GetFonts().DefaultBoldFont->FontSize - padding * 2.0f });
				ImGui::BeginDisabled();
				ImGui::PushFont(GUI::Appearance::GetFonts().DefaultBoldFont);
				ImGui::TextUnformatted(Asset::GetStringifiedAssetType(child->Type));
				ImGui::PopFont();
				ImGui::EndDisabled();

				ImGui::PopID(); ++i;
			}

			ImGui::EndTable();

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

		if (item->Type == AssetType::Spritesheet) {
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
