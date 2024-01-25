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
		m_AssetsDirectory = std::filesystem::current_path() / "assets" / "textures";
		m_CurrentDirectory = m_AssetsDirectory;

		LoadDirectoryEntries();
	}

	void AssetPanel::OnUpdate(Timestep dt)
	{

	}

	void AssetPanel::OnRender()
	{
		if (OnBegin()) {

			for (File entry : m_DirectoryEntries) {
				ImGui::Image(GUI::GetTextureID(entry.Thumbnail), { 60.f, 60.f }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("Mem: %s", String::BytesToString(entry.Thumbnail->GetEstimatedSize()).c_str());
					ImGui::EndTooltip();
				}
				ImGui::SameLine();
			}

			OnEnd();
		}
	}

	void AssetPanel::LoadDirectoryEntries()
	{
		m_DirectoryEntries.clear();

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			FileType fileType = FileType::Unknown;
			Texture2D* thumbnail = AssetManager::GetWhiteTexture(); // todo

			bool isDirectory = entry.is_directory();

			if (!isDirectory) {
				auto it = s_FileTypes.find(entry.path().extension().string());

				if (it != s_FileTypes.end())
					fileType = it->second;

				if (fileType == FileType::Texture)
					thumbnail = AssetManager::GetTexture2D(entry.path().string().c_str());
			}

			File file = { .Name = entry.path().filename().string(), .IsDirectory = isDirectory, .Thumbnail = thumbnail, .Type = fileType};

			m_DirectoryEntries.emplace_back(file);
		}
	}

}
