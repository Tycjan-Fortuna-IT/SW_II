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
		{ ".ttf",		FileType::Font		 }
	};

	static const std::unordered_map<FileType, ImVec4> s_FileTypeColors = {
		{ FileType::Texture,	{ 0.8f, 0.2f, 0.3f, 1.f }	},
		{ FileType::Directory,	{ 1.f, 1.0f, 0.8f, 1.f }	},
        { FileType::Scene,		{ 0.f, 1.0f, 0.f, 1.f }		},
		{ FileType::Font,		{ 0.7f, 1.0f, 0.f, 1.f } }
	};

	static const std::unordered_map<FileType, std::string> s_FileTypeString = {
		{ FileType::Texture,	"Texture"		},
		{ FileType::Directory,	"Directory"		},
        { FileType::Scene,		"Scene"			},
		{ FileType::Font,		"Font"			}
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

		if (OnBegin()) {
			if (ProjectContext::HasContext()) {
				static f32 customThumbnailSize = 200.f;

				bool atAssetsDir = m_CurrentDirectory == m_AssetsDirectory;

				std::filesystem::path currentDirCopy = m_CurrentDirectory;
				std::vector<std::filesystem::path> paths;

				while (currentDirCopy != m_AssetsDirectory.parent_path()) {
					paths.push_back(currentDirCopy);
					currentDirCopy = currentDirCopy.parent_path();
				}

				{
					GUI::ScopedStyle ButtonFramePadding(ImGuiStyleVar_FramePadding, ImVec2{ 10, 6 });

					for (auto it = paths.rbegin(); it != paths.rend(); ++it) {
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

				ImGui::Columns(2, 0, false);

				if (GUI::Button(SW_ICON_REFRESH, { 30.f, 30.f })) {
					LoadDirectoryEntries();
				}

				if (!atAssetsDir) {
					ImGui::SameLine();
					if (GUI::Button(SW_ICON_ARROW_LEFT_BOLD, { 30.f, 30.f })) {
						m_CurrentDirectory = m_CurrentDirectory.parent_path();
						LoadDirectoryEntries();
					}
				}
				ImGui::NextColumn();

				ImGui::DragFloat("Thumbnail size", &customThumbnailSize, 8.f, 64.f, 256.f);

				ImGui::Columns(1);

				bool refreshDirectory = false;

				constexpr f32 padding = 4.0f;
				const f32 scaledThumbnailSize = customThumbnailSize * ImGui::GetIO().FontGlobalScale;
				const f32 scaledThumbnailSizeX = scaledThumbnailSize * 0.55f;
				const f32 cellSize = scaledThumbnailSizeX + 2 * padding + scaledThumbnailSizeX * 0.1f;

				constexpr f32 overlayPaddingY = 6.0f * padding;
				constexpr f32 thumbnailPadding = overlayPaddingY * 0.5f;
				const f32 thumbnailSize = scaledThumbnailSizeX - thumbnailPadding;

				const ImVec2 backgroundThumbnailSize = { scaledThumbnailSizeX + padding * 2, scaledThumbnailSize + padding * 2 };

				const f32 panelWidth = ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ScrollbarSize;
				int columnCount = static_cast<int>(panelWidth / cellSize);

				ImGuiTableFlags flags = ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_ScrollY
					| ImGuiTableFlags_PadOuterX | ImGuiTableFlags_SizingFixedFit;

				ImTextureID whiteTexId = GUI::GetTextureID(AssetManager::GetWhiteTexture()->GetHandle());

				if (ImGui::BeginTable("BodyTable", columnCount, flags)) {
					int i = 0;

					for (const File& entry : m_DirectoryEntries) {
						ImGui::PushID(i);

						bool isDirectory = entry.Type == FileType::Directory;
						const char* filename = entry.Name.data();
						const char* filenameEnd = filename + entry.Name.size();
						ImTextureID textureId = GUI::GetTextureID(entry.Thumbnail);

						ImGui::TableNextColumn();

						ImVec2 cursorPos = ImGui::GetCursorPos();

						const ImVec4 backgroundColor = GUI::Colors::Lighten(GUI::Appearance::GetColors().WindowBackground, 0.11f);
						const ImVec4 borderColor = GUI::Colors::Lighten(GUI::Appearance::GetColors().WindowBackground, 0.20f);

						// Foreground Image
						ImGui::SetCursorPos({ cursorPos.x + padding, cursorPos.y + padding });
						ImGui::SetItemAllowOverlap();
						ImGui::Image(whiteTexId, { backgroundThumbnailSize.x - padding * 2.0f, backgroundThumbnailSize.y - padding * 2.0f },
							{ 0, 0 }, { 1, 1 }, backgroundColor, borderColor);

						DrawItemPopupMenu(entry);

						if (ImGui::IsItemHovered()) {
							ImGui::BeginTooltip();
							ImGui::Text("Mem: %s", String::BytesToString(entry.Thumbnail->GetEstimatedSize()).c_str());
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
						GUI::ClippedText(clipRect.Min, clipRect.Max, filename, filenameEnd, nullptr, { 0, 0 }, nullptr, clipRect.GetSize().x);

						ImGui::SetCursorPos({ cursorPos.x + padding * 2.0f, cursorPos.y + backgroundThumbnailSize.y - GUI::Appearance::GetFonts().DefaultBoldFont->FontSize - padding * 2.0f });
						ImGui::BeginDisabled();
						ImGui::PushFont(GUI::Appearance::GetFonts().DefaultBoldFont);
						ImGui::TextUnformatted(entry.TypeString.c_str());
						ImGui::PopFont();
						ImGui::EndDisabled();

						ImGui::PopID(); ++i;
					}

					ImGui::EndTable();
				}

				if (refreshDirectory) {
					refreshDirectory = false;
					LoadDirectoryEntries();
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

	void AssetPanel::DrawItemPopupMenu(const File& entry)
	{
		if (ImGui::BeginPopupContextItem("ItemPopupMenu")) {
			if (ImGui::MenuItemEx("Show in explorer", SW_ICON_MAGNIFY)) {
				std::filesystem::path pathToFile = entry.FilePath;
				std::filesystem::path toOpen = ProjectContext::Get()->GetAssetDirectory() / pathToFile;

				if (entry.Type == FileType::Directory)
					FileSystem::RevealFolderInFileExplorer(toOpen);
				else
					FileSystem::OpenFolderAndSelectItem(toOpen);
			}

			if (ImGui::MenuItemEx("Delete", SW_ICON_DELETE)) {
				std::filesystem::path pathToFile = entry.FilePath;

				std::filesystem::remove(pathToFile);
			}

			ImGui::EndPopup();
		}
	}

}
