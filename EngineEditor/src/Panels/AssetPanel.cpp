#include "AssetPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "GUI/Icons.hpp"
#include "Core/Debug/LogSystem.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Core/AssetManager.hpp"
#include "GUI/GUI.hpp"
#include "GUI/Colors.hpp"
#include "GUI/Appearance.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Utils/FileSystem.hpp"

namespace SW {

	static std::unordered_map<std::string, FileType> s_FileTypes = {
		{ ".png",	    FileType::Texture	 },
		{ ".jpg",	    FileType::Texture	 },
		{ ".jpeg",	    FileType::Texture	 },
		{ ".bmp",	    FileType::Texture	 },
		{ ".sw",		FileType::Scene		 },
		{ ".ttf",		FileType::Font		 },
		{ ".swprefab",	FileType::Prefab	 },
		{ ".cs",		FileType::Script	 },
	};

	static const std::unordered_map<FileType, ImVec4> s_FileTypeColors = {
		{ FileType::Unknown,   { 0.80f, 0.52f, 0.00f, 1.00f } },
		{ FileType::Texture,   { 0.80f, 0.40f, 0.40f, 1.00f } },
		{ FileType::Directory, { 0.80f, 0.80f, 0.70f, 1.00f } },
		{ FileType::Scene,     { 0.48f, 0.64f, 0.80f, 1.00f } },
		{ FileType::Font,      { 0.53f, 0.53f, 0.53f, 1.00f } },
		{ FileType::Prefab,    { 0.72f, 0.40f, 0.72f, 1.00f } },
		{ FileType::Script,    { 0.45f, 0.74f, 0.45f, 1.00f } },
	};

	static const std::unordered_map<FileType, std::string> s_FileTypeString = {
		{ FileType::Unknown,	"Unknown"       },
		{ FileType::Texture,	"Texture"		},
		{ FileType::Directory,	"Directory"		},
		{ FileType::Scene,		"Scene"			},
		{ FileType::Font,		"Font"			},
		{ FileType::Prefab,		"Prefab"		},
		{ FileType::Script,		"Script"		},
	};

	static const std::unordered_map <FileType, const char*> s_FileTypesToIcon =
	{
		{ FileType::Unknown,	SW_ICON_FILE			},
		{ FileType::Texture,	SW_ICON_FILE_IMAGE		},
		{ FileType::Scene,		SW_ICON_FILE_VIDEO		},
		{ FileType::Font,		SW_ICON_FORMAT_TEXT		},
		{ FileType::Prefab,		SW_ICON_FILE_IMPORT		},
		{ FileType::Script,		SW_ICON_LANGUAGE_CSHARP },
	};

