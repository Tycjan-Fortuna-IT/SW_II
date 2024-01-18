#include "AssetPanel.hpp"

#include <icons/IconsMaterialDesignIcons.h>

#include "Core/Utils/Utils.hpp"

namespace SW {

	AssetPanel::AssetPanel(const char* name)
		: Panel(name, String::FromChar8T(ICON_MDI_FOLDER_STAR), true) {}

	void AssetPanel::OnUpdate(Timestep dt)
	{

	}

	void AssetPanel::OnRender()
	{
		if (OnBegin()) {

			OnEnd();
		}
	}

}
