#include "ProjectSettingsPanel.hpp"
#include "EngineEditor.hpp"

namespace SW
{
	ProjectSettingsPanel::ProjectSettingsPanel() : Panel("Project Settings", SW_ICON_SETTINGS, false)
	{
	}

	void ProjectSettingsPanel::OnUpdate(Timestep /*dt*/)
	{
	}

	void ProjectSettingsPanel::OnRender()
	{
		if (OnBegin())
		{
			if (ProjectContext::HasContext())
			{
				GUI::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 8.0f));
				GUI::ScopedStyle IndentSpacing(ImGuiStyleVar_IndentSpacing, 0.0f);

				if (GUI::Layout::BeginHeaderCollapse("General Settings"))
				{
					GUI::Properties::BeginProperties("##project_settings_panel_general");

					GUI::Properties::SingleLineTextInputDefferedProperty<128>(&ProjectContext::Get()->GetConfig().Name,
					                                                          "Name", "Project name");

					std::filesystem::path projectPath = ProjectContext::Get()->GetAssetDirectory();
					if (GUI::Properties::DrawFolderPickerProperty(
					        &projectPath, "", "Assets directory",
					        "Select the directory where project assets are stored. DO NOT CHANGE while any scene or "
					        "assets are loaded."))
					{
						if (!projectPath.empty())
						{
							ProjectContext::Get()->GetConfig().AssetsDirectory = projectPath;

							EngineEditor::AssetDirContentChangedEvent.Invoke();
						}
					}

					std::filesystem::path assetRegistryPath = ProjectContext::Get()->GetAssetRegistryPath();
					if (GUI::Properties::DrawFilePickerProperty(
					        &assetRegistryPath, "", {{"SW Asset Registry", "sw_registry"}}, "Asset registry path",
					        "Select the path to the asset registry file. DO NOT CHANGE while any scene or assets are "
					        "loaded."))
					{
						if (!assetRegistryPath.empty())
						{
							ProjectContext::Get()->GetConfig().AssetRegistryPath = assetRegistryPath;
						}
					}

					std::filesystem::path audioRegistryPath = ProjectContext::Get()->GetAudioRegistryPath();
					if (GUI::Properties::DrawFilePickerProperty(
					        &audioRegistryPath, "", {{"SW Audio Registry", "sw_registry"}}, "Audio registry path",
					        "Select the path to the audio registry file. DO NOT CHANGE while any scene or assets are "
					        "loaded."))
					{
						if (!audioRegistryPath.empty())
						{
							ProjectContext::Get()->GetConfig().AudioRegistryPath = audioRegistryPath;
						}
					}

					GUI::Properties::EndProperties();

					GUI::Layout::EndHeaderCollapse();
				}
			}
			else
			{
				ImGui::Text("No project loaded.");
			}

			OnEnd();
		}
	}

} // namespace SW
