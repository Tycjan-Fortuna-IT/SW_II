/**
 * @file Input.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/KeyCode.hpp"
#include "Core/MouseCode.hpp"

namespace SW {

	/**
	 * @brief Input class. Provides methods for handling and checking input.
	 */
	class Input final
	{
	public:
		/**
		 * @brief Checks if key is pressed.
		 * 
		 * @param key Key to check.
		 * @return bool If key is pressed.
		 */
		static bool IsKeyPressed(KeyCode key);

		/**
		 * @brief Checks if mouse button is pressed.
		 * 
		 * @param button Mouse button to check.
		 * @return bool If mouse button is pressed.
		 */
		static bool IsMouseButtonPressed(MouseCode button);

		/**
		 * @brief Gets mouse position.
		 * 
		 * @return std::pair<f32, f32> Mouse position.
		 */
		static std::pair<f32, f32> GetMousePosition();
	};

}
