#include "Input.hpp"

#include "Core/Application.hpp"
#include <GLFW/glfw3.h>

namespace SW
{

	std::map<SW::KeyCode, SW::ClickableState> Input::s_KeyStates;

	std::map<SW::MouseCode, SW::ClickableState> Input::s_MouseStates;

	void Input::UpdateKeyState(KeyCode code, ClickableState state)
	{
		s_KeyStates[code] = state;
	}

	void Input::UpdateMouseState(MouseCode code, ClickableState state)
	{
		s_MouseStates[code] = state;
	}

	void Input::UpdateKeysStateIfNecessary()
	{
		for (auto& [code, state] : s_KeyStates)
		{
			if (state == ClickableState::Pressed)
			{
				state = ClickableState::Repeated;
			}
		}

		for (auto& [code, state] : s_MouseStates)
		{
			if (state == ClickableState::Pressed)
			{
				state = ClickableState::Repeated;
			}
		}
	}

	void Input::ClearReleasedKeys()
	{
		for (auto& [code, state] : s_KeyStates)
		{
			if (state == ClickableState::Released)
			{
				state = ClickableState::None;
			}
		}

		for (auto& [code, state] : s_MouseStates)
		{
			if (state == ClickableState::Released)
			{
				state = ClickableState::None;
			}
		}
	}

	bool Input::IsKeyPressed(KeyCode key)
	{
		auto val = s_KeyStates.find(key);

		return val == s_KeyStates.end() ? false : val->second == ClickableState::Pressed;
	}

	bool Input::IsKeyHeld(KeyCode key)
	{
		auto val = s_KeyStates.find(key);

		return val == s_KeyStates.end() ? false : val->second == ClickableState::Repeated;
	}

	bool Input::IsKeyDown(KeyCode key)
	{
		auto val = s_KeyStates.find(key);

		return val == s_KeyStates.end()
		           ? false
		           : (val->second == ClickableState::Pressed || val->second == ClickableState::Repeated);
	}

	bool Input::IsKeyReleased(KeyCode key)
	{
		auto val = s_KeyStates.find(key);

		return val == s_KeyStates.end() ? false : val->second == ClickableState::Released;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto val = s_MouseStates.find(button);

		return val == s_MouseStates.end() ? false : val->second == ClickableState::Pressed;
	}

	bool Input::IsMouseButtonHeld(MouseCode button)
	{
		auto val = s_MouseStates.find(button);

		return val == s_MouseStates.end() ? false : val->second == ClickableState::Repeated;
	}

	bool Input::IsMouseButtonDown(MouseCode button)
	{
		auto val = s_MouseStates.find(button);

		return val == s_MouseStates.end()
		           ? false
		           : (val->second == ClickableState::Pressed || val->second == ClickableState::Repeated);
	}

	bool Input::IsMouseButtonReleased(MouseCode button)
	{
		auto val = s_MouseStates.find(button);

		return val == s_MouseStates.end() ? false : val->second == ClickableState::Released;
	}

	glm::vec2 Input::GetMousePosition()
	{
		f64 x, y;
		glfwGetCursorPos(Application::Get()->GetWindow()->GetHandle(), &x, &y);

		return {static_cast<f32>(x), static_cast<f32>(y)};
	}

	void Input::SetMousePosition(const glm::vec2& position)
	{
		GLFWwindow* window = Application::Get()->GetWindow()->GetHandle();

		glfwSetCursorPos(window, static_cast<double>(position.x), static_cast<double>(position.y));
	}

} // namespace SW
