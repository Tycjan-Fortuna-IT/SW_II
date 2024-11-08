#include "Application.hpp"

#include "Asset/AssetManager.hpp"
#include "GUI/GuiLayer.hpp"
#include "Scripting/ScriptingCore.hpp"
#include "Utils/FileSystem.hpp"
#include <GLFW/glfw3.h>

namespace SW
{

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& spec) : m_Specification(spec)
	{
		const OpenGL::WindowSpecification windowSpec = {
		    .Title       = m_Specification.Title,
		    .Width       = m_Specification.Width,
		    .Height      = m_Specification.Height,
		    .Icon        = m_Specification.Icon,
		    .HasTitlebar = !m_Specification.DisableToolbar,
		};

		const OpenGL::DeviceSpecification deviceSpec = {.DebugProfile = true};

		m_Device = new OpenGL::Device(deviceSpec);
		m_Device->SetVSync(m_Specification.VSync);

		m_Window = new OpenGL::Window(m_Device, windowSpec);
		m_Window->MakeContextCurrent();

		m_InputManager = new InputManager(m_Window);

		const OpenGL::DriverSpecification driverSpec = {.DebugProfile = true};

		m_Driver = new OpenGL::Driver(driverSpec);

		FileSystem::Initialize();
		AssetManager::Initialize();

		if (m_Specification.EnableCSharpSupport)
			ScriptingCore::Get().InitializeHost();

		if (m_Specification.Fullscreen)
			m_Window->Maximize();

		s_Instance = this;

		m_GuiLayer = new GuiLayer();
		m_GuiLayer->OnAttach();

		SYSTEM_INFO("Application has been properly initialized");
	}

	Application::~Application()
	{
		FileSystem::Shutdown();
		AssetManager::Shutdown();

		if (m_Specification.EnableCSharpSupport)
			ScriptingCore::Get().ShutdownHost();

		m_GuiLayer->OnDetach();

		delete m_GuiLayer;

		delete m_InputManager;
		delete m_Driver;
		delete m_Window;
		delete m_Device;

		SYSTEM_INFO("Application has been properly shut down");
	}

	void Application::Run()
	{
		while (!m_Window->ShouldClose())
		{
			const float time  = (float)glfwGetTime();
			const Timestep dt = time - m_LastFrameTime;
			m_LastFrameTime   = time;

			m_InputManager->UpdateKeysStateIfNecessary();

			m_Device->PollEvents();
			m_Window->SwapBuffers();

			{
				PROFILE_SCOPE("Application::Update()");

				for (Layer* layer : m_Layers)
				{
					layer->OnUpdate(dt);
				}
			}

			{
				PROFILE_SCOPE("Application::Render()");

				m_GuiLayer->Begin();
				for (Layer* layer : m_Layers)
				{
					layer->OnRender();
				}
				m_GuiLayer->End();
			}

			m_InputManager->ClearReleasedKeys();
		}
	}

	void Application::Close()
	{
		m_Window->SetShouldClose(true);
	}

	void Application::PushLayer(Layer* layer)
	{
		m_Layers.emplace_back(layer);

		layer->OnAttach();
	}

	void Application::PopLayer()
	{
		VERIFY(!m_Layers.empty(), "No Layer to pop!");

		Layer* back = m_Layers.back();
		back->OnDetach();

		m_Layers.pop_back();

		delete back;
	}

} // namespace SW
