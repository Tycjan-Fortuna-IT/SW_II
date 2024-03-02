/**
 * @file Font.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-02
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#undef INFINITE
#include <msdf-atlas-gen/msdf-atlas-gen.h>

namespace SW {

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
	 * @brief Class representing a font used for rendering text.
	 */
	class Font final
	{
	public:
		/**
		 * @brief Constructs a Font object from the specified font file path.
		 * @param filepath The path to the font file.
		 */
		Font(const std::filesystem::path& filepath);

		/**
		 * @brief Destructor for the Font object.
		 */
		~Font();

		/**
		 * @brief Gets the MSDF data associated with the font.
		 * @return A pointer to the MSDFData structure.
		 */
		const MSDFData* GetMSDFData() const { return m_Data; }

		/**
		 * @brief Gets the atlas texture associated with the font.
		 * @return A pointer to the Texture2D object representing the atlas texture.
		 */
		Texture2D* GetAtlasTexture() const { return m_AtlasTexture; }

		/**
		 * @brief Gets the filename of the font file.
		 * @return The filename of the font file.
		 */
		const std::string& GetFilename() const { return m_Filename; }

		/**
		 * @brief Gets the filepath of the font file.
		 * @return The filepath of the font file.
		 */
		const std::string& GetPath() const { return m_Filepath; }

	private:
		MSDFData* m_Data;			/**< Pointer to the MSDFData structure. */
		Texture2D* m_AtlasTexture;  /**< Pointer to the atlas texture. */

		std::string m_Filename;		/**< File name from which the texture was loaded. */
		std::string m_Filepath;		/**< Filepath from which the texture was loaded. */
	};

}
