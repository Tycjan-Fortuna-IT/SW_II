/**
 * @file Window.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.3
 * @date 2024-01-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/KeyCode.hpp"
#include "Core/MouseCode.hpp"

struct GLFWwindow;

namespace SW
{

	/**
	 * @brief Internal data of the embedded icon, helper structure.
	 */
	struct EmbeddedIcon final
	{
		const u8* Data = nullptr; /** @brief Embedded binary icon of the application. */
		const i32 Size = 0;       /** @brief Number of embedded binary fragments. */
	};

	/**
	 * @brief Window specification structure.
	 */
	struct WindowSpecification
	{
		std::string Title   = "No title"; /** @brief Window title. */
		u16 Width           = 1280;       /** @brief Window width. */
		u16 Height          = 720;        /** @brief Window height. */
		bool VSync          = false;      /** @brief Vertical synchronization. */
		EmbeddedIcon Icon   = {};         /** @brief Window's icon, shown only if toolbar is enabled */
		bool DisableToolbar = false;      /** @brief Whether the default window's toolbar should be visible. */
		bool OverTitlebar   = false;      /** @brief Whether the mouse cursor is hovering over custom titlebar */
	};

	/**
	 * @brief Engine's abstraction of a window.
	 */
	class Window final
	{
	public:
		/**
		 * @brief Construct a new Window object
		 *
		 * @param specification
		 */
		Window(const WindowSpecification& specification);

		/**
		 * @brief Destroy the Window object
		 */
		~Window();

		/**
		 * @brief Window update function. Polls events and swaps buffers.
		 */
		void OnUpdate() const;

		/**
		 * @brief Get the Width object
		 *
		 * @return u16
		 */
		u16 GetWidth() const { return m_Specification.Width; }

		/**
		 * @brief Get the Height object
		 *
		 * @return u16
		 */
		u16 GetHeight() const { return m_Specification.Height; }

		/**
		 * @brief Get the GLFW Handle object
		 *
		 * @return GLFWwindow*
		 */
		GLFWwindow* GetHandle() const { return m_Handle; }

		/**
		 * @brief Get the window specification.
		 * 
		 * @return const WindowSpecification& 
		 */
		const WindowSpecification& GetSpecification() const { return m_Specification; }

		/**
		 * @brief Get the mutable window specification.
		 * 
		 * @return WindowSpecification& 
		 */
		WindowSpecification& GetSpecificationMutable() { return m_Specification; }

		/**
		 * @brief Enable or disable vertical synchronization.
		 *
		 * @param enabled Whether to enable or disable vertical synchronization.
		 */
		void SetVSync(bool enabled);

		/**
		 * @brief Set the state of titlebar drag, used for the window handle to allow for custom way of drag.
		 *
		 * @param over Whether to enable or disable custom drag.
		 */
		void RegisterOverTitlebar(bool over);

		/**
		 * @brief Minimize the window.
		 */
		void Minimize() const;

		/**
		 * @brief Maximize the window.
		 */
		void Maximize() const;

		/**
		 * @brief Restore the window. (unmaximize)
		 */
		void Restore() const;

		/**
		 * @brief Check if the Window is currently maximized.
		 *
		 * @return Whether the windows is currently maximized.
		 */
		bool IsCurrentlyMaximized() const;

	public:
		Eventing::Event<KeyCode> KeyPressedEvent; // Event for key press.
		Eventing::Event<KeyCode> KeyReleasedEvent; // Event for key release.
		Eventing::Event<KeyCode> KeyRepeatEvent; // Event for key repeat.

		Eventing::Event<MouseCode> MousePressedEvent; // Event for mouse button press.
		Eventing::Event<MouseCode> MouseReleasedEvent; // Event for mouse button release.

		Eventing::Event<f32, f32> MouseScrollWheelEvent; // Event for mouse scroll wheel.

		Eventing::Event<f32, f32> ResizedEvent; // Event for window resize.

		Eventing::Event<> CloseEvent; // Event for window close.

	private:
		WindowSpecification m_Specification; /** @brief Window specification. */
		GLFWwindow* m_Handle = nullptr;      /** @brief GLFW window handle. */
	};

} // namespace SW
