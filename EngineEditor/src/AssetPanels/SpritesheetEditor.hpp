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
#include "Core/Asset/Spritesheet.hpp"
#include "Core/Asset/Sprite.hpp"

namespace SW {

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
		Spritesheet* m_Spritesheet = nullptr;

	private:
		void DrawSpriteRectOnCanvas(ImDrawList* drawList, const Sprite* sprite) const;
		void RenderSpriteCards(f32 scale);
	};

}

