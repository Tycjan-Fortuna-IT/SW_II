/**
 * @file Application.hpp
 * @author SW
 * @version 0.2.0
 * @date 2024-11-03
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#include "Core/Layer.hpp"
#include "OpenGL/Rendering/Driver.hpp"
#include "OpenGL/Windowing/Device.hpp"
#include "OpenGL/Windowing/Window.hpp"

int main();

namespace SW
{

	class GuiLayer;
	struct Event;

	/**
	 * @brief Application specification used to customize the application. // TODO replace this
	 */
	struct ApplicationSpecification final
	{
		std::string Title         = "No title"; /** @brief Title of the application. */
		u16 Width                 = 1280;       /** @brief Width of the application window. */
		u16 Height                = 720;        /** @brief Height of the application window. */
		bool VSync                = false;      /** @brief Whether the application should use VSync. */
		OpenGL::EmbeddedIcon Icon = {};         /** @brief Window's icon, shown only if toolbar is enabled. */
		bool DisableToolbar       = false;      /** @brief Whether the default window's toolbar should be visible. */
		bool Fullscreen           = false;      /** @brief Whether the window on start should be full screen. */
		bool EnableCSharpSupport  = true;       /** @brief Whether the C# host should be initialized. */
	};

	class Application
	{
	public:
		// Get application instance
		static Application* Get() { return s_Instance; }

		OpenGL::Device* GetDevice() const { return m_Device; }
		OpenGL::Driver* GetDriver() const { return m_Driver; }
		OpenGL::Window* GetWindow() const { return m_Window; }

		// Start the main loop of the application
		void Run();

		// Close the application and performs any necessary cleanup
		void Close();

		// Add a new layer to the layer stack of the application
		// Allowing the layer to be updated and rendered
		void PushLayer(Layer* layer);

		// Pop the top layer from the top of the layer stack
		void PopLayer();

	protected:
		// Only possible once and internally in derived client class.
		Application(const ApplicationSpecification& spec);
		virtual ~Application();

	private:
		Application(const Application& application)            = delete;
		Application(Application&& application)                 = delete;
		Application& operator=(const Application& application) = delete;
		Application& operator=(Application&& application)      = delete;

		static Application* s_Instance; // singleton instance

		ApplicationSpecification m_Specification;

		OpenGL::Device* m_Device = nullptr;
		OpenGL::Driver* m_Driver = nullptr;
		OpenGL::Window* m_Window = nullptr;

		// Time of the last frame.
		float m_LastFrameTime = 0.f;

		// Layers of the application.
		std::vector<Layer*> m_Layers;

		// Gui layer of the application.
		GuiLayer* m_GuiLayer = nullptr;

		friend int ::main();
	};

} // namespace SW
