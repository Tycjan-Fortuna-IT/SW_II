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

#include "Asset/Panels/AssetEditorPanel.hpp"
#include "Asset/Spritesheet.hpp"
#include "Asset/Sprite.hpp"

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

		void SetAssetHandle(AssetHandle handle) override;

	private:
		ImGuiEx::Canvas m_Canvas;

		Spritesheet** m_Spritesheet = nullptr;
		
		bool m_IsCanvasHovered = false;

		std::string m_SearchString;		/**< The search string used to filter entities in the scene hierarchy. */

	private:
		void DrawRectOnCanvas(ImDrawList* drawList, const glm::vec2& position, const glm::vec2& size, u32 color) const;
		void RenderSpriteCards(f32 vscale);
		void AddNewSprite();
		void ExportSprites() const;
	};

}

