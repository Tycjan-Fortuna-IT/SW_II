#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>

#include "Core/Utils/Input.hpp"

namespace SW
{

	void errorCallback(int error, const char* description)
	{
		fprintf(stderr, "GLFW Error %d: %s\n", error, description);
	}

	Window::Window(const WindowSpecification& specification) : m_Specification(specification)
	{
		SYSTEM_INFO("Creating window: {}, width: {} px, height: {} px", m_Specification.Title, m_Specification.Width,
		            m_Specification.Height);

		glfwSetErrorCallback(errorCallback);

		ASSERT(glfwInit(), "Failed to initialize GLFW");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (m_Specification.DisableToolbar)
		{
			glfwWindowHint(GLFW_TITLEBAR, GLFW_FALSE);
			// glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		}

		m_Handle = glfwCreateWindow(m_Specification.Width, m_Specification.Height, m_Specification.Title.c_str(),
		                            nullptr, nullptr);

		ASSERT(m_Handle, "Failed to create GLFW window");

		glfwMakeContextCurrent(m_Handle);

		if (m_Specification.Icon.Data != nullptr)
		{
			GLFWimage icon;
			i32 channels;

			icon.pixels = stbi_load_from_memory(m_Specification.Icon.Data, m_Specification.Icon.Size, &icon.width,
			                                    &icon.height, &channels, 4);

			glfwSetWindowIcon(m_Handle, 1, &icon);

			stbi_image_free(icon.pixels);
		}

		const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		ASSERT(status, "Failed to initialize GLAD");
		ASSERT(GLVersion.major == 4 && GLVersion.minor >= 5, "Engine requires at least OpenGL version 4.6!")

		SYSTEM_WARN("Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		SYSTEM_WARN("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		SYSTEM_WARN("Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		SYSTEM_WARN("GLSL Version: {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
		SYSTEM_WARN("OpenGL Context Initialized!");
		SYSTEM_WARN("OpenGL Version: {}.{}", GLVersion.major, GLVersion.minor);

		glfwSetWindowUserPointer(m_Handle, this);

		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		this->SetVSync(m_Specification.VSync);

		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* glfwWindow) {
			Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);

			window->CloseEvent.Invoke();
		});

		glfwSetKeyCallback(m_Handle, [](GLFWwindow* glfwWindow, i32 key, i32 /*scancode*/, i32 action, i32 /*mods*/) {
			Window* window  = (Window*)glfwGetWindowUserPointer(glfwWindow);
			KeyCode keyCode = (KeyCode)key;

			switch (action)
			{
			case GLFW_RELEASE: {
				Input::UpdateKeyState(keyCode, ClickableState::Released);

				window->KeyReleasedEvent.Invoke(keyCode);

				break;
			}
			case GLFW_PRESS: {
				Input::UpdateKeyState((KeyCode)key, ClickableState::Pressed);

				window->KeyPressedEvent.Invoke(keyCode);

				break;
			}
			case GLFW_REPEAT: {
				Input::UpdateKeyState((KeyCode)key, ClickableState::Repeated);

				window->KeyRepeatEvent.Invoke(keyCode);

				break;
			}
			default:
				ASSERT(false, "Unsupported key event action: {}");
			}
		});

		glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* glfwWindow, i32 button, i32 action, i32 /*mods*/) {
			Window* window      = (Window*)glfwGetWindowUserPointer(glfwWindow);
			MouseCode mouseCode = (MouseCode)button;

			switch (action)
			{
			case GLFW_RELEASE: {
				Input::UpdateMouseState((MouseCode)button, ClickableState::Released);

				window->MouseReleasedEvent.Invoke(mouseCode);

				break;
			}
			case GLFW_PRESS: {
				Input::UpdateMouseState((MouseCode)button, ClickableState::Pressed);

				window->MousePressedEvent.Invoke(mouseCode);

				break;
			}
			default:
				ASSERT(false, "Unsupported mouse event action: {}");
			}
		});

		glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* glfwWindow, i32 width, i32 height) {
			Window* window            = (Window*)glfwGetWindowUserPointer(glfwWindow);
			WindowSpecification& spec = window->GetSpecificationMutable();

			spec.Width  = (u16)width;
			spec.Height = (u16)height;

			window->ResizedEvent.Invoke(width, height);
		});

		glfwSetScrollCallback(m_Handle, [](GLFWwindow* glfwWindow, f64 xOffset, f64 yOffset) {
			Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);

			window->MouseScrollWheelEvent.Invoke((f32)xOffset, (f32)yOffset);
		});

		glfwSetTitlebarHitTestCallback(m_Handle, [](GLFWwindow* window, int /*xPos*/, int /*yPos*/, int* hit) {
			const WindowSpecification& spec = ((Window*)glfwGetWindowUserPointer(window))->GetSpecification();

			*hit = spec.OverTitlebar ? 1 : 0;
		});
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Handle);
	}

	void Window::OnUpdate() const
	{
		glfwPollEvents();

		glfwSwapBuffers(m_Handle);
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Specification.VSync = enabled;
	}

	void Window::RegisterOverTitlebar(bool over)
	{
		m_Specification.OverTitlebar = over;
	}

	void Window::Minimize() const
	{
		glfwIconifyWindow(m_Handle);
	}

	void Window::Maximize() const
	{
		glfwMaximizeWindow(m_Handle);
	}

	void Window::Restore() const
	{
		glfwRestoreWindow(m_Handle);
	}

	bool Window::IsCurrentlyMaximized() const
	{
		return (bool)glfwGetWindowAttrib(m_Handle, GLFW_MAXIMIZED);
	}

} // namespace SW
