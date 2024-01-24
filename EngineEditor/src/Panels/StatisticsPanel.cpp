#include "StatisticsPanel.hpp"

#include <imgui.h>

#include "Core/Utils/Utils.hpp"
#include "Core/Application.hpp"
#include "Core/Renderer/Renderer2D.hpp"
#include "GUI/Icons.hpp"

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

			ImGui::Text("FPS: %lf", static_cast<f64>(avg));
			ImGui::Text("Frame time (ms): %lf", (1.0 / static_cast<f64>(avg)) * 1000.0f);

			static bool VSync = true;
			if (ImGui::Checkbox("VSync", &VSync)) {
				Application::Get()->GetWindow()->SetVSync(VSync);
			}

			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
			ImGui::Text("Quads: %d", Renderer2D::GetStats().QuadCount);
			ImGui::Text("Vertices: %d", Renderer2D::GetStats().GetTotalVertexCount());
			ImGui::Text("Indices: %d", Renderer2D::GetStats().GetTotalIndexCount());

			OnEnd();
		}
	}

}
