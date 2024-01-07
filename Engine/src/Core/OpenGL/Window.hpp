/**
 * @file Window.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.3
 * @date 2024-01-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <string>

struct GLFWwindow;

namespace SW {

	/**
	 * @brief Window specification structure.
	 */
    struct WindowSpecification final
    {
        std::string Title = "No title";  /** @brief Window title. */
        u16 Width = 1280;				 /** @brief Window width. */
        u16 Height = 720;				 /** @brief Window height. */
        bool VSync = false;				 /** @brief Vertical synchronization. */
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
		 * @brief Enable or disable vertical synchronization.
		 * 
		 * @param enabled Whether to enable or disable vertical synchronization.
		 */
        void SetVSync(bool enabled);

    private:
        WindowSpecification m_Specification; /** @brief Window specification. */
        GLFWwindow* m_Handle = nullptr;	 	 /** @brief GLFW window handle. */
    };

}
