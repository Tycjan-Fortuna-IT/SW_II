#include "Application.hpp"

#include "Events/Event.hpp"

namespace SW {

    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification) {}

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
        if (!this->OnInit())
            ERROR("Application failed to initialize!");

        i16 i = 0;
        while(m_IsRunning) {
            m_Window->OnUpdate();

            for (Layer* layer : m_Layers) {
                layer->OnUpdate(1);
            }

            for (Layer* layer : m_Layers) {
                layer->OnRender();
            }
        }

        if (!this->OnShutdown())
            ERROR("Application failed to shut down!");
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
            FATAL("No Layer to pop!");

        Layer* back = m_Layers.back();
        back->OnDetach();

        m_Layers.pop_back();

        delete back;
    }

}
