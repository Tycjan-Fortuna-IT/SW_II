#include "AssetSerializer.hpp"

#include "Asset.hpp"
#include "core/Project/ProjectContext.hpp"
#include "Spritesheet.hpp"
#include "Core/Utils/SerializationUtils.hpp"
#include "GUI/Editor/EditorResources.hpp"
#include "Asset/Font.hpp"
#include "Cache/FontCache.hpp"
#include "Animation2D.hpp"

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

	void SpriteSerializer::Serialize(const AssetMetaData& metadata)
	{
		ASSERT(false, "Sprite serialization is not supported!");
	}

	Asset* SpriteSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		const std::filesystem::path path = ProjectContext::Get()->GetAssetDirectory() / metadata.Path;

		YAML::Node file = YAML::LoadFile(path.string());

		YAML::Node data = file["Sprite"];

		if (!data) {
			SW_ERROR("Error while deserializing the sprite: {}", metadata.Path.string());

			return new Sprite();
		}

		Sprite* sprite = new Sprite();

		const u64 handle = data["SpritesheetTextureHandle"].as<u64>();
		const bool isValid = AssetManager::IsValid(handle);
		Texture2D** texture = isValid ? AssetManager::GetAssetRaw<Texture2D>(handle) : &EditorResources::MissingAssetIcon;

		sprite->SetTexture(texture);
		sprite->TexCordLeftDown = isValid ? data["TexCordLeftDown"].as<glm::vec2>() : glm::vec2(0.0f, 0.0f);
		sprite->TexCordRightDown = isValid ? data["TexCordRightDown"].as<glm::vec2>() : glm::vec2(1.0f, 0.0f);
		sprite->TexCordUpRight = isValid ? data["TexCordUpRight"].as<glm::vec2>() : glm::vec2(1.0f, 1.0f);
		sprite->TexCordUpLeft = isValid ? data["TexCordUpLeft"].as<glm::vec2>() : glm::vec2(0.0f, 1.0f);

		return sprite;
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
			SW_ERROR("Error while deserializing the spritesheet: {}", metadata.Path.string());
			
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

	void FontSerializer::Serialize(const AssetMetaData& metadata)
	{
		ASSERT(false, "Font serialization is not supported!");
	}

	Asset* FontSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		const std::filesystem::path path = ProjectContext::Get()->GetAssetDirectory() / metadata.Path;

		YAML::Node file = YAML::LoadFile(path.string());

		YAML::Node data = file["Font"];

		if (!data) {
			SW_ERROR("Error while deserializing the font: {}", metadata.Path.string());

			return nullptr;
		}

		const u64 fontSourceHandle = data["FontSourceHandle"].as<u64>();

		const AssetMetaData& sourceMetadata = AssetManager::GetAssetMetaData(fontSourceHandle);

		FontSpecification spec;
		spec.Path = ProjectContext::Get()->GetAssetDirectory() / sourceMetadata.Path;
		spec.Charset = (FontCharsetType)data["CharsetType"].as<int>();;
		spec.PreloadedAtlas = FontCache::TryGetCachedAtlas(metadata.Handle, metadata.ModificationTime);

		Font* font = new Font(spec);

		if (!spec.PreloadedAtlas)
			FontCache::CacheAtlas(font->GetAtlasTexture(), metadata.Handle, metadata.ModificationTime);

		return font;
	}

	void FontSourceSerializer::Serialize(const AssetMetaData& metadata)
	{

	}

	Asset* FontSourceSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		return nullptr;
	}

    void AnimationSerializer::Serialize(const AssetMetaData& metadata)
	{
		const Animation2D* animation = *AssetManager::GetAsset<Animation2D>(metadata.Handle);

		YAML::Emitter output;

		output << YAML::BeginMap;
		output << YAML::Key << "Animation" << YAML::Value;

		output << YAML::BeginMap;
		output << YAML::Key << "Speed" << YAML::Value << animation->Speed;
		output << YAML::Key << "ReverseAlongX" << YAML::Value << animation->ReverseAlongX;
		output << YAML::Key << "ReverseAlongY" << YAML::Value << animation->ReverseAlongY;

		output << YAML::Key << "Sprites" << YAML::Value << YAML::BeginSeq;

		for (Sprite** const sprite : animation->Sprites) {
			output << YAML::BeginMap;

			output << YAML::Key << "Sprite";

			output << YAML::BeginMap;
			output << YAML::Key << "SpriteHandle" << YAML::Value << (*sprite)->GetHandle();
			output << YAML::EndMap;

			output << YAML::EndMap;
		}

		output << YAML::EndMap;
		output << YAML::EndSeq;

		output << YAML::EndMap;


		std::ofstream fout(ProjectContext::Get()->GetAssetDirectory() / metadata.Path);
		fout << output.c_str();
	}

	Asset* AnimationSerializer::TryLoadAsset(const AssetMetaData& metadata)
    {
		const std::filesystem::path path = ProjectContext::Get()->GetAssetDirectory() / metadata.Path;

		YAML::Node file = YAML::LoadFile(path.string());

		YAML::Node data = file["Animation"];

		if (!data) {
			SW_ERROR("Error while deserializing the animation: {}", metadata.Path.string());

			return new Animation2D();
		}

		Animation2D* animation = new Animation2D();

		animation->Speed = data["Speed"].as<f32>();
		animation->ReverseAlongX = data["ReverseAlongX"].as<bool>();
		animation->ReverseAlongY = data["ReverseAlongY"].as<bool>();

		YAML::Node sprites = data["Sprites"];
		for (YAML::Node sprite : sprites) {
			Sprite** spr = AssetManager::GetAssetRaw<Sprite>(sprite["Sprite"]["SpriteHandle"].as<u64>());

			animation->Sprites.emplace_back(spr);
		}

		return animation;
    }

}
