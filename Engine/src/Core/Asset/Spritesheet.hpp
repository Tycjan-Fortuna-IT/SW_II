#pragma once

#include "Asset.hpp"
#include "Core/OpenGL/Texture2D.hpp"

namespace SW {

	class Spritesheet final : public Asset
	{
	public:
		Spritesheet() = default;
		Spritesheet(Texture2D* spritesheetTexture)
			: m_SpritesheetTexture(spritesheetTexture) {}

		AssetType GetAssetType() const override { return AssetType::Spritesheet; }

		Texture2D* GetSpritesheetTexture() const { return m_SpritesheetTexture; }

	private:
		Texture2D* m_SpritesheetTexture = nullptr;
	};

}
