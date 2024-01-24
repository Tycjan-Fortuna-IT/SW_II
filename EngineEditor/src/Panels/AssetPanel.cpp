#include "AssetPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "GUI/Icons.hpp"

namespace SW {

	AssetPanel::AssetPanel(const char* name)
		: Panel(name, SW_ICON_FOLDER_STAR, true) {}

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
