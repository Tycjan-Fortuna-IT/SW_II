#include "Popups.hpp"

#include "../../EngineEditor/src/Panels/AssetPanel.hpp" // remove
#include "GUI.hpp"
#include "Core/Utils/FileSystem.hpp"

namespace SW::GUI::Popups {

	bool DrawAddNewFilePopup(const std::filesystem::path& destinationDir, bool* opened)
	{
		bool added = false;

		if (*opened)
			ImGui::OpenPopup("Add new file");

		if (ImGui::BeginPopupModal("Add new file", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Create new file modal.");
			ImGui::Separator();

			static FileType filetype = FileType::Unknown;

			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Choose file type to create: ");
			ImGui::SameLine();

			GUI::DrawSelectable(filetype, {
				GUI::SelectOption<FileType>{ "Directory", FileType::Directory },
				GUI::SelectOption<FileType>{ "Scene", FileType::Scene },
			});

			static std::string filename = "";

			if (filetype != FileType::Unknown) {
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("Type the file name:              ");
				ImGui::SameLine();

				GUI::DrawSingleLineTextInput(filename);

				if (filetype == FileType::Scene) {
					ImGui::SameLine();
					ImGui::TextUnformatted(".sw");
				}
			}

			if (ImGui::Button("OK", ImVec2(120, 0))) {
				std::string name = filename;

				switch (filetype)
				{
					case FileType::Directory:
					{
						std::filesystem::path newDirPath = destinationDir / name;

						if (!FileSystem::CreateEmptyDirectory(newDirPath))
							SW_ERROR("Failed to create new directory: {}", name);
					} break;
					case FileType::Scene:
					{
						std::filesystem::path newFilePath = destinationDir / name;

						if (!FileSystem::CreateFileWithContent(newFilePath, "Entities:\n"))
							SW_ERROR("Failed to create new file: {}", name);

						name += ".sw";
					} break;
				}

				filetype = FileType::Unknown;
				filename.clear();
				*opened = false;

				added = true;

				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				filetype = FileType::Unknown;
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

}
