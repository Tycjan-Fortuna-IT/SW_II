#include "AssetPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "GUI/Icons.hpp"
#include "Core/Debug/LogSystem.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Core/AssetManager.hpp"
#include "GUI/GUI.hpp"

namespace SW {

	static std::unordered_map<std::string, FileType> s_FileTypes = {
		{ ".png",		FileType::Texture },
		{ ".jpg",		FileType::Texture },
		{ ".jpeg",		FileType::Texture },
		{ ".bmp",		FileType::Texture },
	};

	AssetPanel::AssetPanel(const char* name)
		: Panel(name, SW_ICON_FOLDER_STAR, true)
	{
		m_AssetsDirectory = "assets";
		m_CurrentDirectory = m_AssetsDirectory;

		LoadDirectoryEntries();
	}

	void AssetPanel::OnUpdate(Timestep dt)
	{

	}

	void AssetPanel::OnRender()
	{
		if (OnBegin()) {
			static i32 cols = 10;
			static f32 thumbnailSize = 64.f;

			bool atAssetsDir = m_CurrentDirectory == m_AssetsDirectory;

			ImGui::Columns(3, 0, false);

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

			ImGui::DragInt("Items per row", &cols, 1, 1, 10);

			ImGui::NextColumn();

			ImGui::DragFloat("Thumbnail size", &thumbnailSize, 8.f, 32.f);
			
			ImGui::Columns(1);

			ImGui::Columns(cols, 0 , false);

			for (File entry : m_DirectoryEntries) {
				ImGui::Image(GUI::GetTextureID(entry.Thumbnail), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("Mem: %s", String::BytesToString(entry.Thumbnail->GetEstimatedSize()).c_str());
					ImGui::EndTooltip();

					if (ImGui::IsMouseDoubleClicked(0)) {
						m_CurrentDirectory = m_CurrentDirectory / entry.Name;
						LoadDirectoryEntries();
					}
				}

				ImGui::Text(entry.Name.c_str());

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", entry.FilePath.data(), entry.FilePath.size() + 1);
					ImGui::TextUnformatted(entry.FilePath.c_str());
					ImGui::EndDragDropSource();
				}

				ImGui::NextColumn();
			}

			ImGui::Columns(1);

			OnEnd();
		}
	}

	void AssetPanel::LoadDirectoryEntries()
	{
		m_DirectoryEntries.clear();

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			FileType fileType = FileType::Unknown;

			// todo, introduce separate thumbnail system with lower resolution textures (like 128 x 128)
			Texture2D* thumbnail = AssetManager::GetBlackTexture();

			bool isDirectory = entry.is_directory();

			if (!isDirectory) {
				auto it = s_FileTypes.find(entry.path().extension().string());

				if (it != s_FileTypes.end())
					fileType = it->second;

				if (fileType == FileType::Texture)
					thumbnail = AssetManager::GetTexture2D(entry.path().string().c_str());
			} else {
				thumbnail = AssetManager::GetTexture2D("assets/icons/editor/DirectoryIcon.png");
			}

			File file = { 
				.Name = entry.path().filename().string(),
				.FilePath = entry.path().string(),
				.IsDirectory = isDirectory, 
				.Thumbnail = thumbnail, 
				.Type = fileType
			};

			m_DirectoryEntries.emplace_back(file);
		}
	}

}
