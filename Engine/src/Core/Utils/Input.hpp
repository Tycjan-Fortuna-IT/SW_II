/**
 * @file Input.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-11
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <map>

#include "Core/KeyCode.hpp"
#include "Core/MouseCode.hpp"

namespace SW
{

	/**
	 * @brief The state of a key or mouse button.
	 */
	enum class ClickableState : u8
	{
		Released, /**< The key or mouse button was released. */
		Pressed,  /**< The key or mouse button was pressed. */
		Repeated, /**< The key or mouse button was repeated. */
		None      /**< The key or mouse button was not touched */
	};

	/**
	 * @brief Input class. Provides methods for handling and checking input.
	 *
	 * @warning Check the global GLFW key state! Do not use inside ImGui code, use there ImGui functions for that!
	 */
	class Input final
	{
	public:
		/**
		 * @brief Updates the state of the specified key.
		 * @param code The key to update.
		 * @param state The new state of the key.
		 */
		static void UpdateKeyState(KeyCode code, ClickableState state);

		/**
		 * @brief Updates the state of the specified mouse button.
		 * @param code The mouse button to update.
		 * @param state The new state of the mouse button.
		 */
		static void UpdateMouseState(MouseCode code, ClickableState state);

		/**
		 * @brief Update pressed keys state to held if pressed for one frame.
		 *	      Updates the state faster that GLFW wait after press.
		 *
		 * @warning Be aware that this function should be called at the beginning of the frame.
		 */
		static void UpdateKeysStateIfNecessary();

		/**
		 * @brief Clears the released keys.
		 *
		 * @warning Be aware that this function should be called at the end of the frame.
		 */
		static void ClearReleasedKeys();

		/**
		 * @brief Checks if the specified key is pressed. Only once per frame.
		 * @param key The key to check.
		 * @return true if the key is pressed, false otherwise.
		 */
		static bool IsKeyPressed(KeyCode key);

		/**
		 * @brief Checks if the specified key is held down.
		 * @param key The key to check.
		 * @return true if the key is held down, false otherwise.
		 */
		static bool IsKeyHeld(KeyCode key);

		/**
		 * @brief Checks if the specified key is down. Meaning it's either pressed or held down.
		 * @param key The key to check.
		 * @return true if the key is down, false otherwise.
		 */
		static bool IsKeyDown(KeyCode key);

		/**
		 * @brief Checks if the specified key is released.
		 * @param key The key to check.
		 * @return true if the key is released, false otherwise.
		 */
		static bool IsKeyReleased(KeyCode key);

		/**
		 * @brief Checks if the specified mouse button is pressed.
		 * @param button The mouse button to check.
		 * @return true if the mouse button is pressed, false otherwise.
		 */
		static bool IsMouseButtonPressed(MouseCode button);

		/**
		 * @brief Checks if the specified mouse button is held down.
		 * @param button The mouse button to check.
		 * @return true if the mouse button is held down, false otherwise.
		 */
		static bool IsMouseButtonHeld(MouseCode button);

		/**
		 * @brief Checks if the specified mouse button is down. Meaning it's either pressed or held down.
		 * @param button The mouse button to check.
		 * @return true if the mouse button is down, false otherwise.
		 */
		static bool IsMouseButtonDown(MouseCode button);

		/**
		 * @brief Checks if the specified mouse button is released.
		 * @param button The mouse button to check.
		 * @return true if the mouse button is released, false otherwise.
		 */
		static bool IsMouseButtonReleased(MouseCode button);

		/**
		 * @brief Gets the current position of the mouse.
		 * @return The current position of the mouse.
		 */
		static glm::vec2 GetMousePosition();

		/**
		 * @brief Sets the position of the mouse.
		 * @param position The new position of the mouse.
		 */
		static void SetMousePosition(const glm::vec2& position);

	private:
		static std::map<KeyCode, ClickableState> s_KeyStates; /**< The cached states of the keys. */

		static std::map<MouseCode, ClickableState> s_MouseStates; /**< The cached states of the mouse buttons. */
	};

} // namespace SW
