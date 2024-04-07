#include "Popups.hpp"

#include "GUI.hpp"
#include "Core/Utils/FileSystem.hpp"

#define EMPTY_SCENE_CONTENT \
"Entities: \n"

#define EMPTY_SPRITESHEET_CONTENT \
R"(Spritesheet:
  TextureHandle: 0
  ViewZoom: 1
  GridSize: 64
  CenterOffset: [0, 0]
  ViewPos: [0, 0]
  ShowImageBorders: false
  ExportPath: ""
  Sprites:
    {})"

namespace SW::GUI::Popups {

	bool DrawAddNewFilePopup(const std::filesystem::path& destinationDir, bool* opened)
	{
		bool added = false;

		if (*opened)
			ImGui::OpenPopup("Add new file");

		if (ImGui::BeginPopupModal("Add new file", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Create new file modal.");
			ImGui::Separator();

			static AssetType filetype = AssetType::Unknown;

			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Choose file type to create: ");
			ImGui::SameLine();

			GUI::DrawSelectable(filetype, {
				GUI::SelectOption<AssetType>{ "Directory", AssetType::Directory },
				GUI::SelectOption<AssetType>{ "Scene", AssetType::Scene },
				GUI::SelectOption<AssetType>{ "Spritesheet", AssetType::Spritesheet },
			});

			static std::string filename = "";

			if (filetype != AssetType::Unknown) {
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("Type the file name:              ");
				ImGui::SameLine();

				GUI::DrawSingleLineTextInput(filename);
				ImGui::SameLine();

				switch (filetype) {
					case AssetType::Scene:
					{
						ImGui::TextUnformatted(".sw_scene");
					} break;
					case AssetType::Spritesheet:
					{
						ImGui::TextUnformatted(".sw_spritesh");
					} break;
				}
			}

			if (ImGui::Button("OK", ImVec2(120, 0))) {
				std::string name = filename;

				switch (filetype) {
					case AssetType::Directory:
					{
						std::filesystem::path newDirPath = destinationDir / name;

						if (!FileSystem::CreateEmptyDirectory(newDirPath))
							SW_ERROR("Failed to create new directory: {}", name);
					} break;
					case AssetType::Scene:
					{
						std::filesystem::path newFilePath = destinationDir / name += ".sw_scene";

						if (!FileSystem::CreateFileWithContent(newFilePath, EMPTY_SCENE_CONTENT))
							SW_ERROR("Failed to create new file: {}", name);
					} break;
					case AssetType::Spritesheet:
					{
						std::filesystem::path newFilePath = destinationDir / name += ".sw_spritesh";

						if (!FileSystem::CreateFileWithContent(newFilePath, EMPTY_SPRITESHEET_CONTENT))
							SW_ERROR("Failed to create new spritesheet: {}", name);
					} break;
				}

				filetype = AssetType::Unknown;
				filename.clear();
				*opened = false;

				added = true;

				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				filetype = AssetType::Unknown;
				filename.clear();
				*opened = false;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		return added;
	}

	bool DrawDeleteFilePopup(const std::filesystem::path& filepath, bool* opened)
	{
		bool removed = false;

		if (*opened)
			ImGui::OpenPopup("Delete?");

		if (ImGui::BeginPopupModal("Delete?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			std::string toDelete = filepath.filename().string();

			ImGui::Text("This file fill be deleted!  [%s]", toDelete.c_str());
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) {
				std::filesystem::remove(filepath);

				*opened = false;
				removed = true;

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

		return removed;
	}

	bool DrawDeleteFileToRenamePopup(const std::filesystem::path& filepath, bool* opened)
	{
		bool renamed = false;

		if (*opened)
			ImGui::OpenPopup("Rename file");

		if (ImGui::BeginPopupModal("Rename file", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Rename file modal. Remember about the extension!");
			ImGui::Separator();

			std::string currentName = filepath.filename().string();

			ImGui::TextUnformatted("Current name: ");
			ImGui::SameLine();
			ImGui::TextUnformatted(currentName.c_str());

			ImGui::TextUnformatted("Type the new name:");
			ImGui::SameLine();

			static std::string filename = currentName;

			GUI::DrawSingleLineTextInput(filename);

			if (ImGui::Button("OK", ImVec2(120, 0))) {
				if (!FileSystem::RenameFile(filepath, filename))
					SW_ERROR("Failed to rename the file: {}", filepath.string());

				*opened = false;
				filename.clear();

				renamed = true;

				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				*opened = false;
				filename.clear();

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		return renamed;
	}

	void FontSourceImportDialog::Open(AssetHandle handle)
	{
		Reset();

		m_Data.FontSourceHandle = handle;

		ImGui::OpenPopup("Font Source Import Dialog");
	}

	void FontSourceImportDialog::OnRender()
	{
		constexpr ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_FramePadding;

		GUI::ScopedStyle WindowPadding(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
		GUI::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 8.0f));
		
		bool closeable = true;

		// Always should be in the middle of the screen
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		
		if (ImGui::BeginPopupModal("Font Source Import Dialog", &closeable)) {

			const bool generalOpen = ImGui::TreeNodeEx("FontImport_General", treeFlags, "%s", "General Import Options");
			
			if (generalOpen) {
				GUI::BeginProperties("##font_import_general_property");

				GUI::DrawSaveFilePickerProperty(m_Data.OutputPath, { { "SW Font file", "sw_font" } }, "Font output file");

				GUI::EndProperties();
			
				ImGui::TreePop();
			}

			const bool advancedOpen = ImGui::TreeNodeEx("FontImport_Advanced", treeFlags, "%s", "Advanced Import Options");
			
			if (advancedOpen) {
				GUI::BeginProperties("##font_import_advanced_property");
				
				GUI::DrawRadioButtonProperty(m_Data.CharsetType, {
					GUI::SelectOption<FontCharsetType>{ "ASCII", FontCharsetType::ASCII },
					GUI::SelectOption<FontCharsetType>{ "ALL", FontCharsetType::ALL }
				}, "Charset Type", "Define which set of characters to use (ALL is much more memory intensive than ASCII)");

				GUI::EndProperties();
				
				ImGui::TreePop();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

			const f32 width = ImGui::GetContentRegionAvail().x;

			bool isDataValid = (bool)m_Data;

			if (!isDataValid) {
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			if (ImGui::Button("Import", ImVec2(width / 2.f, 30.f))) {
				FontAssetImporter::Import(m_Data);

				ImGui::CloseCurrentPopup();
			}

			if (!isDataValid) {
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}

			ImGui::SameLine();

			ImGui::SetItemDefaultFocus();

			if (ImGui::Button("Cancel", ImVec2(width / 2.f - 10.f, 30.f)))
				ImGui::CloseCurrentPopup();

			ImGui::PopStyleVar(1);

			ImGui::EndPopup();
		}
	}

	void FontSourceImportDialog::Reset()
	{
		m_Data.FontSourceHandle = 0u;
		m_Data.CharsetType = SW::FontCharsetType::ASCII;
		m_Data.OutputPath = "";
	}

}
