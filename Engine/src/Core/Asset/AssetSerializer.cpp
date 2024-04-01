#include "AssetSerializer.hpp"

#include "Asset.hpp"
#include "core/Project/ProjectContext.hpp"
#include "Spritesheet.hpp"

namespace SW {

	void Texture2DSerializer::Serialize(const Asset* asset)
	{
		ASSERT(false, "Texture2D serialization is not supported!");
	}

	Asset* Texture2DSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		Asset* texture = new Texture2D(ProjectContext::Get()->GetAssetDirectory() / metadata.Path, true);

		return texture;
	}

	void SpritesheetSerializer::Serialize(const Asset* asset)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Asset* SpritesheetSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		Asset* spritesheet = new Spritesheet();

		return spritesheet;
	}

}
