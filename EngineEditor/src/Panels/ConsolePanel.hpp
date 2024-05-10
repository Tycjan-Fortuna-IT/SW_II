/**
 * @file ConsolePanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "GUI/Panel.hpp"

namespace SW {

	struct ConsoleMessage
	{
		LogLevel Level = LogLevel::LOG_LEVEL_TRACE;
		std::string Message;
		time_t Time;
	};

	/**
	 * @brief The ConsolePanel class represents a panel that displays the console window.
	 *        in a tree-like structure.
	 */
	class ConsolePanel final : public Panel
	{
	public:
		/**
		 * @brief Constructs a ConsolePanel object with an optional name.
		 * 
		 * @param name The name of the panel. Default is "Console".
		 */
		explicit ConsolePanel(const char* name = "Console");

		/**
		 * @brief Default destructor for the ConsolePanel class.
		 */
		~ConsolePanel() override = default;

		/** @brief Copy constructor (deleted). */
		ConsolePanel(const ConsolePanel& other) = delete;
		/** @brief Move constructor (deleted). */
		ConsolePanel(ConsolePanel&& other) = delete;
		/** @brief Copy assignment operator (deleted). */
		ConsolePanel& operator=(const ConsolePanel& other) = delete;
		/** @brief Move assignment operator (deleted). */
		ConsolePanel& operator=(ConsolePanel&& other) = delete;

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

}
