/**
 * @file SpritesheetEditor.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-17
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <imgui_canvas.h>

#include "Core/Asset/Panels/AssetEditorPanel.hpp"
#include "Core/Asset/Spritesheet.hpp"
#include "Core/Asset/Sprite.hpp"

namespace SW {

	class SpritesheetEditor final : public AssetEditorPanel
	{
	public:
		SpritesheetEditor(const char* name, const char* icon);

		void OnUpdate(Timestep ts) override;

		void OnWindowStylePush() override;

		void OnWindowStylePop() override;

		void OnOpen() override;

		void OnClose() override;

		void Render() override;

		void SetAsset(Asset* asset) override;

	private:
		ImGuiEx::Canvas m_Canvas;

		AssetHandle m_SpritesheetHandle = 0u;
		Spritesheet* m_Spritesheet = nullptr;

	private:
		void DrawSpriteRectOnCanvas(ImDrawList* drawList, const Sprite* sprite) const;
		void RenderSpriteCards(f32 scale);
	};

}

