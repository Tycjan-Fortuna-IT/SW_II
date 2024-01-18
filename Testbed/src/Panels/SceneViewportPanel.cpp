#include "SceneViewportPanel.hpp"

#include <icons/IconsMaterialDesignIcons.h>

#include "Core/Utils/Utils.hpp"

namespace SW {

	SceneViewportPanel::SceneViewportPanel(const char* name)
		: Panel(name, String::FromChar8T(ICON_MDI_TERRAIN), true) {}

	void SceneViewportPanel::OnUpdate(Timestep dt)
	{

	}

	void SceneViewportPanel::OnRender()
	{
		if (OnBegin()) {

			OnEnd();
		}
	}

}
