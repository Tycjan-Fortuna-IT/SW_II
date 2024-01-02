#include "pch.hpp"
#include "Application.hpp"

#include "KeyCode.hpp"
#include "Events/Event.hpp"
#include "GLFW/glfw3.h"

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

        EventSystem::Initialize();

        EventSystem::Register(EventCode::EVENT_CODE_APPLICATION_QUIT, nullptr, [this](Event event, void* sender, void* listener) -> bool {
            Application::Close();

            return true;
        });

        EventSystem::Register(EventCode::EVENT_CODE_KEY_PRESSED, nullptr, [this](Event event, void* sender, void* listener) -> bool {
            TRACE("Key pressed: %i", event.Payload.u16[0]);

            if (event.Payload.u16[0] == KeyCode::Escape) {
                Application::Close();
            }

            return true;
        });

        TRACE("Application has been properly initialized");

        return true;
    }

    bool Application::OnShutdown() {
        EventSystem::Shutdown();

        TRACE("Application has been properly shut down");

        return true;
    }

    void Application::OnEvent() {

    }

    void Application::Run() {
        while(m_IsRunning) {
            const float time = (float)glfwGetTime();
            const float dt = time - m_LastFrameTime;
            m_LastFrameTime = time;

            m_Window->OnUpdate();

            for (Layer* layer : m_Layers) {
                layer->OnUpdate(dt);
            }

            for (Layer* layer : m_Layers) {
                layer->OnRender();
            }
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
        TRACE("POP");

        if (m_Layers.empty())
            SW_FATAL("No Layer to pop!");

        Layer* back = m_Layers.back();
        back->OnDetach();

        m_Layers.pop_back();

        delete back;
    }

}
