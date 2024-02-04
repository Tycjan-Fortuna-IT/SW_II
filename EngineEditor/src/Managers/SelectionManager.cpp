#include "SelectionManager.hpp"

namespace SW {

	u64 SelectionManager::s_SelectedID = NOT_SELECTED;

	void SelectionManager::SelectByID(u64 id)
	{
		if (s_SelectedID != NOT_SELECTED)
			Deselect();

		s_SelectedID = id;
	}

	void SelectionManager::Deselect()
	{
		s_SelectedID = NOT_SELECTED;
	}

}
