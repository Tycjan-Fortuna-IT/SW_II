/**
 * @file ConsolePanel.hpp
 * @version 0.1.1
 * @date 2024-05-12
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#include "GUI/Panel.hpp"

namespace SW
{
	struct ConsoleMessage
	{
		LogLevel Level = LogLevel::LOG_LEVEL_TRACE;
		std::string Message;
		time_t Time;
	};

	/**
	 * @brief The ConsolePanel class represents a panel that displays the console window with application logs.
	 */
	class ConsolePanel final : public Panel
	{
	public:
		ConsolePanel();
		~ConsolePanel() override = default;

		ConsolePanel(const ConsolePanel& other)            = delete;
		ConsolePanel(ConsolePanel&& other)                 = delete;
		ConsolePanel& operator=(const ConsolePanel& other) = delete;
		ConsolePanel& operator=(ConsolePanel&& other)      = delete;

		PanelType GetPanelType() const override { return PanelType::ConsolePanel; }

		/**
		 * @brief Called every frame to update the panel.
		 * @param dt The time since the last frame.
		 */
		void OnUpdate(Timestep dt) override;

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() override;

		/**
		 * @brief Adds a message to the console.
		 *
		 * @param message The message to be added.
		 */
		void AddMessage(const ConsoleMessage& message);

	private:
		const char* GetLevelName(LogLevel level) const;

		ImVec4 GetLevelColor(LogLevel level) const;

		const char* GetLevelIcon(LogLevel level) const;

	private:
		std::vector<ConsoleMessage> m_Messages;
	};

} // namespace SW
