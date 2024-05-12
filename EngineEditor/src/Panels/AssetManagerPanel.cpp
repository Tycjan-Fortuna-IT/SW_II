#include "AssetManagerPanel.hpp"

namespace SW {

	AssetManagerPanel::AssetManagerPanel()
		: Panel("Asset Manager", SW_ICON_FILE, false) {}

	void AssetManagerPanel::OnUpdate(Timestep dt)
	{
 
	}

	void AssetManagerPanel::OnRender()
	{
		if (OnBegin()) {

			OnEnd();
		}
	}

}
