#include "StatisticsPanel.hpp"

#include "Core/Application.hpp"
#include "Core/Renderer/Renderer2D.hpp"
#include "GUI/Icons.hpp"
#include "GUI/GUI.hpp"
#include "Core/Editor/EditorSettings.hpp"

namespace SW {

	StatisticsPanel::StatisticsPanel()
		: Panel("Statistics", SW_ICON_INFORMATION_VARIANT, true) {}

	void StatisticsPanel::OnRender()
	{
		GUI::ScopedColor PanelBackground(ImGuiCol_WindowBg, GUI::Appearance::GetColors().ChildBackground);

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

			const ImVec2 maxWidth = ImGui::GetContentRegionAvail();
			ImVec2 graphSize = ImVec2(maxWidth.x, 60.0f);
			ImGui::PlotLines("##FPS", m_FpsValues, static_cast<int>(size), 0, nullptr, 0, m_FpsValues[0] + m_FpsValues[0] / 2.f, graphSize);

			static constexpr ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_AllowItemOverlap
				| ImGuiTreeNodeFlags_Framed
				| ImGuiTreeNodeFlags_Bullet;

			if (ImGui::TreeNodeEx("##stats_tree_node", treeFlags, "%s", SW_ICON_CHART_BAR "  Statistics")) {
				GUI::Properties::BeginProperties("##renderer2d_properties");

				std::string averageFps = std::to_string(static_cast<f64>(avg));
				GUI::Properties::SingleLineTextInputProperty(&averageFps, "FPS", nullptr, ImGuiInputTextFlags_ReadOnly);
				
				std::string frameTime = std::to_string((1.0 / static_cast<f64>(avg)) * 1000.0f);
				GUI::Properties::SingleLineTextInputProperty(&frameTime, "Frame time (ms)", nullptr, ImGuiInputTextFlags_ReadOnly);

				std::string drawCalls = std::to_string(Renderer2D::GetStats().DrawCalls);
				GUI::Properties::SingleLineTextInputProperty(&drawCalls, "2D Draw Calls", nullptr, ImGuiInputTextFlags_ReadOnly);

				std::string quadCount = std::to_string(Renderer2D::GetStats().QuadCount);
				GUI::Properties::SingleLineTextInputProperty(&quadCount, "Quads", nullptr, ImGuiInputTextFlags_ReadOnly);

				std::string vertexCount = std::to_string(Renderer2D::GetStats().GetTotalVertexCount());
				GUI::Properties::SingleLineTextInputProperty(&vertexCount, "Vertices", nullptr, ImGuiInputTextFlags_ReadOnly);

				std::string indexCount = std::to_string(Renderer2D::GetStats().GetTotalIndexCount());
				GUI::Properties::SingleLineTextInputProperty(&indexCount, "Indices", nullptr, ImGuiInputTextFlags_ReadOnly);
				
				GUI::Properties::EndProperties();

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("##settings_tree_node", treeFlags, "%s", SW_ICON_SETTINGS_OUTLINE "  Editor Settings")) {
				GUI::Properties::BeginProperties("##editor_settings");
				if (GUI::Properties::CheckboxProperty(&EditorSettings::Get().VSync, "Enable / Disable VSync")) {
					Application::Get()->GetWindow()->SetVSync(EditorSettings::Get().VSync);
				}
				GUI::Properties::CheckboxProperty(&EditorSettings::Get().ShowPhysicsColliders, "Show Physics Colliders");
				GUI::Properties::EndProperties();
				
				ImGui::TreePop();
			}

			OnEnd();
		}
	}

}
