#include "SceneHierarchyPanel.hpp"

#include <icons/IconsMaterialDesignIcons.h>

#include "Core/Utils/Utils.hpp"

namespace SW {

	SceneHierarchyPanel::SceneHierarchyPanel(const char* name)
		: Panel(name, String::FromChar8T(ICON_MDI_VIEW_LIST), true) {}

	void SceneHierarchyPanel::OnUpdate(Timestep dt)
	{

	}

	void SceneHierarchyPanel::OnRender()
	{
		if (OnBegin()) {

			OnEnd();
		}
	}

}