	AssetPanel::AssetPanel(const char* name)
		: Panel(name, SW_ICON_FOLDER_STAR, true)
	{
		EventSystem::Register(EVENT_CODE_PROJECT_LOADED, nullptr, [this](Event event, void* sender, void* listener) -> bool {		
			InvalidateAssetDirectory();

			return false;
		});
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

			OnEnd();
		}
	}

	void AssetPanel::InvalidateAssetDirectory()
	{
        m_AssetsDirectory = ProjectContext::Get()->GetAssetDirectory() / "assets";

#ifdef SW_DEBUG_BUILD
		m_CurrentDirectory = m_AssetsDirectory / "scenes";
#else 
		m_CurrentDirectory = m_AssetsDirectory;
#endif

		LoadDirectoryEntries();
	}

	void AssetPanel::LoadDirectoryEntries()
	{
		m_DirectoryEntries.clear();

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			FileType fileType = FileType::Unknown;

			// todo, introduce separate thumbnail system with lower resolution textures (like 128 x 128)
			Texture2D* thumbnail = AssetManager::GetBlackTexture();

			ImVec4 colorIndicator = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

			std::filesystem::path filepath = std::filesystem::relative(entry.path(), m_AssetsDirectory.parent_path());

			if (!entry.is_directory()) {
				auto it = s_FileTypes.find(filepath.extension().string());

				if (it != s_FileTypes.end())
					fileType = it->second;

				if (fileType == FileType::Texture) {
					thumbnail = AssetManager::GetTexture2D(filepath.string().c_str());
				} else if (fileType == FileType::Scene) {
					thumbnail = AssetManager::GetEditorTexture2D("assets/icons/editor/FileIcons/Scene_FileIcon.png");
				} else if (fileType == FileType::Font) {
					thumbnail = AssetManager::GetEditorTexture2D("assets/icons/editor/FileIcons/Scene_FontIcon.png");
				} else {
					thumbnail = AssetManager::GetEditorTexture2D("assets/icons/editor/TextFile.png");
				}
			} else {
				fileType = FileType::Directory;
				thumbnail = AssetManager::GetEditorTexture2D("assets/icons/editor/DirectoryIcon.png");
			}

			auto colorIt = s_FileTypeColors.find(fileType);

			if (colorIt != s_FileTypeColors.end())
				colorIndicator = colorIt->second;

			std::string typeString = "Unknown";

			auto nameIt = s_FileTypeString.find(fileType);

			if (nameIt != s_FileTypeString.end())
				typeString = nameIt->second;

			File file = { 
				.Name = filepath.filename().string(),
				.FilePath = filepath.string(),
				.Thumbnail = thumbnail, 
				.Type = fileType,
				.TypeString = typeString,
				.ColorIndicator = colorIndicator,
			};

			m_DirectoryEntries.emplace_back(file);
		}
	}

	void AssetPanel::DrawHeader()
	{
		if (ImGui::Button(SW_ICON_REFRESH)) {
			LoadDirectoryEntries();
		}

		ImGui::SameLine();

		bool atAssetsDir = m_CurrentDirectory == m_AssetsDirectory;

		if (atAssetsDir) {
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		ImGui::SameLine();

		if (ImGui::Button(SW_ICON_ARROW_LEFT_BOLD)) {
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
			LoadDirectoryEntries();
		}

		if (atAssetsDir) {
			ImGui::PopStyleVar();
			ImGui::PopItemFlag();
		}

		ImGui::SameLine();

		std::filesystem::path currentDirCopy = m_CurrentDirectory;
		std::vector<std::filesystem::path> paths;

		while (currentDirCopy != m_AssetsDirectory.parent_path()) {
			paths.push_back(currentDirCopy);
			currentDirCopy = currentDirCopy.parent_path();
		}

		{
			for (auto it = paths.rbegin(); it != paths.rend(); ++it) {
				ImGui::SameLine();
				ImGui::TextUnformatted(SW_ICON_FOLDER);
				ImGui::SameLine();

				if (ImGui::Button(it->filename().string().c_str())) {
					m_CurrentDirectory = *it;
					LoadDirectoryEntries();
				}

				if (it + 1 != paths.rend()) {
					ImGui::SameLine();
					ImGui::TextUnformatted("/");
				}
			}
		}
	}

	void AssetPanel::DrawDirectoryTreeViewRecursive(const std::filesystem::path& path)
	{
		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_SpanFullWidth;

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
			ImGuiTreeNodeFlags nodeFlags = treeNodeFlags;

			const std::filesystem::path& entryPath = entry.path();

			const bool entryIsFile = !std::filesystem::is_directory(entryPath);

			if (entryIsFile)
				nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			const bool selected = m_CurrentDirectory == entryPath;

			if (selected) {
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
				ImGui::PushStyleColor(ImGuiCol_Header, GUI::Theme::SelectionMuted);
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, GUI::Theme::SelectionMuted);
			} else {
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, GUI::Theme::Background);
			}

			bool opened = ImGui::TreeNodeEx(entryPath.string().c_str(), nodeFlags, "");

			ImGui::PopStyleColor(selected ? 2 : 1);

			if (!entryIsFile && ImGui::IsItemClicked()) {
				m_CurrentDirectory = entryPath;
				LoadDirectoryEntries();
			}

			std::string filename = entryPath.filename().string();

			const char* folderIcon = SW_ICON_FILE;

			if (entryIsFile) {
				FileType fileType = FileType::Unknown;

				std::string entryPathStr = entryPath.extension().string();

				const auto& fileTypeIt = s_FileTypes.find(entryPathStr.c_str());

				if (fileTypeIt != s_FileTypes.end())
					fileType = fileTypeIt->second;

				const auto& fileTypeIconIt = s_FileTypesToIcon.find(fileType);

				if (fileTypeIconIt != s_FileTypesToIcon.end())
					folderIcon = fileTypeIconIt->second;
			} else {
				folderIcon = opened ? SW_ICON_FOLDER_OPEN : SW_ICON_FOLDER;
			}

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::TextBrighter);
			ImGui::TextUnformatted(folderIcon);
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::TextUnformatted(filename.c_str());

			if (!entryIsFile) {
				if (opened) {
					DrawDirectoryTreeViewRecursive(entryPath);

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
			| ImGuiTableFlags_ContextMenuInBody
			| ImGuiTableFlags_ScrollY;

		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_SpanFullWidth
			| ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::BeginTable("SideViewTable", 1, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGuiTreeNodeFlags nodeFlags = treeNodeFlags;

			const bool selected = m_CurrentDirectory == m_AssetsDirectory;

			if (selected) {
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
				ImGui::PushStyleColor(ImGuiCol_Header, GUI::Theme::SelectionMuted);
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, GUI::Theme::SelectionMuted);
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, GUI::Theme::Background);
			}

			const bool opened = ImGui::TreeNodeEx("AssetsDir", nodeFlags, "");

			if (ImGui::IsItemClicked()) {
				m_CurrentDirectory = m_AssetsDirectory;
				LoadDirectoryEntries();
			}

			ImGui::PopStyleColor(selected ? 2 : 1);

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::TextBrighter);
			ImGui::TextUnformatted(opened ? SW_ICON_FOLDER_OPEN : SW_ICON_FOLDER);
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::TextUnformatted("Assets");

			if (opened) {
				DrawDirectoryTreeViewRecursive(m_AssetsDirectory);

				ImGui::TreePop();
			}

			ImGui::EndTable();
		}
	}

	void AssetPanel::DrawBody()
	{
		bool refreshDirectory = false;

		constexpr f32 customThumbnailSize = 150.f;
		constexpr f32 padding = 6.0f;
		const f32 scaledThumbnailSize = customThumbnailSize * ImGui::GetIO().FontGlobalScale;
		const f32 scaledThumbnailSizeX = scaledThumbnailSize * 0.55f;
		const f32 cellSize = scaledThumbnailSizeX + 2 * padding - 5.f;

		constexpr f32 overlayPaddingY = 6.0f * padding;
		constexpr f32 thumbnailPadding = overlayPaddingY * 0.5f;
		const f32 thumbnailSize = scaledThumbnailSizeX - thumbnailPadding;

		const ImVec2 backgroundThumbnailSize = { scaledThumbnailSizeX + padding * 2, scaledThumbnailSize + padding * 2 };

		const f32 panelWidth = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize;
		int columnCount = static_cast<int>(panelWidth / cellSize);

		if (!columnCount) // Edge case
			return;

		ImGuiTableFlags flags = ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_PadOuterX | ImGuiTableFlags_SizingFixedFit;

		ImTextureID whiteTexId = GUI::GetTextureID(AssetManager::GetWhiteTexture()->GetHandle());

		bool isAnyItemHovered = false;

		if (ImGui::BeginTable("BodyTable", columnCount, flags)) {
			int i = 0;

			for (const File& entry : m_DirectoryEntries) {
				ImGui::PushID(i);

				bool isDirectory = entry.Type == FileType::Directory;

				std::string filename = "Error";

				if (entry.Name.size() > 20) {
					filename = entry.Name.substr(0, std::min<u64>(20, entry.Name.size())) + "...";
				} else {
					filename = entry.Name;
				}

				const char* filenameEnd = filename.data() + filename.size();

				const ImTextureID textureId = GUI::GetTextureID(entry.Thumbnail);

				ImGui::TableNextColumn();

				const ImVec2 cursorPos = ImGui::GetCursorPos();

				// Foreground Image
				ImGui::SetCursorPos({ cursorPos.x + padding, cursorPos.y + padding });
				ImGui::SetItemAllowOverlap();

				const ImVec2 imageSize = { backgroundThumbnailSize.x - padding * 2.0f, backgroundThumbnailSize.y - padding * 2.0f };
				const ImVec2 imagePos = ImGui::GetCursorScreenPos();

				const ImVec2 min = imagePos;
				const ImVec2 max = { imagePos.x + imageSize.x, imagePos.y + imageSize.y };

				const ImVec4 backgroundColor = ImGui::ColorConvertU32ToFloat4(GUI::Theme::SelectionMuted);
				const ImVec4 borderColor = ImGui::IsMouseHoveringRect(min, max) ?
					ImGui::ColorConvertU32ToFloat4(GUI::Theme::Selection) : ImGui::ColorConvertU32ToFloat4(GUI::Theme::PropertyField);

				ImGui::Image(whiteTexId, imageSize, { 0, 0 }, { 1, 1 }, backgroundColor, borderColor);

				DrawDirectoryEntryPopup(entry.FilePath);

				if (ImGui::IsItemHovered())
					isAnyItemHovered = true;

				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();

					if (entry.Type == FileType::Texture) {
						ImGui::Text("Name: %s \nThumbnail mem: %s", entry.Name.c_str(), String::BytesToString(entry.Thumbnail->GetEstimatedSize()).c_str());
					} else {
						ImGui::Text("Name: %s", entry.Name.c_str());
					}

					ImGui::EndTooltip();

					if (ImGui::IsMouseDoubleClicked(0) && isDirectory) {
						m_CurrentDirectory = m_CurrentDirectory / entry.Name;
						refreshDirectory = true;
					}
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					const char* type = "CONTENT_BROWSER_ITEM";

					if (entry.Type == FileType::Font)
						type = "Font";
					else if (entry.Type == FileType::Scene)
						type = "Scene";
					else if (entry.Type == FileType::Texture)
						type = "Texture";
					else if (entry.Type == FileType::Prefab)
						type = "Prefab";

					ImGui::SetDragDropPayload(type, entry.FilePath.data(), entry.FilePath.size() + 1);
					ImGui::TextUnformatted(entry.FilePath.c_str());
					ImGui::Spacing();

					const f32 tooltipSize = ImGui::GetFrameHeight() * 11.0f;
					ImGui::Image(textureId, { tooltipSize, tooltipSize }, { 0, 1 }, { 1, 0 });
					ImGui::EndDragDropSource();
				}

				// Thumbnail Image
				ImGui::SetCursorPos({ cursorPos.x + thumbnailPadding * 0.75f, cursorPos.y + thumbnailPadding });
				ImGui::SetItemAllowOverlap();
				ImGui::Image(textureId, { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

				// Type Color frame
				const ImVec2 typeColorFrameSize = { scaledThumbnailSizeX, scaledThumbnailSizeX * 0.03f };
				ImGui::SetCursorPosX(cursorPos.x + padding);
				ImGui::Image(whiteTexId, typeColorFrameSize, { 0, 0 }, { 1, 1 }, entry.ColorIndicator);

				const ImVec2 rectMin = ImGui::GetItemRectMin();
				const ImVec2 rectSize = ImGui::GetItemRectSize();
				const ImRect clipRect = ImRect({ rectMin.x + padding * 1.0f, rectMin.y + padding * 2.0f },
					{ rectMin.x + rectSize.x, rectMin.y + scaledThumbnailSizeX - GUI::Appearance::GetFonts().SmallFont->FontSize - padding * 4.0f });
				GUI::ClippedText(clipRect.Min, clipRect.Max, filename.c_str(), filenameEnd, nullptr, { 0, 0 }, nullptr, clipRect.GetSize().x);

				ImGui::SetCursorPos({ cursorPos.x + padding * 2.0f, cursorPos.y + backgroundThumbnailSize.y - GUI::Appearance::GetFonts().DefaultBoldFont->FontSize - padding * 2.0f });
				ImGui::BeginDisabled();
				ImGui::PushFont(GUI::Appearance::GetFonts().DefaultBoldFont);
				ImGui::TextUnformatted(entry.TypeString.c_str());
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
		}
	}

	void AssetPanel::DrawDirectoryEntryPopup(const std::filesystem::path& entry)
	{
		static bool isPopupDisplayed = false;

		if (!isPopupDisplayed && ImGui::BeginPopupContextItem("DirectoryEntryPopupMenu")) {
			isPopupDisplayed = true;

			if (ImGui::MenuItemEx("Rename", SW_ICON_RENAME_BOX)) {

			}

			if (ImGui::MenuItemEx("Show in explorer", SW_ICON_MAGNIFY)) {
				std::filesystem::path toOpen = ProjectContext::Get()->GetAssetDirectory() / entry;

				if (std::filesystem::is_directory(toOpen))
					FileSystem::RevealFolderInFileExplorer(toOpen);
				else
					FileSystem::OpenFolderAndSelectItem(toOpen);
			}

			if (ImGui::MenuItemEx("Delete", SW_ICON_DELETE)) {
				std::filesystem::remove(ProjectContext::Get()->GetAssetDirectory() / entry);
			}

			ImGui::EndPopup();
		}

		if (!ImGui::IsPopupOpen("DirectoryEntryPopupMenu")) {
			isPopupDisplayed = false;
		}
	}

	void AssetPanel::DrawAssetPanelPopup()
	{
		if (ImGui::BeginPopupContextItem("AssetPanelPopupMenu")) {
			if (ImGui::BeginMenuEx("New", SW_ICON_NEW_BOX)) {
				if (ImGui::MenuItemEx("Scene", SW_ICON_IMAGE)) {

				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}
	}

}
