#include "Application.hpp"

#include <GLFW/glfw3.h>
#include "Core/KeyCode.hpp"
#include "Events/Event.hpp"
#include "GUI/GuiLayer.hpp"

namespace SW {

    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification) {
        if (!this->OnInit())
            SW_ERROR("Application failed to initialize!");
    }

    bool Application::OnInit() {
        const WindowSpecification specification = {
            .Title = m_Specification.Title,
            .Width = m_Specification.Width,
            .Height = m_Specification.Height,
            .VSync = m_Specification.VSync
        };

        m_Window = new Window(specification);

        m_IsRunning = true;

        s_Instance = this;

		m_GuiLayer = new GuiLayer();

        EventSystem::Initialize();

        EventSystem::Register(EventCode::EVENT_CODE_APPLICATION_QUIT, nullptr, [this](Event event, void* sender, void* listener) -> bool {
            Application::Close();

            return true;
        });

        EventSystem::Register(EventCode::EVENT_CODE_KEY_PRESSED, nullptr, [this](Event event, void* sender, void* listener) -> bool {
            if (event.Payload.u16[0] == KeyCode::Escape) {
                Application::Close();
            }

            return false;
        });

		SW_INFO("Application has been properly initialized");

		return true;
	}

	bool Application::OnShutdown() {
		EventSystem::Shutdown();

		delete m_Window;
		delete m_GuiLayer;

		SW_INFO("Application has been properly shut down");

		return true;
	}

	void Application::OnEvent() {

	}

	void Application::Run() {
		while (m_IsRunning) {
			const float time = (float)glfwGetTime();
			const float dt = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_Window->OnUpdate();

			for (Layer* layer : m_Layers) {
				layer->OnUpdate(dt);
			}

			m_GuiLayer->Begin();
			for (Layer* layer : m_Layers) {
				layer->OnRender();
			}
			m_GuiLayer->End();
		}

		if (!this->OnShutdown())
			SW_ERROR("Application failed to shut down!");
	}

	void Application::Close() {
		m_IsRunning = false;
	}

	void Application::PushLayer(Layer* layer) {
		m_Layers.emplace_back(layer);

		layer->OnAttach();
	}

	void Application::PopLayer() {
        if (m_Layers.empty())
            SW_FATAL("No Layer to pop!");

        Layer* back = m_Layers.back();
        back->OnDetach();

        m_Layers.pop_back();

        delete back;
    }

}
