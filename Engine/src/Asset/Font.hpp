/**
 * @file Font.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.2.0
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#undef INFINITE
#include <msdf-atlas-gen/msdf-atlas-gen.h>

#include "Asset/Asset.hpp"

namespace SW
{

	class Texture2D;

	/**
	 * @brief Struct representing the data required for Multi-channel Signed Distance Field (MSDF) rendering.
	 */
	struct MSDFData final
	{
		std::vector<msdf_atlas::GlyphGeometry> Glyphs; /**< Vector of glyph geometries. */

		msdf_atlas::FontGeometry FontGeometry; /**< Font geometry data. */
	};

	/**
	 * @brief Enum class representing the character set type for the font.
	 */
	enum class FontCharsetType
	{
		ASCII = 0, /**< ASCII character set. */
		ALL,       /**< All characters. */
	};

	/**
	 * @brief Struct representing the specification of the font.
	 */
	struct FontSpecification
	{
		std::filesystem::path Path;                       /**< Path to the font file. */
		FontCharsetType Charset = FontCharsetType::ASCII; /**< Character set type. */
		Texture2D* PreloadedAtlas =
		    nullptr; /**< Pointer to the preloaded atlas texture. (If not passed will be automaticaly created) */
		bool ApplyMSDFColoring = true; /**< Flag indicating whether to apply MSDF coloring. */
		int ForceWidth         = 0;    /**< Forced width of the font atlas. */
		int ForceHeight        = 0;    /**< Forced height of the font atlas. */
	};

	/**
	 * @brief Class representing a font used for rendering text.
	 */
	class Font final : public Asset
	{
	public:
		/**
		 * @brief Constructor for the Font object.
		 * @param spec FontSpecification structure.
		 */
		Font(const FontSpecification& spec);

		/**
		 * @brief Destructor for the Font object.
		 */
		~Font();

		/**
		 * @brief Gets the static type of the asset.
		 * @return AssetType::Font.
		 */
		static AssetType GetStaticType() { return AssetType::Font; }

		/**
		 * @brief Gets the type of the asset.
		 * @return AssetType::Font.
		 */
		AssetType GetAssetType() const override { return AssetType::Font; }

		/**
		 * @brief Gets the MSDF data associated with the font.
		 * @return A pointer to the MSDFData structure.
		 */
		const MSDFData& GetMSDFData() const { return m_Data; }

		/**
		 * @brief Gets the atlas texture associated with the font.
		 * @return A pointer to the Texture2D object representing the atlas texture.
		 */
		Texture2D* GetAtlasTexture() const { return m_AtlasTexture; }

	private:
		MSDFData m_Data;                     /**< MSDFData structure. */
		Texture2D* m_AtlasTexture = nullptr; /**< Pointer to the atlas texture. */
	};

} // namespace SW
