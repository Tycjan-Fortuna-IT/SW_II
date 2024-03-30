#pragma once

#include "AssetManagerBase.hpp"

namespace SW {
	
	class EditorAssetManager final : public AssetManagerBase
	{
	public:
		const Asset* GetAsset(AssetHandle handle) override;

		Asset* GetAssetRaw(AssetHandle handle) override;
	};

}
