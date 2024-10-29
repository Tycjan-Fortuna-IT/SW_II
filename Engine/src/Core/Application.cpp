#include "Application.hpp"

#include "Asset/AssetManager.hpp"
#include "Core/KeyCode.hpp"
#include "GUI/GuiLayer.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Scripting/ScriptingCore.hpp"
#include "Utils/FileSystem.hpp"
#include "Utils/Input.hpp"
#include <GLFW/glfw3.h>

namespace SW
{

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
	{
		if (!this->OnInit())
			SYSTEM_ERROR("Application failed to initialize!");
	}

	Application::~Application()
	{
		delete m_GuiLayer;
		delete m_Window;
	}

	bool Application::OnInit()
	{
		const WindowSpecification specification = {
		    .Title          = m_Specification.Title,
		    .Width          = m_Specification.Width,
		    .Height         = m_Specification.Height,
		    .VSync          = m_Specification.VSync,
		    .Icon           = m_Specification.Icon,
		    .DisableToolbar = m_Specification.DisableToolbar,
		};

		m_Window = new Window(specification);

		FileSystem::Initialize();
		AssetManager::Initialize();
		RendererAPI::Initialize();

		if (m_Specification.EnableCSharpSupport)
			ScriptingCore::Get().InitializeHost();

		m_WindowCloseEventListener = m_Window->CloseEvent += std::bind_front(&Application::Close, this);

		if (m_Specification.Fullscreen)
			m_Window->Maximize();

		m_IsRunning = true;

		s_Instance = this;

		m_GuiLayer = new GuiLayer();
		m_GuiLayer->OnAttach();

		SYSTEM_INFO("Application has been properly initialized");

		return true;
	}

	bool Application::OnShutdown()
	{
		FileSystem::Shutdown();
		AssetManager::Shutdown();
		RendererAPI::Shutdown();

		if (m_Specification.EnableCSharpSupport)
			ScriptingCore::Get().ShutdownHost();

		m_GuiLayer->OnDetach();

		m_Window->CloseEvent -= m_WindowCloseEventListener;

		SYSTEM_INFO("Application has been properly shut down");

		return true;
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			const float time  = (float)glfwGetTime();
			const Timestep dt = time - m_LastFrameTime;
			m_LastFrameTime   = time;

			Input::UpdateKeysStateIfNecessary();

			m_Window->OnUpdate();

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

			Input::ClearReleasedKeys();
		}

		if (!this->OnShutdown())
			SYSTEM_ERROR("Application failed to shut down!");
	}

	void Application::Close()
	{
		m_IsRunning = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_Layers.emplace_back(layer);

		layer->OnAttach();
	}

	void Application::PopLayer()
	{
		if (m_Layers.empty())
			SYSTEM_FATAL("No Layer to pop!");

		Layer* back = m_Layers.back();
		back->OnDetach();

		m_Layers.pop_back();

		delete back;
	}

} // namespace SW
