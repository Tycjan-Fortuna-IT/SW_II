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

			ImRect bar = GUI2::RectOffset(barRectangle, 0.0f, padding.y);

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

		bool Checkbox(bool* value, bool center)
		{
			bool modified = false;
			const f32 w = ImGui::GetContentRegionAvail().x;

			f32 width = ImGui::GetIO().FontGlobalScale * ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

			if (center)
				GUI2::MoveMousePosX(w / 2 - width / 2);

			modified = ImGui::Checkbox("##checkbox", value);

			Components::ItemActivityOutline(OutlineFlags_All, GUI::Theme::ActivityOutline,
				GUI::Theme::ActivityOutline, 2.f);

			return modified;
		}

		bool Toggle(bool* value, bool center)
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			const ImVec4* colors = ImGui::GetStyle().Colors;

			const f32 height = ImGui::GetFrameHeight();
			const f32 width = height * 2.f;
			const f32 radius = height * 0.50f;
			const f32 rounding = height * 0.2f;

			bool modified = false;
			const f32 w = ImGui::GetContentRegionAvail().x;
			if (center)
				GUI2::MoveMousePosX(w / 2 - width / 2);

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

		bool Vector2Input(glm::vec2* vector, f32 resetValue /*= 0.f*/, f32 min /*= -FLT_MAX*/, f32 max /*= FLT_MAX*/, const std::string& format /*= "%.2f" */)
		{
			bool modified = false;

			const ImGuiIO& io = ImGui::GetIO();

			ImFont* boldFont = GUI::Appearance::GetFonts().DefaultBoldFont;

			constexpr f32 spacingX = 8.0f;
			const ImVec2 size = ImGui::GetContentRegionAvail();

			GUI2::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
			GUI2::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

			constexpr f32 framePadding = 2.0f;
			const f32 lineHeight = GImGui->Font->FontSize + framePadding * 2.0f;
			const ImVec2 buttonSize = { lineHeight + 7.0f, ImGui::GetFrameHeight() };
			const f32 inputItemWidth = size.x / 2.0f - buttonSize.x - 4.0f;

			// X
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::PushFont(boldFont);
				if (ImGui::Button("X", buttonSize)) {
					vector->x = resetValue;
					modified = true;
				}
				ImGui::PopFont();
				ImGui::PopStyleColor(4);

				ImGui::SameLine();
				ImGui::SetNextItemWidth(inputItemWidth);
				if (ImGui::DragFloat("##X", &vector->x, 0.05f, min, max, format.c_str()))
					modified = true;
				Components::ItemActivityOutline(Components::OutlineFlags_All, GUI::Theme::ActivityOutline, GUI::Theme::ActivityOutline);
				ImGui::PopStyleVar();
			}

			ImGui::SameLine();

			// Y
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::PushFont(boldFont);
				if (ImGui::Button("Y", buttonSize)) {
					vector->y = resetValue;
					modified = true;
				}

				ImGui::PopFont();
				ImGui::PopStyleColor(4);

				ImGui::SameLine();
				ImGui::SetNextItemWidth(inputItemWidth);
				if (ImGui::DragFloat("##Y", &vector->y, 0.05f, min, max, format.c_str()))
					modified = true;
				Components::ItemActivityOutline(Components::OutlineFlags_All, GUI::Theme::ActivityOutline, GUI::Theme::ActivityOutline);
				ImGui::PopStyleVar();
			}

			return modified;
		}

		bool Vector3Input(glm::vec3* vector, f32 resetValue /*= 0.f*/, f32 min /*= -FLT_MAX*/, f32 max /*= FLT_MAX*/, const std::string& format /*= "%.2f" */)
		{
			bool modified = false;

			const ImGuiIO& io = ImGui::GetIO();

			ImFont* boldFont = GUI::Appearance::GetFonts().DefaultBoldFont;

			constexpr f32 spacingX = 7.0f;
			const ImVec2 size = ImGui::GetContentRegionAvail();

			GUI2::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
			GUI2::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

			constexpr f32 framePadding = 2.0f;
			const f32 lineHeight = GImGui->Font->FontSize + framePadding * 2.0f;
			const ImVec2 buttonSize = { lineHeight + 7.0f, ImGui::GetFrameHeight() };
			const f32 inputItemWidth = size.x / 3.0f - buttonSize.x - 5.0f;

			// X
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::PushFont(boldFont);
				if (ImGui::Button("X", buttonSize)) {
					vector->x = resetValue;
					modified = true;
				}
				ImGui::PopFont();
				ImGui::PopStyleColor(4);

				ImGui::SameLine();
				ImGui::SetNextItemWidth(inputItemWidth);
				if (ImGui::DragFloat("##X", &vector->x, 0.05f, min, max, format.c_str()))
					modified = true;
				Components::ItemActivityOutline(Components::OutlineFlags_All, GUI::Theme::ActivityOutline, GUI::Theme::ActivityOutline);
				ImGui::PopStyleVar();
			}

			ImGui::SameLine();

			// Y
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::PushFont(boldFont);
				if (ImGui::Button("Y", buttonSize)) {
					vector->y = resetValue;
					modified = true;
				}
				ImGui::PopFont();
				ImGui::PopStyleColor(4);

				ImGui::SameLine();
				ImGui::SetNextItemWidth(inputItemWidth);
				if (ImGui::DragFloat("##Y", &vector->y, 0.05f, min, max, format.c_str()))
					modified = true;
				Components::ItemActivityOutline(Components::OutlineFlags_All, GUI::Theme::ActivityOutline, GUI::Theme::ActivityOutline);
				ImGui::PopStyleVar();
			}

			ImGui::SameLine();

			// Z
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::PushFont(boldFont);
				if (ImGui::Button("Z", buttonSize)) {
					vector->z = resetValue;
					modified = true;
				}
				ImGui::PopFont();
				ImGui::PopStyleColor(4);

				ImGui::SameLine();
				ImGui::SetNextItemWidth(inputItemWidth);
				if (ImGui::DragFloat("##Z", &vector->z, 0.05f, min, max, format.c_str()))
					modified = true;
				Components::ItemActivityOutline(Components::OutlineFlags_All, GUI::Theme::ActivityOutline, GUI::Theme::ActivityOutline);
				ImGui::PopStyleVar();
			}

			return modified;
		}

		bool Vector3ColorPicker(glm::vec3* vector, ImGuiColorEditFlags flags /*= ImGuiColorEditFlags_None */)
		{
			bool modified = false;

			const f32 w = ImGui::GetContentRegionAvail().x;
			ImGui::PushItemWidth(w);

			modified = ImGui::ColorEdit3("##Color", glm::value_ptr(*vector), flags);

			Components::ItemActivityOutline();

			return modified;
		}

		bool Vector4ColorPicker(glm::vec4* vector, ImGuiColorEditFlags flags /*= ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf */)
		{
			bool modified = false;

			const f32 w = ImGui::GetContentRegionAvail().x;
			ImGui::PushItemWidth(w);

			modified = ImGui::ColorEdit4("##Color", glm::value_ptr(*vector), flags);

			Components::ItemActivityOutline();

			return modified;
		}

		bool ToggleButton(
			bool* value, const char* whenOnLabel, const char* whenOffLabel, bool center /*= true*/,
			ImGuiButtonFlags buttonFlags /*= ImGuiButtonFlags_None */
		) {
			if (*value) {
				ImVec4 color = ImGui::ColorConvertU32ToFloat4(GUI::Theme::SelectionDark);

				ImGui::PushStyleColor(ImGuiCol_Button, color);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
			} else {
				ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_Button];
				ImVec4 hoveredColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
				ImGui::PushStyleColor(ImGuiCol_Button, color);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
			}

			bool modified = false;

			const f32 w = ImGui::GetContentRegionAvail().x;

			f32 width = ImGui::GetIO().FontGlobalScale * ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
			f32 textSize = ImGui::CalcTextSize(*value ? whenOnLabel : whenOffLabel).x;

			if (center)
				GUI2::MoveMousePosX(w / 2 - textSize / 2 - width / 2);

			if (ImGui::ButtonEx(*value ? whenOnLabel : whenOffLabel, { 0, 0 }, buttonFlags)) {
				*value = !*value;
				modified = true;
			}

			Components::ItemActivityOutline(OutlineFlags_All, GUI::Theme::ActivityOutline,
					GUI::Theme::ActivityOutline, 2.f);

			ImGui::PopStyleColor(3);

			return modified;
		}

		bool ImageButton(const Texture2D& texture, const glm::vec2& size, ImU32 tintHovered /*= Color::DarkGray*/)
		{
			const f32 padding = (size.y - (f32)texture.GetHeight()) / 2.0f;

			if (ImGui::InvisibleButton(std::to_string(texture.GetTexHandle()).c_str(), ImVec2(size.x, size.y)))
				return true;

			ImDrawList* drawList = ImGui::GetWindowDrawList();

			const ImRect rect = GUI::RectExpanded(GUI::GetItemRect(), -padding, -padding);

			const ImTextureID ID = GUI::GetTextureID(texture);

			if (ImGui::IsItemHovered())
				drawList->AddImage(ID, rect.Min, rect.Max, { 0, 1 }, { 1, 0 }, tintHovered);
			else
				drawList->AddImage(ID, rect.Min, rect.Max, { 0, 1 }, { 1, 0 });

			return false;
		}

	}

	namespace Widgets {

		bool Splitter(bool splitVertically, f32 thickness, f32* size1, f32* size2, f32 minSize1, f32 minSize2, f32 splitterLongAxisSize /*= -1.0f*/)
		{
			ImGuiContext& g = *GImGui;
			ImGuiWindow* window = g.CurrentWindow;
			ImGuiID id = window->GetID("##Splitter");
			ImRect bb;

			bb.Min = window->DC.CursorPos + (splitVertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
			bb.Max = bb.Min + ImGui::CalcItemSize(splitVertically ?
				ImVec2(thickness, splitterLongAxisSize) : ImVec2(splitterLongAxisSize, thickness), 0.0f, 0.0f);

			return ImGui::SplitterBehavior(bb, id, splitVertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, minSize1, minSize2, 0.0f);
		}

		void DrawScale(const ImVec2& from, const ImVec2& to, f32 majorUnit, f32 minorUnit, f32 labelAlignment, f32 sign /*= 1.0f*/)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 direction = (to - from) * ImInvLength(to - from, 0.0f);
			ImVec2 normal = ImVec2(-direction.y, direction.x);
			f32 distance = sqrtf(ImLengthSqr(to - from));

			if (ImDot(direction, direction) < FLT_EPSILON)
				return;

			f32 minorSize = 5.0f;
			f32 majorSize = 10.0f;
			f32 labelDistance = 8.0f;

			drawList->AddLine(from, to, IM_COL32(255, 255, 255, 255));

			ImVec2 p = from;

			for (f32 d = 0.0f; d <= distance; d += minorUnit, p += direction * minorUnit)
				drawList->AddLine(p - normal * minorSize, p + normal * minorSize, IM_COL32(255, 255, 255, 255));

			for (f32 d = 0.0f; d <= distance + majorUnit; d += majorUnit) {
				p = from + direction * d;

				drawList->AddLine(p - normal * majorSize, p + normal * majorSize, IM_COL32(255, 255, 255, 255));

				if (d == 0.0f)
					continue;

				char label[16];
				snprintf(label, 15, "%g", d * sign);
				ImVec2 labelSize = ImGui::CalcTextSize(label);

				ImVec2 labelPosition = p + ImVec2(fabsf(normal.x), fabsf(normal.y)) * labelDistance;
				f32 labelAlignedSize = ImDot(labelSize, direction);
				labelPosition += direction * (-labelAlignedSize + labelAlignment * labelAlignedSize * 2.0f);
				labelPosition = ImFloor(labelPosition + ImVec2(0.5f, 0.5f));

				drawList->AddText(labelPosition, IM_COL32(255, 255, 255, 255), label);
			}
		}

        void ClippedText(
			ImDrawList* drawList, const ImVec2& posMin, const ImVec2& posMax, const char* text, const char* textDisplayEnd,
			const ImVec2* textSizeIfKnown, const ImVec2& align, const ImRect* cliprect, f32 wrapWidth
		) {
			// Perform CPU side clipping for single clipped element to avoid using scissor state
			ImVec2 pos = posMin;
			const ImVec2 textSize = textSizeIfKnown ? *textSizeIfKnown : ImGui::CalcTextSize(text, textDisplayEnd, false, wrapWidth);

			const ImVec2* clipMin = cliprect ? &cliprect->Min : &posMin;
			const ImVec2* clipMax = cliprect ? &cliprect->Max : &posMax;

			// Align whole block. We should defer that to the better rendering function when we'll have support for individual line alignment.
			if (align.x > 0.0f)
				pos.x = ImMax(pos.x, pos.x + (posMax.x - pos.x - textSize.x) * align.x);

			if (align.y > 0.0f)
				pos.y = ImMax(pos.y, pos.y + (posMax.y - pos.y - textSize.y) * align.y);

			// Render
			ImVec4 fineClipRect(clipMin->x, clipMin->y, clipMax->x, clipMax->y);
			drawList->AddText(nullptr, 0.0f, pos, ImGui::GetColorU32(ImGuiCol_Text), text, textDisplayEnd, wrapWidth, &fineClipRect);
		}

		void ClippedText(const ImVec2& posMin, const ImVec2& posMax, const char* text, const char* textEnd, const ImVec2* textSizeIfKnown, const ImVec2& align, const ImRect* clipRect, f32 wrapWidth)
		{
			// Hide anything after a '##' string
			const char* textDisplayEnd = ImGui::FindRenderedTextEnd(text, textEnd);
			const int text_len = static_cast<int>(textDisplayEnd - text);
			if (text_len == 0)
				return;

			ImGuiContext& g = *GImGui;
			ImGuiWindow* window = g.CurrentWindow;
			ClippedText(window->DrawList, posMin, posMax, text, textDisplayEnd, textSizeIfKnown, align, clipRect, wrapWidth);
			if (g.LogEnabled)
				ImGui::LogRenderedText(&posMin, text, textDisplayEnd);
		}

		bool DrawFolderPicker(std::filesystem::path* path, const std::filesystem::path& relative)
		{
			const f32 posx = ImGui::GetCursorPosX();
			const f32 framePaddingY = ImGui::GetStyle().FramePadding.y;
			constexpr f32 bw = 28.f;
			constexpr f32 buttonOffset = 11.f;
			constexpr f32 iconOffset = 6.f;
			constexpr f32 hintOffset = 28.f;

			bool modified = false;
			bool isPicked = !path->empty();

			std::string tag = "None";

			const f32 w = ImGui::GetContentRegionAvail().x;
			f32 inputWidth = 0.0f;

			{
				GUI2::ScopedStyle FrameRounding(ImGuiStyleVar_FrameRounding, 3.0f);
				GUI2::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(28.0f, framePaddingY));

				if (ImGui::Button("##folder_picker", { isPicked ? w - bw : w , ImGui::GetFrameHeight() })) {
					std::filesystem::path pickedPath = FileSystem::OpenFolderDialog(relative.string().c_str());

					if (!pickedPath.empty()) {
						*path = std::filesystem::relative(pickedPath, relative);
						modified = true;
					}
				}
				inputWidth = ImGui::GetItemRectSize().x;

				if (ImGui::IsItemHovered() && isPicked) {
					std::filesystem::path fullPath = relative / *path;

					ImGui::BeginTooltip();
					ImGui::TextUnformatted(fullPath.string().c_str());
					ImGui::EndTooltip();
				}

				Components::ItemActivityOutline();
			}

			if (isPicked) {
				tag = path->string();

				ImGui::SameLine(w - bw + buttonOffset);
				ImGui::SetNextItemAllowOverlap();

				GUI2::ScopedColor Border(ImGuiCol_Border, ImVec4{ 1.f, 1.f , 1.f , 0.f });
				GUI2::ScopedColor Button(ImGuiCol_Button, ImVec4{ 1.f, 1.f , 1.f , 0.f });

				if (ImGui::Button(SW_ICON_CLOSE_OCTAGON, ImVec2{ bw, ImGui::GetFrameHeight() })) {
					path->clear();
					modified = true;
				}

				if (GUI2::IsItemHovered())
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			ImGui::SameLine(w - inputWidth - iconOffset + isPicked ? hintOffset : 0.f);
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::Selection);
			ImGui::TextUnformatted(SW_ICON_FOLDER);
			ImGui::PopStyleColor();

			ImGui::SameLine(w - inputWidth - iconOffset + isPicked ? 0.f : hintOffset);
			ImGui::TextUnformatted(tag.c_str());

			return modified;
		}

		bool DrawFilePicker(std::filesystem::path* path, const std::filesystem::path& relative, const std::initializer_list<FileDialogFilterItem> filters)
		{
			const f32 posx = ImGui::GetCursorPosX();
			const f32 framePaddingY = ImGui::GetStyle().FramePadding.y;
			constexpr f32 bw = 28.f;
			constexpr f32 buttonOffset = 11.f;
			constexpr f32 iconOffset = 6.f;
			constexpr f32 hintOffset = 28.f;

			bool modified = false;
			bool isPicked = !path->empty();

			std::string tag = "None";

			const f32 w = ImGui::GetContentRegionAvail().x;
			f32 inputWidth = 0.0f;

			{
				GUI2::ScopedStyle FrameRounding(ImGuiStyleVar_FrameRounding, 3.0f);
				GUI2::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(28.0f, framePaddingY));

				if (ImGui::Button("##file_picker", { isPicked ? w - bw : w , ImGui::GetFrameHeight() })) {
					std::filesystem::path pickedPath = FileSystem::OpenFileDialog(filters);

					if (!pickedPath.empty()) {
						*path = std::filesystem::relative(pickedPath, relative);
						modified = true;
					}
				}
				inputWidth = ImGui::GetItemRectSize().x;

				if (ImGui::IsItemHovered() && isPicked) {
					std::filesystem::path fullPath = relative / *path;

					ImGui::BeginTooltip();
					ImGui::TextUnformatted(fullPath.string().c_str());
					ImGui::EndTooltip();
				}

				Components::ItemActivityOutline();
			}

			if (isPicked) {
				tag = path->string();

				ImGui::SameLine(w - bw + buttonOffset);
				ImGui::SetNextItemAllowOverlap();

				GUI2::ScopedColor Border(ImGuiCol_Border, ImVec4{ 1.f, 1.f , 1.f , 0.f });
				GUI2::ScopedColor Button(ImGuiCol_Button, ImVec4{ 1.f, 1.f , 1.f , 0.f });

				if (ImGui::Button(SW_ICON_CLOSE_OCTAGON, ImVec2{ bw, ImGui::GetFrameHeight() })) {
					path->clear();
					modified = true;
				}

				if (GUI2::IsItemHovered())
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			ImGui::SameLine(w - inputWidth - iconOffset + isPicked ? hintOffset : 0.f);
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::Selection);
			ImGui::TextUnformatted(SW_ICON_FILE);
			ImGui::PopStyleColor();

			ImGui::SameLine(w - inputWidth - iconOffset + isPicked ? 0.f : hintOffset);
			ImGui::TextUnformatted(tag.c_str());

			return modified;
		}

        bool Vector2Table(std::vector<glm::vec2>* vector, f32 defaultValue /*= 0.f*/)
		{
			u64 pointsCount = vector->size();
			bool modified = false;

			GUI2::Components::ScalarInput<u64>(&pointsCount, 1u, 1u);

			if (pointsCount > vector->size()) {
				vector->emplace_back(defaultValue, defaultValue);
				modified = true;
			}
			else if (pointsCount < vector->size()) {
				vector->pop_back();
				modified = true;
			}

			constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit
				| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

			int removeAt = -1;

			if (ImGui::BeginTable("##table_vector2", 2, flags)) {
				ImGui::TableSetupColumn("Vector");
				ImGui::TableSetupColumn("Action");
				ImGui::TableHeadersRow();

				for (int i = 0; i < (int)vector->size(); i++) {
					glm::vec2& point = vector->at(i);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					const std::string vectorId = "##table_vector2_element" + i;

					ImGui::PushID(vectorId.c_str());
					GUI2::Components::Vector2Input(&point, defaultValue);

					ImGui::TableNextColumn();

					if (ImGui::Button(SW_ICON_CLOSE_OCTAGON, { ImGui::GetFrameHeight(), ImGui::GetFrameHeight() }))
						removeAt = i;

					ImGui::PopID();
				}

				ImGui::EndTable();
			}

			if (removeAt > -1) {
				vector->erase(vector->begin() + removeAt);
				modified = true;
			}

			return modified;
		}

		bool Vector3Table(std::vector<glm::vec3>* vector, f32 defaultValue /*= 0.f*/)
		{
			u64 pointsCount = vector->size();
			bool modified = false;

			GUI2::Components::ScalarInput<u64>(&pointsCount, 1u, 1u);

			if (pointsCount > vector->size()) {
				vector->emplace_back(defaultValue, defaultValue, defaultValue);
				modified = true;
			}
			else if (pointsCount < vector->size()) {
				vector->pop_back();
				modified = true;
			}

			constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit
				| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

			int removeAt = -1;

			if (ImGui::BeginTable("##table_vector3", 2, flags)) {
				ImGui::TableSetupColumn("Vector");
				ImGui::TableSetupColumn("Action");
				ImGui::TableHeadersRow();

				for (int i = 0; i < (int)vector->size(); i++) {
					glm::vec3& point = vector->at(i);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					const std::string vectorId = "##table_vector3_element" + i;

					ImGui::PushID(vectorId.c_str());
					GUI2::Components::Vector3Input(&point, defaultValue);

					ImGui::TableNextColumn();

					if (ImGui::Button(SW_ICON_CLOSE_OCTAGON, { ImGui::GetFrameHeight(), ImGui::GetFrameHeight() }))
						removeAt = i;

					ImGui::PopID();
				}

				ImGui::EndTable();
			}

			if (removeAt > -1) {
				vector->erase(vector->begin() + removeAt);
				modified = true;
			}

			return modified;
		}

	}

	namespace Properties {

		bool CheckboxProperty(bool* value, const char* label, const char* tooltip /*= nullptr*/, bool center /*= true*/)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::Checkbox(value, center);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool ToggleProperty(bool* value, const char* label, const char* tooltip /*= nullptr*/, bool center /*= true*/)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::Toggle(value, center);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool ButtonProperty(
			const char* icon, const char* label, const char* tooltip /*= nullptr*/, glm::vec2 size /*= glm::vec2(40.0f)*/
		) {
			bool modified = false;

			Properties::BeginPropertyGrid(label, tooltip);

			GUI2::MoveMousePosX(ImGui::GetColumnWidth() / 2.0f - ImGui::GetStyle().FramePadding.x - 7.5f);

			modified = ImGui::Button(icon, { size.x, size.y });

			Properties::EndPropertyGrid();

			return modified;
		}

		bool ToggleButtonProperty(
			bool* value, const char* label, const char* tooltip /*= nullptr*/, const char* whenOnLabel /*= nullptr*/, const char* whenOffLabel /*= nullptr*/,
			bool center /*= true*/, ImGuiButtonFlags buttonFlags /*= ImGuiButtonFlags_None */
		) {
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::ToggleButton(value, whenOnLabel, whenOffLabel, center, buttonFlags);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool Vector2InputProperty(
			glm::vec2* vector, const char* label, const char* tooltip /*= nullptr*/, f32 resetValue /*= 0.f*/,
			f32 min /*= -FLT_MAX*/, f32 max /*= FLT_MAX*/, const std::string& format /*= "%.2f" */
		) {
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::Vector2Input(vector, resetValue, min, max, format);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool Vector3InputProperty(
			glm::vec3* vector, const char* label, const char* tooltip /*= nullptr*/, f32 resetValue /*= 0.f*/,
			f32 min /*= -FLT_MAX*/, f32 max /*= FLT_MAX*/, const std::string& format /*= "%.2f" */
		) {
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::Vector3Input(vector, resetValue, min, max, format);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool Vector3ColorPickerProperty(glm::vec3* vector, const char* label, const char* tooltip /*= nullptr*/, ImGuiColorEditFlags flags /*= ImGuiColorEditFlags_None */)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::Vector3ColorPicker(vector, flags);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool Vector4ColorPickerProperty(
			glm::vec4* vector, const char* label, const char* tooltip /*= nullptr*/,
			ImGuiColorEditFlags flags /*= ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf */
		) {
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::Vector4ColorPicker(vector, flags);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool EntityDropdownProperty(u64* ID, Scene* scene, const char* label, const char* tooltip /*= nullptr*/)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Widgets::EntityDropdown(ID, scene);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool DrawFolderPickerProperty(
			std::filesystem::path* path, const std::filesystem::path& relative,
			const char* label /*= "Folder"*/, const char* tooltip /*= nullptr */
		) {
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Widgets::DrawFolderPicker(path, relative);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool DrawFilePickerProperty(
			std::filesystem::path* path, const std::filesystem::path& relative,
			const std::initializer_list<FileDialogFilterItem> filters, const char* label /*= "File"*/, const char* tooltip /*= nullptr */
		) {
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Widgets::DrawFilePicker(path, relative, filters);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool Vector2TableProperty(
			std::vector<glm::vec2>* vector, const char* label /*= "File"*/, const char* tooltip /*= nullptr*/,
			f32 defaultValue /*= 0.f */
		) {
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Widgets::Vector2Table(vector, defaultValue);

			Properties::EndPropertyGrid();

			return modified;
		}

		bool Vector3TableProperty(
			std::vector<glm::vec3>* vector, const char* label /*= "File"*/, const char* tooltip /*= nullptr*/,
			f32 defaultValue /*= 0.f */
		) {
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Widgets::Vector3Table(vector, defaultValue);

			Properties::EndPropertyGrid();

			return modified;
		}

		void DrawImagePartProperty(
			Texture2D* wholeImage, const char* label, const char* tooltip /*= nullptr*/,
			glm::vec2 offset /*= glm::vec2(0.0f)*/, glm::vec2 size /*= glm::vec2(0.0f)*/, glm::vec4 tint /*= glm::vec4(1.0f)*/,
			bool showBorder /*= false */
		) {
			f32 width = (f32)wholeImage->GetWidth();
			f32 height = (f32)wholeImage->GetHeight();

			GUI2::Properties::BeginPropertyGrid(label, tooltip);

			ImVec2 uv0 = ImVec2(offset.x / width, (height - offset.y) / height);
			ImVec2 uv1 = ImVec2((offset.x + size.x) / width, (height - offset.y - (size.y)) / height);

			f32 originalImageWidth = size.x;
			f32 originalImageHeight = size.y;

			ImVec2 space = ImGui::GetContentRegionAvail();
			ImVec2 imagePartSize = ImVec2(originalImageWidth, originalImageHeight);

			// if image is smaller than the space, scale it up
			// this nasty way, because of variations and flickering of ImGui::GetContentRegionAvail() at certain column sizes.
			if (originalImageHeight < space.x) {
				f32 scale = std::floor(space.x / originalImageWidth);
				imagePartSize = ImVec2(originalImageWidth * scale, originalImageHeight * scale);
			}

			const ImVec4 borderCol = showBorder ?
				ImGui::ColorConvertU32ToFloat4(GUI::Theme::TextBrighter) : ImVec4(0.f, 0.f, 0.f, 0.f);

			ImGui::Image(
				GUI2::GetTextureID(wholeImage->GetTexHandle()), imagePartSize, uv0, uv1, { tint.r, tint.g, tint.b, tint.a }, borderCol
			);

			GUI2::Properties::EndPropertyGrid();
		}

	}

}
