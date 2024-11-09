/**
 * @file FontAssetImporter.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset.hpp"
#include "Asset/FontAsset.hpp"

namespace SW
{

	struct FontImportData
	{
		AssetHandle FontSourceHandle        = 0u;
		OpenGL::FontCharsetType CharsetType = OpenGL::FontCharsetType::ASCII;
		std::filesystem::path OutputPath;

		explicit operator bool() { return FontSourceHandle && !OutputPath.empty(); }
	};

	class FontAssetImporter final
	{
	public:
		/**
		 * @brief Import a font asset.
		 * @note Used to import a font asset from a font source file.
		 *
		 * @param data Font import data.
		 */
		static void Import(const FontImportData& data);
	};

} // namespace SW
