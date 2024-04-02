#include "AssetSerializer.hpp"

#include "Asset.hpp"
#include "core/Project/ProjectContext.hpp"
#include "Spritesheet.hpp"
#include "Core/Utils/SerializationUtils.hpp"

namespace SW {

	void Texture2DSerializer::Serialize(const AssetMetaData& metadata)
	{
		ASSERT(false, "Texture2D serialization is not supported!");
	}

	Asset* Texture2DSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		Texture2D* texture = new Texture2D(ProjectContext::Get()->GetAssetDirectory() / metadata.Path, true);

		return texture;
	}

	void SpritesheetSerializer::Serialize(const AssetMetaData& metadata)
	{
		const Spritesheet* spritesheet = *AssetManager::GetAsset<Spritesheet>(metadata.Handle);
		
		YAML::Emitter output;

		output << YAML::BeginMap;
		output << YAML::Key << "Spritesheet" << YAML::Value;

		output << YAML::BeginMap;
		Texture2D* texture = spritesheet->GetSpritesheetTexture();
		output << YAML::Key << "TextureHandle" << YAML::Value << (texture ? texture->GetHandle() : 0u);
		output << YAML::Key << "ViewZoom" << YAML::Value << spritesheet->GetViewZoom();
		output << YAML::Key << "GridScale" << YAML::Value << spritesheet->GetGridScale();
		output << YAML::Key << "CenterOffset" << YAML::Value << spritesheet->GetCenterOffset();
		output << YAML::Key << "ViewPos" << YAML::Value << spritesheet->GetViewPos();
		output << YAML::EndMap;

		output << YAML::EndMap;


		std::ofstream fout(ProjectContext::Get()->GetAssetDirectory() / metadata.Path);
		fout << output.c_str();
	}

	Asset* SpritesheetSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		const std::filesystem::path path = ProjectContext::Get()->GetAssetDirectory() / metadata.Path;

		YAML::Node file = YAML::LoadFile(path.string());

		YAML::Node data = file["Spritesheet"];

		if (!data) {
			SW_ERROR("Error while deserializing the spritesheet: {}, no entities section found!", metadata.Path.string());
			
			return new Spritesheet();
		}

		Spritesheet* spritesheet = new Spritesheet();

		const u64 handle = data["TextureHandle"].as<u64>();
		Texture2D** texture = handle ? AssetManager::GetAssetRaw<Texture2D>(handle) : nullptr;
		spritesheet->SetSpritesheetTexture(texture);

		const f32 viewZoom = data["ViewZoom"].as<f32>();
		spritesheet->SetViewZoom(viewZoom);

		const f32 gridScale = data["GridScale"].as<f32>();
		spritesheet->SetGridScale(gridScale);

		const glm::vec2 centerOffset = data["CenterOffset"].as<glm::vec2>();
		spritesheet->SetCenterOffset(centerOffset);

		const glm::vec2 viewPost = data["ViewPos"].as<glm::vec2>();
		spritesheet->SetViewPos(viewPost);

		return spritesheet;
	}

}
