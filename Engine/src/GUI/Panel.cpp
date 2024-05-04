#include "Panel.hpp"

namespace SW {

	Panel::Panel(const char* name, const char* icon, bool defaultShow)
		: m_Showing(defaultShow), m_Name(name), m_Icon(icon)
	{
		m_ID = std::format(" {} {}\t\t###{}", icon, m_Name, m_Name);
	}

	bool Panel::OnBegin(i32 flags)
	{
		if (!m_Showing)
			return false;

		ImGui::SetNextWindowSize(ImVec2(480, 640), ImGuiCond_FirstUseEver);

		ImGui::Begin(m_ID.c_str(), &m_Showing, flags | ImGuiWindowFlags_NoCollapse);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2.0f));

		return true;
	}

	void Panel::OnEnd() const
	{
		ImGui::PopStyleVar();
		ImGui::End();
	}

}
