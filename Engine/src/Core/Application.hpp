/**
 * @file Application.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.2
 * @date 2024-01-09
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Layer.hpp"
#include "Core/OpenGL/Window.hpp"
#include "OpenGL/Texture2D.hpp"

int main();

namespace SW
{

	class GuiLayer;
	struct Event;

	/**
	 * @brief Application specification used to customize the application.
	 */
	struct ApplicationSpecification final
	{
		std::string Title        = "No title"; /** @brief Title of the application. */
		u16 Width                = 1280;       /** @brief Width of the application window. */
		u16 Height               = 720;        /** @brief Height of the application window. */
		bool VSync               = false;      /** @brief Whether the application should use VSync. */
		EmbeddedIcon Icon        = {};         /** @brief Window's icon, shown only if toolbar is enabled. */
		bool DisableToolbar      = false;      /** @brief Whether the default window's toolbar should be visible. */
		bool Fullscreen          = false;      /** @brief Whether the window on start should be full screen. */
		bool EnableCSharpSupport = true;       /** @brief Whether the C# host should be initialized. */
	};

	/**
	 * @brief Main application class. (Singleton)
	 */
	class Application
	{
	public:
		/**
		 * @brief Get the Application object.
		 *
		 * @return Application*
		 */
		static Application* Get() { return s_Instance; }

		/**
		 * @brief Get the Window object
		 *
		 * @return const Window*
		 */
		Window* GetWindow() const { return m_Window; }

		/**
		 * @brief Initialize the application
		 *
		 * This function is called when the application starts. It should be used to perform any necessary
		 * initialization.
		 *
		 * @return true if initialization was successful, false otherwise.
		 */
		bool OnInit();

		/**
		 * @brief Shutdown the application
		 *
		 * This function is called when the application is about to close. It should be used to perform any necessary
		 * cleanup.
		 *
		 * @return true if shutdown was successful, false otherwise.
		 */
		bool OnShutdown();

		/**
		 * @brief Run the application
		 *
		 * This function starts the main loop of the application.
		 */
		void Run();

		/**
		 * @brief Close the application
		 *
		 * This function closes the application and performs any necessary cleanup.
		 */
		void Close();

		/**
		 * @brief Push a new layer onto the layer stack
		 *
		 * This function adds a new layer to the layer stack of the application.
		 * Allowing the layer to be updated and rendered.
		 *
		 * @param layer The layer to add.
		 */
		void PushLayer(Layer* layer);

		/**
		 * @brief Pop the top layer from the layer stack
		 *
		 * This function removes the top layer from the layer stack of the application.
		 */
		void PopLayer();

		Ref<Texture2D> s;

	protected:
		/**
		 * @brief Construct a new Application.
		 * @warning Only possible once and internally in derived client class.
		 *
		 * @param specification
		 */
		Application(const ApplicationSpecification& specification);

		/**
		 * @brief Destroy the Application object
		 */
		virtual ~Application();

	private:
		/** @brief Copy constructor - not allowed. */
		Application(const Application& application) = delete;
		/** @brief Move constructor - not allowed. */
		Application(Application&& application) = delete;
		/** @brief Copy assignment operator - not allowed. */
		Application& operator=(const Application& application) = delete;
		/** @brief Move assignment operator - not allowed. */
		Application& operator=(Application&& application) = delete;

		static Application* s_Instance; /** @brief Singleton instance of the application. */

		ApplicationSpecification m_Specification; /** @brief Specification of the application. */
		Window* m_Window = nullptr;               /** @brief Window of the application. */

		bool m_IsRunning      = false; /** @brief Whether the application is running. */
		float m_LastFrameTime = 0.f;   /** @brief Time of the last frame. */

		std::vector<Layer*> m_Layers; /** @brief Layers of the application. */

		GuiLayer* m_GuiLayer = nullptr; /** @brief Gui layer of the application. */

		friend int ::main();

	private:
		Eventing::ListenerID m_WindowCloseEventListener;
	};

} // namespace SW
