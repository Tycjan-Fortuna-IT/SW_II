#include "AssetManagerPanel.hpp"

#include "Core/Project/ProjectContext.hpp"
#include "GUI/Appearance.hpp"

namespace SW {

	AssetManagerPanel::AssetManagerPanel()
		: Panel("Asset Manager", SW_ICON_FILE, false) {}

	void AssetManagerPanel::OnUpdate(Timestep /*dt*/)
	{
 
	}

	void AssetManagerPanel::OnRender()
	{
		if (OnBegin()) {
			if (ProjectContext::HasContext()) {
				GUI::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 8.0f));
				GUI::ScopedStyle IndentSpacing(ImGuiStyleVar_IndentSpacing, 0.0f);

				constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit
					| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;

				if (GUI::Layout::BeginHeaderCollapse("Available assets")) {
					const std::map<AssetHandle, AssetMetaData>& avail = AssetManager::GetRegistry().GetAvailableAssets();
					ImGui::Text("Available assets: %d", avail.size());

					GUI::Widgets::SearchInput(&m_AvailableSearchString);

					if (ImGui::BeginTable("Assets", 3, tableFlags)) {
						ImGui::TableSetupColumn("ID");
						ImGui::TableSetupColumn("Type");
						ImGui::TableSetupColumn("Path");

						ImGui::TableHeadersRow();

						for (auto& [handle, metadata] : avail) {
							if (
								!m_AvailableSearchString.empty() &&
								metadata.Path.filename().string().find(m_AvailableSearchString) == std::string::npos &&
								std::to_string(handle).find(m_AvailableSearchString) == std::string::npos
							) {
								continue;
							}

							ImGui::TableNextRow();

							ImGui::TableNextColumn();
							ImGui::Text("%llu", handle);

							ImGui::TableNextColumn();
							ImGui::Text("%s", Asset::GetStringifiedAssetType(metadata.Type));

							ImGui::TableNextColumn();
							ImGui::Text("%s", metadata.Path.string().c_str());
						}

						ImGui::EndTable();
					}

					GUI::Layout::EndHeaderCollapse();
				}

				if (GUI::Layout::BeginHeaderCollapse("Loaded assets")) {
					const std::unordered_map<AssetHandle, Asset*>& loaded = AssetManager::GetLoadedAssets();
					ImGui::Text("Loaded assets: %d", loaded.size());

					GUI::Widgets::SearchInput(&m_LoadedSearchString);

					if (ImGui::BeginTable("Assets", 4, tableFlags)) {
						ImGui::TableSetupColumn("ID");
						ImGui::TableSetupColumn("Type");
						ImGui::TableSetupColumn("Path");
						ImGui::TableSetupColumn("Actions (dangerous!)");

						ImGui::TableHeadersRow();

						for (auto& [handle, asset] : loaded) {
							const AssetMetaData& metadata = AssetManager::GetAssetMetaData(handle);
							if (
								!m_LoadedSearchString.empty() &&
								metadata.Path.filename().string().find(m_LoadedSearchString) == std::string::npos &&
								std::to_string(handle).find(m_LoadedSearchString) == std::string::npos
							) {
								continue;
							}

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("%llu", handle);

							ImGui::TableNextColumn();
							ImGui::Text("%s", Asset::GetStringifiedAssetType(metadata.Type));

							ImGui::TableNextColumn();
							ImGui::Text("%s", metadata.Path.string().c_str());

							ImGui::TableNextColumn();

							{
								GUI::ScopedID IDScope(handle);
								GUI::ScopedFont BoldFont(GUI::Appearance::GetFonts().DefaultBoldFont);
								GUI::ScopedStyle NoFramePadding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2.0f));

								if (ImGui::Button("Force Reload"))
									AssetManager::ForceReload(handle);

								ImGui::SameLine();

								if (ImGui::Button("Force unload"))
									AssetManager::ForceUnload(handle);
							}
						}

						ImGui::EndTable();
					}

					GUI::Layout::EndHeaderCollapse();
				}
			} else {
				ImGui::Text("No project loaded.");
			}

			OnEnd();
		}
	}

}
