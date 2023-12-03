#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Debug/Logger.hpp"

namespace SW {
    Window::Window(const WindowSpecification& specification)
        : m_Specification(specification) {
        TRACE("Creating window: %s, width: %i px, height: %i px", m_Specification.Title, m_Specification.Width, m_Specification.Height);

        ASSERT(glfwInit(), "Failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Handle = glfwCreateWindow(
            m_Specification.Width,
            m_Specification.Height,
            m_Specification.Title.c_str(),
            nullptr,
            nullptr
        );

        ASSERT(m_Handle, "Failed to create GLFW window");

        glfwMakeContextCurrent(m_Handle);

        const int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        ASSERT(status, "Failed to initialize GLAD");
        ASSERT(GLVersion.major == 4 && GLVersion.minor >= 6, "Engine requires at least OpenGL version 4.6!")

        INFO("OpenGL Info:");
        INFO("  Vendor: %s", glGetString(GL_VENDOR));
        INFO("  Renderer: %s", glGetString(GL_RENDERER));
        INFO("  Version: %s", glGetString(GL_VERSION));
        INFO("  GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
        INFO("  OpenGL Context Initialized!");
        INFO("  OpenGL Version: %i.%i", GLVersion.major, GLVersion.minor);

        glfwSetWindowUserPointer(m_Handle, this);

        glViewport(0, 0, m_Specification.Width, m_Specification.Height);

        this->SetVSync(m_Specification.VSync);
    }

    Window::~Window() {
        glfwDestroyWindow(m_Handle);
    }

    void Window::OnUpdate() const {
        glfwPollEvents();
        glfwSwapBuffers(m_Handle);
    }

    void Window::SetVSync(bool enabled) {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Specification.VSync = enabled;
    }
}
