#include "ConsolePanel.hpp"

#include "GUI/Icons.hpp"

namespace SW
{
	ConsolePanel::ConsolePanel() : Panel("Console", SW_ICON_CONSOLE, true)
	{
		m_Messages.reserve(100);
	}

	void ConsolePanel::OnUpdate(Timestep /*dt*/)
	{
	}

	void ConsolePanel::OnRender()
	{
		if (OnBegin())
		{

			if (ImGui::Button(SW_ICON_DELETE " Clear"))
			{
				m_Messages.clear();
			}

			constexpr ImGuiTableFlags flags =
			    ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Sortable |
			    ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;

			if (ImGui::BeginTable("##ConsoleTable", 3, flags))
			{
				ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_NoSort);
				ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_PreferSortDescending, 0.f, 1);
				ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_NoSort);
				ImGui::TableHeadersRow();

				if (ImGuiTableSortSpecs* sortingData = ImGui::TableGetSortSpecs())
				{
					if (sortingData->Specs)
					{
						std::sort(m_Messages.begin(), m_Messages.end(),
						          [sortingData](const ConsoleMessage& a, const ConsoleMessage& b) {
							          return sortingData->Specs->SortDirection == ImGuiSortDirection_Ascending
							                     ? a.Time < b.Time
							                     : a.Time > b.Time;
						          });
					}
				}

				ImGuiListClipper clipper;
				clipper.Begin((int)m_Messages.size());

				while (clipper.Step())
				{
					for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
					{
						const ConsoleMessage& message = m_Messages[(size_t)row_n];
						ImGui::TableNextRow();
						ImGui::TableNextColumn();

						ImGui::PushStyleColor(ImGuiCol_Text, GetLevelColor(message.Level));
						ImGui::Text("%s %s", GetLevelIcon(message.Level), GetLevelName(message.Level));

						ImGui::TableNextColumn();

						std::stringstream timeString = std::stringstream();
						timeString << std::put_time(std::localtime(&message.Time), "%T");
						ImGui::TextUnformatted(timeString.str().c_str());

						ImGui::TableNextColumn();

						ImGui::TextWrapped(message.Message.c_str());
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTable();
			}

			OnEnd();
		}
	}

	void ConsolePanel::AddMessage(const ConsoleMessage& message)
	{
		m_Messages.push_back(message);
	}

	ImVec4 ConsolePanel::GetLevelColor(LogLevel level) const
	{
		switch (level)
		{
		case LogLevel::LOG_LEVEL_TRACE:
			return {0.50f, 0.50f, 0.50f, 1.00f}; // Grey
		case LogLevel::LOG_LEVEL_INFO:
			return {0.10f, 0.60f, 0.10f, 1.00f}; // Green
		case LogLevel::LOG_LEVEL_DEBUG:
			return {0.00f, 0.50f, 0.50f, 1.00f}; // Cyan
		case LogLevel::LOG_LEVEL_WARN:
			return {0.60f, 0.60f, 0.10f, 1.00f}; // Yellow
		case LogLevel::LOG_LEVEL_ERROR:
			return {0.90f, 0.25f, 0.25f, 1.00f}; // Red
		case LogLevel::LOG_LEVEL_FATAL:
			return {0.60f, 0.20f, 0.80f, 1.00f}; // Purple
		}

		return {1.00f, 1.00f, 1.00f, 1.00f};
	}

	const char* ConsolePanel::GetLevelName(LogLevel level) const
	{
		switch (level)
		{
		case LogLevel::LOG_LEVEL_TRACE:
			return "Trace";
		case LogLevel::LOG_LEVEL_INFO:
			return "Info";
		case LogLevel::LOG_LEVEL_DEBUG:
			return "Debug";
		case LogLevel::LOG_LEVEL_WARN:
			return "Warning";
		case LogLevel::LOG_LEVEL_ERROR:
			return "Error";
		case LogLevel::LOG_LEVEL_FATAL:
			return "Critical";
		}

		return "Unknown name";
	}

	const char* ConsolePanel::GetLevelIcon(LogLevel level) const
	{
		switch (level)
		{
		case LogLevel::LOG_LEVEL_TRACE:
			return SW_ICON_MESSAGE_TEXT;
		case LogLevel::LOG_LEVEL_INFO:
			return SW_ICON_INFORMATION;
		case LogLevel::LOG_LEVEL_DEBUG:
			return SW_ICON_BUG;
		case LogLevel::LOG_LEVEL_WARN:
			return SW_ICON_ALERT;
		case LogLevel::LOG_LEVEL_ERROR:
			return SW_ICON_CLOSE_OCTAGON;
		case LogLevel::LOG_LEVEL_FATAL:
			return SW_ICON_ALERT_OCTAGON;
		}

		return u8"Unknown name";
	}

} // namespace SW
