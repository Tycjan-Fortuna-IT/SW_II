/**
 * @file Panel.hpp
 * @version 0.1.0
 * @date 2024-05-12
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#include <string>

#include "Core/Defines.hpp"
#include "Core/Timestep.hpp"

namespace SW {

	enum class PanelType : u8
	{
		AssetPanel,
		ConsolePanel,
		PropertiesPanel,
		SceneHierarchyPanel,
		SceneViewportPanel,
		StatisticsPanel,
		AssetManagerPanel,
		AudioEventsPanel,
		ProjectSettingsPanel,
	};

	/**
	 * @brief Represents a GUI panel.
	 * 
	 * The Panel class provides a base class for creating GUI panels in the application.
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
		virtual ~Panel() = default;

		Panel(const Panel& other) = delete;
		Panel(Panel&& other) = delete;
		Panel& operator=(const Panel& other) = delete;
		Panel& operator=(Panel&& other) = delete;

		virtual PanelType GetPanelType() const = 0;

		/**
		 * @brief Called every frame to update the panel.
		 * @param dt The time since the last frame.
		 */
		virtual void OnUpdate(Timestep dt) = 0;

		/**
		 * @brief Called every frame to render the panel.
		 */
		virtual void OnRender() = 0;

		/**
		 * @brief Checks if the panel is currently showing.
		 * @return True if the panel is showing, false otherwise.
		 */
		bool IsOpen() const { return m_Open; }

		const char* GetName() const { return m_Name; }
		const char* GetIcon() const { return m_Icon; }

		void Open() { m_Open = true; }

		void Close() { m_Open = false; }

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

	protected:
		bool m_Open = false; ///< Flag indicating whether the panel is showing.

	private:
		const char* m_Name; ///< The name of the panel.
		const char* m_Icon; ///< The icon of the panel.

		std::string m_ID;   ///< The ID of the panel.
	};

}
