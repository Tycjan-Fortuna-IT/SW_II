#include "GUI/GUI.hpp"

namespace SW::GUI {

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
