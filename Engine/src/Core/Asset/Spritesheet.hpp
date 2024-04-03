#pragma once

#include "Asset.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Sprite.hpp"
#include "AssetManager.hpp"

namespace SW {

	struct SpriteData
	{
		SpriteData(const std::string& name)
			: Name(name) {}

		std::string Name;
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Scale = glm::vec2(1.0f);
		glm::vec4 Tint = glm::vec4(1.0f);
	};

	class Spritesheet final : public Asset
	{
	public:
		Spritesheet() = default;

		AssetType GetAssetType() const override { return AssetType::Spritesheet; }

		Texture2D* GetSpritesheetTexture() const { return m_SpritesheetTexture ? *m_SpritesheetTexture : nullptr; }
		void SetSpritesheetTexture(Texture2D** texture) { m_SpritesheetTexture = texture; }

	public:
		std::vector<SpriteData> Sprites;

		f32 ViewZoom = 1.0f;
		f32 GridScale = 64.0f;

		glm::vec2 CenterOffset = glm::vec2(0.f, 0.f);
		glm::vec2 ViewPos = glm::vec2(0.f, 0.f);

		std::filesystem::path ExportPath; // where all sprites should be exported

		bool ShowImageBorders = false;
		
	private:
		Texture2D** m_SpritesheetTexture = nullptr;
	};

}
