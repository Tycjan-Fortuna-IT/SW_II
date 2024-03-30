#include "AssetSourceItem.hpp"

namespace SW {

    const char* AssetSourceItem::GetStringifiedAssetSourceType() const
    {
		switch (Type) {
			case AssetSourceType::Unknown:		return "Unknown";		break;
			case AssetSourceType::Directory:	return "Directory";		break;
			case AssetSourceType::Texture:		return "Texture";		break;
			case AssetSourceType::Sprite:		return "Sprite";		break;
			case AssetSourceType::Spritesheet:  return "Spritesheet";	break;
			case AssetSourceType::Font:			return "Font";			break;
			case AssetSourceType::Scene:		return "Scene";			break;
			case AssetSourceType::Prefab:		return "Prefab";		break;
			case AssetSourceType::Script:		return "Script";		break;
		}

		return "Unknown";
    }

}
