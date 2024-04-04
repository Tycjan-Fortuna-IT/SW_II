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
		output << YAML::Key << "ViewZoom" << YAML::Value << spritesheet->ViewZoom;
		output << YAML::Key << "GridSize" << YAML::Value << spritesheet->GridSize;
		output << YAML::Key << "CenterOffset" << YAML::Value << spritesheet->CenterOffset;
		output << YAML::Key << "ViewPos" << YAML::Value << spritesheet->ViewPos;
		output << YAML::Key << "ShowImageBorders" << YAML::Value << spritesheet->ShowImageBorders;
		output << YAML::Key << "ExportPath" << YAML::Value << spritesheet->ExportPath.string();

		output << YAML::Key << "Sprites" << YAML::Value << YAML::BeginSeq;

		for (const SpriteData& sprite : spritesheet->Sprites) {
			output << YAML::BeginMap;

			output << YAML::Key << "Sprite";

			output << YAML::BeginMap;
			output << YAML::Key << "Name" << YAML::Value << sprite.Name;
			output << YAML::Key << "Position" << YAML::Value << sprite.Position;
			output << YAML::Key << "Size" << YAML::Value << sprite.Size;
			output << YAML::EndMap;

			output << YAML::EndMap;
		}

		output << YAML::EndMap;
		output << YAML::EndSeq;

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
		spritesheet->ViewZoom = data["ViewZoom"].as<f32>();
		spritesheet->GridSize = data["GridSize"].as<f32>();
		spritesheet->CenterOffset = data["CenterOffset"].as<glm::vec2>();
		spritesheet->ViewPos = data["ViewPos"].as<glm::vec2>();
		spritesheet->ShowImageBorders = data["ShowImageBorders"].as<bool>();
		spritesheet->ExportPath = data["ExportPath"].as<std::string>();

		YAML::Node sprites = data["Sprites"];
		for (YAML::Node sprite : sprites) {
			std::string name = sprite["Sprite"]["Name"].as<std::string>();

			SpriteData spriteData(name);
			spriteData.Position = sprite["Sprite"]["Position"].as<glm::vec2>();
			spriteData.Size = sprite["Sprite"]["Size"].as<glm::vec2>();

			spritesheet->Sprites.emplace_back(spriteData);
		}

		return spritesheet;
	}

}
