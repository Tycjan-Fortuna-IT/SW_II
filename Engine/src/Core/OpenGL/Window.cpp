#include "Window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>

#include "Core/Events/Event.hpp"
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

		glfwSetWindowUserPointer(m_Handle, &m_Specification);

		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		this->SetVSync(m_Specification.VSync);

		glfwSetWindowCloseCallback(m_Handle, [](UNUSED GLFWwindow* window) {
			EventSystem::Emit({.Code = EVENT_CODE_APPLICATION_QUIT});
		});

		glfwSetKeyCallback(m_Handle,
		                   [](UNUSED GLFWwindow* window, i32 key, UNUSED i32 scancode, i32 action, UNUSED i32 mods) {
			                   switch (action)
			                   {
			                   case GLFW_RELEASE: {
				                   Input::UpdateKeyState((KeyCode)key, ClickableState::Released);

				                   EventSystem::Emit({.Code = EVENT_CODE_KEY_RELEASED, .Payload = {.u16 = {(u16)key}}});

				                   break;
			                   }
			                   case GLFW_PRESS: {
				                   Input::UpdateKeyState((KeyCode)key, ClickableState::Pressed);

				                   EventSystem::Emit({.Code = EVENT_CODE_KEY_PRESSED, .Payload = {.u16 = {(u16)key}}});

				                   break;
			                   }
			                   case GLFW_REPEAT: {
				                   Input::UpdateKeyState((KeyCode)key, ClickableState::Repeated);

				                   EventSystem::Emit({.Code = EVENT_CODE_KEY_REPEAT, .Payload = {.u16 = {(u16)key}}});

				                   break;
			                   }
			                   default:
				                   SYSTEM_WARN("Unsupported key event action: {}");
			                   }
		                   });

		glfwSetMouseButtonCallback(m_Handle, [](UNUSED GLFWwindow* window, int button, int action, UNUSED int mods) {
			switch (action)
			{
			case GLFW_RELEASE: {
				Input::UpdateMouseState((MouseCode)button, ClickableState::Released);

				EventSystem::Emit({.Code = EVENT_CODE_MOUSE_BUTTON_RELEASED, .Payload = {.u16 = {(u16)button}}});

				break;
			}
			case GLFW_PRESS: {
				Input::UpdateMouseState((MouseCode)button, ClickableState::Pressed);

				EventSystem::Emit({.Code = EVENT_CODE_MOUSE_BUTTON_PRESSED, .Payload = {.u16 = {(u16)button}}});

				break;
			}
			default:
				SYSTEM_WARN("Unsupported mouse event action: {}");
			}
		});

		glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* window, i32 width, i32 height) {
			WindowSpecification* spec = (WindowSpecification*)glfwGetWindowUserPointer(window);

			spec->Width  = (u16)width;
			spec->Height = (u16)height;

			EventSystem::Emit({.Code = EVENT_CODE_WINDOW_RESIZED, .Payload = {.i32 = {width, height}}});
		});

		glfwSetScrollCallback(m_Handle, [](UNUSED GLFWwindow* window, f64 xOffset, f64 yOffset) {
			EventSystem::Emit({.Code = EVENT_CODE_MOUSE_WHEEL, .Payload = {.f32 = {(f32)xOffset, (f32)yOffset}}});
		});

		glfwSetTitlebarHitTestCallback(m_Handle, [](GLFWwindow* window, UNUSED int xPos, UNUSED int yPos, int* hit) {
			*hit = static_cast<WindowSpecification*>(glfwGetWindowUserPointer(window))->OverTitlebar ? 1 : 0;
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
