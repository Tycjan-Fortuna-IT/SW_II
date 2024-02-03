/**
 * @file EditorSettings.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	/**
	 * @brief The EditorSettings struct contains settings for the editor.
	 */
	struct EditorSettings final {
		bool VSync = true; /**< Whether to enable VSync. */
		bool ShowPhysicsColliders = false; /**< Whether to show physics colliders. */

		/**
		 * @brief Get the instance of the EditorSettings.
		 * @return The instance of the EditorSettings.
		 */
		static EditorSettings& Get();
	};

}
