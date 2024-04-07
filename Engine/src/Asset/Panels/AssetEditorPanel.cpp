#include "AssetEditorPanel.hpp"

namespace SW {

	AssetEditorPanel::AssetEditorPanel(const char* name = "Unnamed Panel", const char* icon = "")
		: m_Name(name), m_Icon(icon), m_MinSize(200, 400), m_MaxSize(2000, 2000)
	{
		m_ID = fmt::format(" {} {}\t\t###{}", icon, m_Name, m_Name);
	}

	void AssetEditorPanel::OnRender()
	{
		if (!m_IsOpen)
			return;

		const bool wasOpen = m_IsOpen;

		ImGui::SetNextWindowSizeConstraints(m_MinSize, m_MaxSize);

		OnWindowStylePush();
		ImGui::Begin(m_ID.c_str(), &m_IsOpen);
		OnWindowStylePop();

		Render();

		ImGui::End();

		if (wasOpen && !m_IsOpen)
			OnClose();
	}

	void AssetEditorPanel::Open()
	{
		if (m_IsOpen)
			return;

		m_IsOpen = true;

		OnOpen();
	}

	void AssetEditorPanel::Close()
	{
		if (!m_IsOpen)
			return;

		m_IsOpen = false;

		OnClose();
	}

}
