/**
 * @file SelectionManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-19
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <Core/Defines.hpp>

#define NOT_SELECTED 0u

namespace SW {

	class SelectionManager final
	{
	public:
		static void Select(u64 id);
		static void Deselect();

		static bool IsSelected() { return s_SelectedID != NOT_SELECTED; }
		static u64 GetSelectionID() { return s_SelectedID; }

	private:
		static u64 s_SelectedID;
	};

}
