/**
 * @file Spritesheet.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset.hpp"
#include "AssetManager.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Sprite.hpp"

namespace SW
{

	struct SpriteData
	{
		SpriteData(const std::string& name) : Name(name) {}

		std::string Name;
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Size     = glm::vec2(32.0f);
	};

	class Spritesheet final : public Asset
	{
	public:
		Spritesheet() = default;

		static AssetType GetStaticType() { return AssetType::Spritesheet; }
		AssetType GetAssetType() const override { return AssetType::Spritesheet; }

		Texture2D* GetSpritesheetTexture() const { return m_SpritesheetTexture ? *m_SpritesheetTexture : nullptr; }
		void SetSpritesheetTexture(Texture2D** texture) { m_SpritesheetTexture = texture; }

		Texture2D** GetSpritesheetTextureRaw() const { return m_SpritesheetTexture; }

	public:
		std::vector<SpriteData> Sprites;

		f32 ViewZoom = 1.0f;
		f32 GridSize = 64.0f;

		glm::vec2 CenterOffset = glm::vec2(0.f, 0.f);
		glm::vec2 ViewPos      = glm::vec2(0.f, 0.f);

		std::filesystem::path ExportPath; // where all sprites should be exported

		bool ShowImageBorders = false;
		bool ShowGridCoords   = false;

	private:
		Texture2D** m_SpritesheetTexture = nullptr;
	};

} // namespace SW
