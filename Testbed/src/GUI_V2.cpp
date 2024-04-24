#include "GUI_V2.hpp"

namespace SW::GUI2 {

	namespace Layout {

		bool BeginMenuBar(const ImRect& barRectangle)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			if (window->SkipItems)
				return false;

			ImGui::BeginGroup();
			ImGui::PushID("##menubar");

			const ImVec2 padding = window->WindowPadding;

			ImRect bar = GUI::RectOffset(barRectangle, 0.0f, padding.y);

			ImRect clip(
				IM_ROUND(
					ImMax(window->Pos.x, bar.Min.x + window->WindowBorderSize + window->Pos.x - 10.0f)),
					IM_ROUND(bar.Min.y + window->WindowBorderSize + window->Pos.y
					),
				IM_ROUND(ImMax(
					bar.Min.x + window->Pos.x, bar.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize))),
					IM_ROUND(bar.Max.y + window->Pos.y
					)
			);

			clip.ClipWith(window->OuterRectClipped);

			ImGui::PushClipRect(clip.Min, clip.Max, false);

			window->DC.CursorPos = window->DC.CursorMaxPos = ImVec2(bar.Min.x + window->Pos.x, bar.Min.y + window->Pos.y);
			window->DC.LayoutType = ImGuiLayoutType_Horizontal;
			window->DC.NavLayerCurrent = ImGuiNavLayer_Menu;
			window->DC.MenuBarAppending = true;

			ImGui::AlignTextToFramePadding();

			return true;
		}

		void EndMenuBar()
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			if (window->SkipItems)
				return;

			ImGuiContext& g = *GImGui;

			if (
				ImGui::NavMoveRequestButNoResultYet() &&
				(g.NavMoveDir == ImGuiDir_Left || g.NavMoveDir == ImGuiDir_Right) &&
				(g.NavWindow->Flags & ImGuiWindowFlags_ChildMenu)
			) {
				ImGuiWindow* nav_earliest_child = g.NavWindow;

				while (nav_earliest_child->ParentWindow && (nav_earliest_child->ParentWindow->Flags & ImGuiWindowFlags_ChildMenu))
					nav_earliest_child = nav_earliest_child->ParentWindow;

				if (
					nav_earliest_child->ParentWindow == window &&
					nav_earliest_child->DC.ParentLayoutType == ImGuiLayoutType_Horizontal &&
					(g.NavMoveFlags & ImGuiNavMoveFlags_Forwarded) == 0
				) {
					const ImGuiNavLayer layer = ImGuiNavLayer_Menu;

					ImGui::FocusWindow(window);
					ImGui::SetNavID(window->NavLastIds[layer], layer, 0, window->NavRectRel[layer]);

					g.NavDisableHighlight = true;
					g.NavDisableMouseHover = g.NavMousePosDirty = true;

					ImGui::NavMoveRequestForward(g.NavMoveDir, g.NavMoveClipDir, g.NavMoveFlags, g.NavMoveScrollFlags);
				}
			}

			ImGui::PopClipRect();
			ImGui::PopID();

			IM_MSVC_WARNING_SUPPRESS(6011); // Static Analysis false positive "warning C6011: Dereferencing NULL pointer 'window'"
			window->DC.MenuBarOffset.x = window->DC.CursorPos.x - window->Pos.x;

			g.GroupStack.back().EmitItem = false;

			ImGui::EndGroup();

			window->DC.LayoutType = ImGuiLayoutType_Vertical;
			window->DC.NavLayerCurrent = ImGuiNavLayer_Main;
			window->DC.MenuBarAppending = false;
		}

	}

	namespace Components {

		void ItemActivityOutline(
			OutlineFlags flags /*= OutlineFlags_All*/, ImColor activeCol /*= GUI::Theme::ActivityOutline*/, ImColor hoverCol /*= GUI::Theme::Outline*/,
			f32 rounding /*= GImGui->Style.FrameRounding*/
		) {
			if (IsItemDisabled())
				return;

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			const ImRect rect = RectExpanded(GetItemRect(), 2.0f, 2.0f);

			if ((flags & OutlineFlags_WhenActive) && ImGui::IsItemActive()) {
				if (flags & OutlineFlags_HighlightActive) {
					drawList->AddRect(rect.Min, rect.Max, activeCol, rounding, 0, 1.5f);
				}
			} else if ((flags & OutlineFlags_WhenHovered) && ImGui::IsItemHovered() && !ImGui::IsItemActive()) {
				drawList->AddRect(rect.Min, rect.Max, hoverCol, rounding, 0, 1.5f);
			}
		}

		void RectangleOutline(
			ImRect rect, ImColor color /*= GUI::Theme::Outline*/, f32 thickness /*= 1.0f*/, f32 rounding /*= 0.0f*/,
			f32 offsetX /*= 0.0f*/, f32 offsetY /*= 0.0f*/
		) {
			ImVec2 min = rect.Min;
			min.x -= thickness;
			min.y -= thickness;
			min.x += offsetX;
			min.y += offsetY;

			ImVec2 max = rect.Max;
			max.x += thickness;
			max.y += thickness;
			max.x += offsetX;
			max.y += offsetY;

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Border)), rounding, 0, thickness);
		}

		void ItemOutline(ImColor color /*= GUI::Theme::Outline*/, f32 thickness /*= 1.0f*/, f32 rounding /*= 0.0f*/, f32 offsetX /*= 0.0f*/, f32 offsetY /*= 0.0f */)
		{
			RectangleOutline(GUI2::GetItemRect(), color, thickness, rounding, offsetX, offsetX);
		}

		bool Checkbox(bool* value)
		{
			bool modified = false;
			const f32 w = ImGui::GetContentRegionAvail().x;

			ImGui::SetNextItemWidth(w);

			f32 width = ImGui::GetIO().FontGlobalScale * ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

			GUI::MoveMousePosX(w / 2 - width / 2);

			modified = ImGui::Checkbox("##checkbox", value);

			GUI2::Components::ItemActivityOutline(OutlineFlags_All, GUI::Theme::ActivityOutline,
				GUI::Theme::ActivityOutline, 2.f);

			return modified;
		}

		bool Toggle(bool* value)
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			const ImVec4* colors = ImGui::GetStyle().Colors;

			const f32 height = ImGui::GetFrameHeight();
			const f32 width = height * 2.f;
			const f32 radius = height * 0.50f;
			const f32 rounding = height * 0.2f;

			bool modified = false;
			const f32 w = ImGui::GetContentRegionAvail().x;
			GUI::MoveMousePosX(w / 2 - width / 2);

			const ImVec2 p = ImGui::GetCursorScreenPos();

			ImGui::InvisibleButton("##toggle2", ImVec2(width, height));
			if (ImGui::IsItemClicked()) {
				*value = !*value;
				modified = true;
			}

			f32 t = *value ? 1.0f : 0.0f;

			ImGuiContext& g = *GImGui;
			const f32 animSpeed = 0.03f;

			if (g.LastActiveId == g.CurrentWindow->GetID("##toggle2")) {
				f32 t_anim = ImSaturate(g.LastActiveIdTimer / animSpeed);

				t = *value ? (t_anim) : (1.0f - t_anim);
			}

			ImU32 col_bg = ImGui::GetColorU32(*value ? ImGui::ColorConvertU32ToFloat4(GUI::Theme::Selection) : colors[ImGuiCol_Button]);

			Components::ItemActivityOutline(Components::OutlineFlags_All, GUI::Theme::ActivityOutline,
				GUI::Theme::ActivityOutline, rounding);

			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, rounding);
			draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));

			return modified;
		}

	}

	namespace Widgets {


	}

}
