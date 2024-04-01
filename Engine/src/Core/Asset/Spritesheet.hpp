#pragma once

#include "Asset.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Sprite.hpp"

namespace SW {

	class Spritesheet final : public Asset
	{
	public:
		Spritesheet() = default;
		Spritesheet(Texture2D* spritesheetTexture)
			: m_SpritesheetTexture(spritesheetTexture) {}

		AssetType GetAssetType() const override { return AssetType::Spritesheet; }

		Texture2D* GetSpritesheetTexture() const { return m_SpritesheetTexture; }

		f32 GetViewZoom() const { return m_ViewZoom; }
		void SetViewZoom(f32 val) { m_ViewZoom = val; }
	
		f32 GetGridScale() const { return m_GridScale; }
		void SetGridScale(f32 val) { m_GridScale = val; }

		const glm::vec2& GetCenterOffset() const { return CenterOffset; }
		void SetCenterOffset(const glm::vec2& val) { CenterOffset = val; }
	
		const std::vector<Sprite*>& GetSprites() const { return m_Sprites; }

	private:
		Texture2D* m_SpritesheetTexture = nullptr;
		std::vector<Sprite*> m_Sprites;

		f32 m_ViewZoom = 1.0f;
		f32 m_GridScale = 64.0f;
		
		glm::vec2 CenterOffset = glm::vec2(0.f, 0.f);
	};

}
