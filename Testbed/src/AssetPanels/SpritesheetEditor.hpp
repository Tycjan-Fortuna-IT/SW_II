/**
 * @file SpritesheetEditor.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-17
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Asset/Panels/AssetEditorPanel.hpp"

namespace SW {

	class Spritesheet final : public Asset
	{
	public:
		Spritesheet(AssetHandle handle, Texture2D* spritesheetTexture)
			: Asset(handle, spritesheetTexture->GetPath()), m_SpritesheetTexture(spritesheetTexture) {}

		AssetType GetAssetType() const override { return AssetType::Spritesheet; }

		Texture2D* GetSpritesheetTexture() const { return m_SpritesheetTexture; }

	private:
		Texture2D* m_SpritesheetTexture = nullptr;
	};

	struct SpriteData final
	{
		std::string Name;
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Scale = glm::vec2(1.0f);
		glm::vec4 Tint = glm::vec4(1.0f);
	};

	class SpritesheetEditor final : public AssetEditorPanel
	{
	public:
		SpritesheetEditor(const char* name, const char* icon);

		void OnUpdate(Timestep ts) override final;

		void OnWindowStylePush() override final;

		void OnWindowStylePop() override final;

		void OnOpen() override final;

		void OnClose() override final;

		void Render() override final;

		void SetAsset(Asset* asset) override;

	private:
		std::vector<SpriteData> m_Sprites;

		Spritesheet* m_Spritesheet = nullptr;

		void RenderSpriteCards(f32 scale);
	};

}

