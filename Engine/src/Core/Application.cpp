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
        EventSystem::Register(EventCode::EVENT_CODE_APPLICATION_QUIT, nullptr, OnApplicationCloseEvent);

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
        if (!this->OnInit()) {}

        i16 i = 0;
        while(m_IsRunning) {
            m_Window->OnUpdate();
        }

        if (!this->OnShutdown()) {}
    }

    void Application::Close() {
        m_IsRunning = false;
    }

    bool Application::OnApplicationCloseEvent(Event event, void* sender, void* listener) {
        Application::Get()->Close();

        return true;
    }

}
