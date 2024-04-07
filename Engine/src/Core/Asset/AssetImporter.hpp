#pragma once

#include "Asset.hpp"
#include "Font.hpp"

namespace SW {

	struct FontImportData
	{
		AssetHandle FontSourceHandle = 0u;
		FontCharsetType CharsetType = FontCharsetType::ASCII;
		std::filesystem::path OutputPath;

		explicit operator bool() { return FontSourceHandle && !OutputPath.empty(); }
	};

	class FontAssetImporter final
	{
	public:
		static void Import(const FontImportData& data);
	};

}

