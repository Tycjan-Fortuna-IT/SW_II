#include "ProjectSettingsPanel.hpp"

namespace SW {

	ProjectSettingsPanel::ProjectSettingsPanel()
		: Panel("Project Settings", SW_ICON_SETTINGS, false) {}

	void ProjectSettingsPanel::OnUpdate(Timestep /*dt*/)
	{

	}

	void ProjectSettingsPanel::OnRender()
	{
		if (OnBegin()) {
			if (ProjectContext::HasContext()) {
				GUI::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 8.0f));
				GUI::ScopedStyle IndentSpacing(ImGuiStyleVar_IndentSpacing, 0.0f);

				if (GUI::Layout::BeginHeaderCollapse("General Settings")) {
					GUI::Properties::BeginProperties("##project_settings_panel_general");

					GUI::Properties::SingleLineTextInputDefferedProperty<128>(&ProjectContext::Get()->GetConfig().Name, "Name", "Project name");

					std::filesystem::path projectPath = ProjectContext::Get()->GetAssetDirectory();
					if (GUI::Properties::DrawFolderPickerProperty(&projectPath, "", "Assets directory", "Select the directory where project assets are stored. DO NOT CHANGE while any scene or assets are loaded.")) {
						if (!projectPath.empty()) {
							ProjectContext::Get()->GetConfig().AssetsDirectory = projectPath;
							EventSystem::Emit(EVENT_CODE_ASSET_DIR_CONTENT_CHANGED);
						}
					}

					GUI::Properties::EndProperties();

					GUI::Layout::EndHeaderCollapse();
				}
			} else {
				ImGui::Text("No project loaded.");
			}

			OnEnd();
		}
	}

}
