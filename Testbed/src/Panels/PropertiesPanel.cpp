#include "PropertiesPanel.hpp"

#include <icons/IconsMaterialDesignIcons.h>

#include "Core/Utils/Utils.hpp"

namespace SW {

	PropertiesPanel::PropertiesPanel(const char* name)
		: Panel(name, String::FromChar8T(ICON_MDI_INFORMATION), true) {}

	void PropertiesPanel::OnUpdate(Timestep dt)
	{

	}

	void PropertiesPanel::OnRender()
	{
		if (OnBegin()) {

			OnEnd();
		}
	}

}
