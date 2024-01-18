#include "ConsolePanel.hpp"

#include <icons/IconsMaterialDesignIcons.h>

#include "Core/Utils/Utils.hpp"

namespace SW {

	ConsolePanel::ConsolePanel(const char* name)
		: Panel(name, String::FromChar8T(ICON_MDI_CONSOLE), true) {}

	void ConsolePanel::OnUpdate(Timestep dt)
	{

	}

	void ConsolePanel::OnRender()
	{
		if (OnBegin()) {

			OnEnd();
		}
	}

}
