#include "AssetImporter.hpp"
#include "Core/Utils/SerializationUtils.hpp"

#include "Core/Project/ProjectContext.hpp"

namespace SW
{

	void FontAssetImporter::Import(const FontImportData& data)
	{
		YAML::Emitter output;

		output << YAML::BeginMap;

		output << YAML::Key << "Font" << YAML::Value;

		output << YAML::BeginMap;
		output << YAML::Key << "FontSourceHandle" << YAML::Value << data.FontSourceHandle;
		output << YAML::Key << "CharsetType" << YAML::Value << (int)data.CharsetType;
		output << YAML::EndMap;

		output << YAML::EndMap;

		std::ofstream fout(ProjectContext::Get()->GetAssetDirectory() / data.OutputPath);
		fout << output.c_str();
	}

} // namespace SW
