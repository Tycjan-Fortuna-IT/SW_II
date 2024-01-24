#include "ConsolePanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "GUI/Icons.hpp"

namespace SW {

	ConsolePanel::ConsolePanel(const char* name)
		: Panel(name, SW_ICON_CONSOLE, true) {}

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
