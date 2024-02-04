/**
 * @file SelectionManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-02-04
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <Core/Defines.hpp>

#define NOT_SELECTED 0u

namespace SW {

	/**
	 * @brief The SelectionManager class is responsible for managing the selection of objects.
	 */
	class SelectionManager final
	{
	public:
		/**
		 * @brief Selects an object by its ID.
		 * @param id The ID of the object to be selected.
		 */
		static void SelectByID(u64 id);

		/**
		 * @brief Deselects the currently selected object.
		 */
		static void Deselect();

		/**
		 * @brief Checks if an object is currently selected.
		 * @return True if an object is selected, false otherwise.
		 */
		static bool IsSelected() { return s_SelectedID != NOT_SELECTED; }

		/**
		 * @brief Gets the ID of the currently selected object.
		 * @return The ID of the currently selected object.
		 */
		static u64 GetSelectionID() { return s_SelectedID; }

	private:
		static u64 s_SelectedID; /** @brief The ID of the currently selected object. */
	};

}
