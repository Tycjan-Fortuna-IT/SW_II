/**
 * @file FontAsset.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-09-11
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset/Asset.hpp"
#include "OpenGL/Rendering/Font.hpp"
#include "Texture2DAsset.hpp"

namespace SW
{
	struct FontAssetSpecification
	{
		std::filesystem::path Path;

		OpenGL::FontCharsetType Charset = OpenGL::FontCharsetType::ASCII;

		// Pointer to the preloaded atlas texture. (If not passed will be automatically created)
		Texture2DAsset* PreloadedAtlas = nullptr;

		// Flag indicating whether to apply MSDF coloring.
		bool ApplyMSDFColoring = true;

		// Forced width and height of the font atlas.
		int ForceWidth  = 0;
		int ForceHeight = 0;
	};

	class FontAsset : public Asset, public OpenGL::Font
	{
	public:
		FontAsset(const FontAssetSpecification& spec);

		static AssetType GetStaticType() { return AssetType::Font; }
		AssetType GetAssetType() const override { return AssetType::Font; }

		Texture2DAsset* GetTextureAtlasAsset() const { return (Texture2DAsset*)GetAtlasTexture(); }
	};

} // namespace SW
