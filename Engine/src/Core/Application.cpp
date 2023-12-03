#include "Application.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Debug/Logger.hpp"

namespace SW {

    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification) {}

    bool Application::OnInit() {
        m_IsRunning = true;

        TRACE("Application has been properly initialized");

        return true;
    }

    bool Application::OnShutdown() {
        delete s_Instance;

        TRACE("Application has been properly shut down");

        return true;
    }

    void Application::OnEvent() {

    }

    void Application::Run() {
        if (!this->OnInit()) {}

        i16 i = 0;
        while(m_IsRunning) {
            if (i++ > 15) break;
            printf("Iteration %i \n", i);
        }

        if (!this->OnShutdown()) {}
    }

    void Application::Close() {
        m_IsRunning = false;
    }
}
