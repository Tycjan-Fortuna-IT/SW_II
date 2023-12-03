#include "Application.hpp"

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

namespace SW {

    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification) {
        //GLFWwindow* window = glfwCreateWindow(1280, 720, "Title", nullptr, nullptr);
    }

    bool Application::OnInit() {
        m_IsRunning = true;

        return true;
    }

    bool Application::OnShutdown() {
        delete s_Instance;

        return true;
    }

    void Application::OnEvent() {
        
    }

    void Application::OnUpdate() {
        
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
