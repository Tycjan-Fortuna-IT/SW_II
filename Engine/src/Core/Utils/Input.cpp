#include "Input.hpp"

#include <GLFW/glfw3.h>
#include "Core/Application.hpp"

namespace SW {

	bool Input::IsKeyPressed(KeyCode key)
	{
		const i32 state = glfwGetKey(Application::Get()->GetWindow()->GetHandle(), (i32)key);

		return (state == GLFW_PRESS) | (state == GLFW_REPEAT);
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		const i32 state = glfwGetMouseButton(Application::Get()->GetWindow()->GetHandle(), (i32)button);

		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		f64 x, y;
		glfwGetCursorPos(Application::Get()->GetWindow()->GetHandle(), &x, &y);

		return {
			static_cast<f32>(x),
			static_cast<f32>(y)
		};
	}

	void Input::SetMousePosition(const glm::vec2& position)
	{
		GLFWwindow* window = Application::Get()->GetWindow()->GetHandle();
		
		glfwSetCursorPos(window, static_cast<double>(position.x), static_cast<double>(position.y));
	}

}
