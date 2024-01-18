/**
 * @file Panel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <string>

#include "Core/Defines.hpp"
#include "Core/Timestep.hpp"

namespace SW {

	/**
	 * @brief Represents a GUI panel.
	 * 
	 * The Panel class provides a base class for creating GUI panels in an application.
	 * It contains common functionality for managing the panel's visibility, name, and icon.
	 * Derived classes can override the OnUpdate and OnRender methods to implement custom behavior.
	 */
	class Panel
	{
	public:
		/**
		 * @brief Constructs a Panel object.
		 * @param name The name of the panel.
		 * @param icon The icon of the panel.
		 * @param defaultShow Whether the panel should be shown by default.
		 */
		Panel(const char* name = "Unnamed Panel", const char* icon = "", bool defaultShow = false);

		/**
		 * @brief Destroys the Panel object.
		 */
		virtual ~Panel() = default;

		/** @brief Copy constructor (deleted). */
		Panel(const Panel& other) = delete;
		/** @brief Move constructor (deleted). */
		Panel(Panel&& other) = delete;
		/** @brief Copy assignment operator (deleted). */
		Panel& operator=(const Panel& other) = delete;
		/** @brief Move assignment operator (deleted). */
		Panel& operator=(Panel&& other) = delete;

		/**
		 * @brief Called every frame to update the panel.
		 * @param dt The time since the last frame.
		 */
		virtual void OnUpdate(Timestep dt) {}

		/**
		 * @brief Called every frame to render the panel.
		 */
		virtual void OnRender() = 0;

		/**
		 * @brief Checks if the panel is currently showing.
		 * @return True if the panel is showing, false otherwise.
		 */
		bool IsShowing() const { return m_Showing; }

	protected:
		/**
		 * @brief Called when the panel begins.
		 * @param flags Additional flags for the panel.
		 * @return True if the panel was shown, false otherwise.
		 */
		bool OnBegin(i32 flags = 0);

		/**
		 * @brief Called when the panel ends.
		 */
		void OnEnd() const;

		bool m_Showing = false; ///< Flag indicating whether the panel is showing.

	private:
		std::string m_ID;   ///< The ID of the panel.
		std::string m_Name; ///< The name of the panel.
		std::string m_Icon; ///< The icon of the panel.
	};

}
