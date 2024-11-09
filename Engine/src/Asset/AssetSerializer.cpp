#include "AssetSerializer.hpp"

#include "Animation2D.hpp"
#include "Asset.hpp"
#include "Asset/FontAsset.hpp"
#include "Audio/Sound.hpp"
#include "Cache/FontCache.hpp"
#include "Core/Scene/Scene.hpp"
#include "Core/Scene/SceneSerializer.hpp"
#include "Core/Utils/SerializationUtils.hpp"
#include "GUI/Editor/EditorResources.hpp"
#include "OpenGL/Rendering/Font.hpp"
#include "OpenGL/Rendering/Texture2D.hpp"
#include "Prefab.hpp"
#include "Spritesheet.hpp"
#include "core/Project/ProjectContext.hpp"

namespace SW
{

	void SceneAssetSerializer::Serialize(const AssetMetaData& metadata)
	{
		Scene* scene = *AssetManager::GetAssetRaw<Scene>(metadata.Handle);

		SceneSerializer::Serialize(scene, ProjectContext::Get()->GetAssetDirectory() / metadata.Path);
	}

	Asset* SceneAssetSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		return SceneSerializer::Deserialize(ProjectContext::Get()->GetAssetDirectory() / metadata.Path);
	}

	void Texture2DSerializer::Serialize(const AssetMetaData& /*metadata*/)
	{
		ASSERT(false, "Texture2D serialization is not supported!");
	}

	Asset* Texture2DSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		Texture2DAsset* texture = new Texture2DAsset(ProjectContext::Get()->GetAssetDirectory() / metadata.Path, true);

		return texture;
	}

	void SpriteSerializer::Serialize(const AssetMetaData& /*metadata*/)
	{
		ASSERT(false, "Sprite serialization is not supported!");
	}

	Asset* SpriteSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		const std::filesystem::path path = ProjectContext::Get()->GetAssetDirectory() / metadata.Path;

		YAML::Node file = YAML::LoadFile(path.string());

		YAML::Node data = file["Sprite"];

		if (!data)
		{
			SYSTEM_ERROR("Error while deserializing the sprite: {}", metadata.Path.string());

			return new Sprite();
		}

		Sprite* sprite = new Sprite();

		const u64 handle   = TryDeserializeNode<u64>(data, "SpritesheetTextureHandle", 0);
		const bool isValid = AssetManager::IsValid(handle);
		Texture2DAsset** texture =
		    isValid ? AssetManager::GetAssetRaw<Texture2DAsset>(handle) : &EditorResources::MissingAssetIcon;

		sprite->SetTexture(texture);
		sprite->TexCordLeftDown  = isValid
		                               ? TryDeserializeNode<glm::vec2>(data, "TexCordLeftDown", glm::vec2(0.0f, 0.0f))
		                               : glm::vec2(0.0f, 0.0f);
		sprite->TexCordRightDown = isValid
		                               ? TryDeserializeNode<glm::vec2>(data, "TexCordRightDown", glm::vec2(1.0f, 0.0f))
		                               : glm::vec2(1.0f, 0.0f);
		sprite->TexCordUpRight = isValid ? TryDeserializeNode<glm::vec2>(data, "TexCordUpRight", glm::vec2(1.0f, 1.0f))
		                                 : glm::vec2(1.0f, 1.0f);
		sprite->TexCordUpLeft  = isValid ? TryDeserializeNode<glm::vec2>(data, "TexCordUpLeft", glm::vec2(0.0f, 1.0f))
		                                 : glm::vec2(0.0f, 1.0f);

		return sprite;
	}

	void SpritesheetSerializer::Serialize(const AssetMetaData& metadata)
	{
		const Spritesheet* spritesheet = *AssetManager::GetAsset<Spritesheet>(metadata.Handle);

		YAML::Emitter output;

		output << YAML::BeginMap;
		output << YAML::Key << "Spritesheet" << YAML::Value;

		output << YAML::BeginMap;
		Texture2DAsset* texture = spritesheet->GetSpritesheetTexture();
		output << YAML::Key << "TextureHandle" << YAML::Value << (texture ? texture->Asset::GetHandle() : 0u);
		output << YAML::Key << "ViewZoom" << YAML::Value << spritesheet->ViewZoom;
		output << YAML::Key << "GridSize" << YAML::Value << spritesheet->GridSize;
		output << YAML::Key << "CenterOffset" << YAML::Value << spritesheet->CenterOffset;
		output << YAML::Key << "ViewPos" << YAML::Value << spritesheet->ViewPos;
		output << YAML::Key << "ShowImageBorders" << YAML::Value << spritesheet->ShowImageBorders;
		output << YAML::Key << "ExportPath" << YAML::Value << spritesheet->ExportPath.string();

		output << YAML::Key << "Sprites" << YAML::Value << YAML::BeginSeq;

		for (const SpriteData& sprite : spritesheet->Sprites)
		{
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

		if (!data)
		{
			SYSTEM_ERROR("Error while deserializing the spritesheet: {}", metadata.Path.string());

			return new Spritesheet();
		}

		Spritesheet* spritesheet = new Spritesheet();

		const u64 handle         = TryDeserializeNode<u64>(data, "TextureHandle", 0);
		Texture2DAsset** texture = handle ? AssetManager::GetAssetRaw<Texture2DAsset>(handle) : nullptr;

		spritesheet->SetSpritesheetTexture(texture);
		spritesheet->ViewZoom         = TryDeserializeNode<f32>(data, "ViewZoom", 1.0f);
		spritesheet->GridSize         = TryDeserializeNode<f32>(data, "GridSize", 32.0f);
		spritesheet->CenterOffset     = TryDeserializeNode<glm::vec2>(data, "CenterOffset", glm::vec2(0.0f, 0.0f));
		spritesheet->ViewPos          = TryDeserializeNode<glm::vec2>(data, "ViewPos", glm::vec2(0.0f, 0.0f));
		spritesheet->ShowImageBorders = TryDeserializeNode<bool>(data, "ShowImageBorders", false);
		spritesheet->ExportPath       = TryDeserializeNode<std::string>(data, "ExportPath", "");

		YAML::Node sprites = data["Sprites"];
		for (YAML::Node sprite : sprites)
		{
			std::string name = TryDeserializeNode<std::string>(sprite["Sprite"], "Name", "Sprite");

			SpriteData spriteData(name);
			spriteData.Position = TryDeserializeNode<glm::vec2>(sprite["Sprite"], "Position", glm::vec2(0.0f, 0.0f));
			spriteData.Size     = TryDeserializeNode<glm::vec2>(sprite["Sprite"], "Size", glm::vec2(1.0f, 1.0f));

			spritesheet->Sprites.emplace_back(spriteData);
		}

		return spritesheet;
	}

	void FontSerializer::Serialize(const AssetMetaData& /*metadata*/)
	{
		ASSERT(false, "Font serialization is not supported!");
	}

	Asset* FontSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		const std::filesystem::path path = ProjectContext::Get()->GetAssetDirectory() / metadata.Path;

		YAML::Node file = YAML::LoadFile(path.string());

		YAML::Node data = file["Font"];

		if (!data)
		{
			SYSTEM_ERROR("Error while deserializing the font: {}", metadata.Path.string());

			return nullptr;
		}

		const u64 fontSourceHandle = TryDeserializeNode<u64>(data, "FontSourceHandle", 0);

		const AssetMetaData& sourceMetadata = AssetManager::GetAssetMetaData(fontSourceHandle);

		FontAssetSpecification spec;
		spec.Path = ProjectContext::Get()->GetAssetDirectory() / sourceMetadata.Path;
		spec.Charset =
		    (OpenGL::FontCharsetType)TryDeserializeNode<u32>(data, "Charset", (int)OpenGL::FontCharsetType::ASCII);
		spec.PreloadedAtlas = FontCache::TryGetCachedAtlas(metadata.Handle, metadata.ModificationTime);

		FontAsset* font = new FontAsset(spec);

		if (!spec.PreloadedAtlas)
			FontCache::CacheAtlas(font->GetAtlasTexture(), metadata.Handle, metadata.ModificationTime);

		return font;
	}

	void FontSourceSerializer::Serialize(const AssetMetaData& /*metadata*/)
	{
	}

	Asset* FontSourceSerializer::TryLoadAsset(const AssetMetaData& /*metadata*/)
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

		for (Sprite** const sprite : animation->Sprites)
		{
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

		if (!data)
		{
			SYSTEM_ERROR("Error while deserializing the animation: {}", metadata.Path.string());

			return new Animation2D();
		}

		Animation2D* animation = new Animation2D();

		animation->Speed         = TryDeserializeNode<f32>(data, "Speed", 1.0f);
		animation->ReverseAlongX = TryDeserializeNode<bool>(data, "ReverseAlongX", false);
		animation->ReverseAlongY = TryDeserializeNode<bool>(data, "ReverseAlongY", false);

		YAML::Node sprites = data["Sprites"];
		for (YAML::Node sprite : sprites)
		{
			Sprite** spr =
			    AssetManager::GetAssetRaw<Sprite>(TryDeserializeNode<u64>(sprite["Sprite"], "SpriteHandle", 0));

			animation->Sprites.emplace_back(spr);
		}

		return animation;
	}

	void SoundSerializer::Serialize(const AssetMetaData& /*metadata*/)
	{
	}

	Asset* SoundSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		Sound* sound = new Sound(ProjectContext::Get()->GetAssetDirectory() / metadata.Path);

		return sound;
	}

	void PrefabSerializer::Serialize(const AssetMetaData& metadata)
	{
		Prefab* prefab     = *AssetManager::GetAssetRaw<Prefab>(metadata.Handle);
		Scene* prefabScene = prefab->GetSceneRaw();

		YAML::Emitter output;

		output << YAML::BeginMap;
		output << YAML::Key << "Prefab" << YAML::Value;

		output << YAML::BeginMap;
		output << YAML::Key << "RootEntityHandle" << YAML::Value << prefab->GetRootEntity().GetID();

		output << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		std::map<u64, Entity> sortedEntities;

		for (auto&& [handle, idc] : prefabScene->GetRegistry().GetEntitiesWith<IDComponent>().each())
			sortedEntities[idc.ID] = Entity{handle, prefabScene};

		for (auto [id, entity] : sortedEntities)
		{
			SceneSerializer::SerializeEntity(output, entity, prefabScene);
		}

		output << YAML::EndSeq;
		output << YAML::EndMap;

		std::ofstream fout(ProjectContext::Get()->GetAssetDirectory() / metadata.Path);
		fout << output.c_str();
	}

	Asset* PrefabSerializer::TryLoadAsset(const AssetMetaData& metadata)
	{
		const std::filesystem::path path = ProjectContext::Get()->GetAssetDirectory() / metadata.Path;

		YAML::Node file = YAML::LoadFile(path.string());

		YAML::Node data = file["Prefab"];

		if (!data)
		{
			ASSERT(false, "Error while deserializing the prefab: {}, no entities section found!", path.string());

			return new Prefab();
		}

		YAML::Node entities = data["Entities"];

		Prefab* prefab     = new Prefab();
		Scene* prefabScene = prefab->GetSceneRaw();

		SceneSerializer::DeserializeEntitiesNode(entities, prefabScene);

		const u64 rootHandle = TryDeserializeNode<u64>(data, "RootEntityHandle", 0);
		prefab->SetRootEntity(prefabScene->GetEntityByID(rootHandle));

		return prefab;
	}

} // namespace SW
