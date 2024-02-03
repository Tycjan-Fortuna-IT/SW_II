#include "StatisticsPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "Core/Application.hpp"
#include "Core/Renderer/Renderer2D.hpp"
#include "GUI/Icons.hpp"
#include "GUI/GUI.hpp"
#include "Core/Editor/EditorSettings.hpp"

namespace SW {

	StatisticsPanel::StatisticsPanel(const char* name)
		: Panel(name, SW_ICON_INFORMATION_VARIANT, true) {}

	void StatisticsPanel::OnRender()
	{
		if (OnBegin()) {
			f32 avg = 0.0f;

			const u64 size = m_FrameTimes.size();
			if (size >= 200)
				m_FrameTimes.erase(m_FrameTimes.begin());

			m_FrameTimes.emplace_back(ImGui::GetIO().Framerate);

			for (u32 i = 0; i < size; i++) {
				const float frameTime = m_FrameTimes[i];
				m_FpsValues[i] = frameTime;
				avg += frameTime;
			}

			avg /= static_cast<f32>(size);

			ImGui::PushItemWidth(-1);
			ImGui::PlotLines("##FPS", m_FpsValues, static_cast<int>(size));
			ImGui::PopItemWidth();

			static constexpr ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_AllowItemOverlap
				| ImGuiTreeNodeFlags_Framed
				| ImGuiTreeNodeFlags_FramePadding
				| ImGuiTreeNodeFlags_Bullet;

			if (ImGui::TreeNodeEx("##stats_tree_node", treeFlags, "%s", SW_ICON_CHART_BAR "  Statistics")) {
				GUI::BeginProperties("##renderer2d_properties");
				GUI::DrawReadonlyTextProperty(std::to_string(static_cast<f64>(avg)).c_str(), "FPS");
				GUI::DrawReadonlyTextProperty(std::to_string((1.0 / static_cast<f64>(avg)) * 1000.0f).c_str(), "Frame time (ms)");
				GUI::DrawReadonlyTextProperty(std::to_string(Renderer2D::GetStats().DrawCalls).c_str(), "2D Draw Calls");
				GUI::DrawReadonlyTextProperty(std::to_string(Renderer2D::GetStats().QuadCount).c_str(), "Quads");
				GUI::DrawReadonlyTextProperty(std::to_string(Renderer2D::GetStats().GetTotalVertexCount()).c_str(), "Vertices");
				GUI::DrawReadonlyTextProperty(std::to_string(Renderer2D::GetStats().GetTotalIndexCount()).c_str(), "Indices");
				GUI::EndProperties();

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("##settings_tree_node", treeFlags, "%s", SW_ICON_SETTINGS_OUTLINE "  Editor Settings")) {
				GUI::BeginProperties("##editor_settings");
				if (GUI::DrawBooleanProperty(EditorSettings::Get().VSync, "Enable / Disable VSync")) {
					Application::Get()->GetWindow()->SetVSync(EditorSettings::Get().VSync);
				}
				GUI::DrawBooleanProperty(EditorSettings::Get().ShowPhysicsColliders, "Show Physics Colliders");
				GUI::EndProperties();
				
				ImGui::TreePop();
			}

			OnEnd();
		}
	}

}
