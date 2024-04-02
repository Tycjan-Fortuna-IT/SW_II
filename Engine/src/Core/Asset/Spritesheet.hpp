#pragma once

#include "Asset.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Sprite.hpp"
#include "AssetManager.hpp"

namespace SW {

	class Spritesheet final : public Asset
	{
	public:
		Spritesheet() = default;

		AssetType GetAssetType() const override { return AssetType::Spritesheet; }

		AssetHandle GetSpritesheetTextureHandle() const { return m_SpritesheetTextureHandle; }
		void SetSpritesheetTextureHandle(AssetHandle handle){ m_SpritesheetTextureHandle = handle; }

		Texture2D* GetSpritesheetTexture() const { 
			return m_SpritesheetTextureHandle ? AssetManager::GetAssetRaw<Texture2D>(m_SpritesheetTextureHandle) : nullptr;
		}

		const std::vector<Sprite*>& GetSprites() const { return m_Sprites; }

		f32 GetViewZoom() const { return m_ViewZoom; }
		void SetViewZoom(f32 val) { m_ViewZoom = val; }
	
		f32 GetGridScale() const { return m_GridScale; }
		void SetGridScale(f32 val) { m_GridScale = val; }

		const glm::vec2& GetCenterOffset() const { return CenterOffset; }
		void SetCenterOffset(const glm::vec2& val) { CenterOffset = val; }
	
		const glm::vec2& GetViewPos() const { return ViewPos; }
		void SetViewPos(const glm::vec2& val) { ViewPos = val; }

	private:
		AssetHandle m_SpritesheetTextureHandle = 0u;
		std::vector<Sprite*> m_Sprites;

		f32 m_ViewZoom = 1.0f;
		f32 m_GridScale = 64.0f;
		
		glm::vec2 CenterOffset = glm::vec2(0.f, 0.f);
		glm::vec2 ViewPos = glm::vec2(0.f, 0.f);
	};

}
