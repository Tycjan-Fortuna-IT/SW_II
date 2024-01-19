/**
 * @file GUI.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.2
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <imgui_internal.h>

#include "Core/OpenGL/Texture2D.hpp"
#include "Core/Math/Vector2.hpp"
#include "Colors.hpp"

namespace SW::GUI {

	/**
	 * @brief Utility helper class to make use of the scope mechanic
	 *		  so that we easily can avoid many calls to Push/Pop StyleVar.
	 *		  This way more advanced UI code doesn't get so clustered.
	 */
	class ScopedStyle final
	{
	public:
		ScopedStyle(const ScopedStyle&) = delete;
		ScopedStyle(const ScopedStyle&&) = delete;
		ScopedStyle& operator=(const ScopedStyle&) = delete;
		ScopedStyle& operator=(const ScopedStyle&&) = delete;

		template<typename T>
		ScopedStyle(ImGuiStyleVar styleVar, T value) { ImGui::PushStyleVar(styleVar, value); }

		~ScopedStyle() { ImGui::PopStyleVar(); }
	};

	/**
	 * @brief Utility helper class to make use of the scope mechanic
	 *		  so that we easily can avoid many calls to Push/Pop StyleColor.
	 *		  This way more advanced UI code doesn't get so clustered.
	 */
	class ScopedColor final
	{
	public:
		ScopedColor(const ScopedColor&) = delete;
		ScopedColor(const ScopedColor&&) = delete;
		ScopedColor& operator=(const ScopedColor&) = delete;
		ScopedColor& operator=(const ScopedColor&&) = delete;

		template<typename T>
		ScopedColor(ImGuiCol colourId, T colour) { ImGui::PushStyleColor(colourId, ImColor(colour).Value); }

		~ScopedColor() { ImGui::PopStyleColor(); }
	};

	/**
	 * @brief Utility helper class to make use of the scope mechanic
	 *		  so that we easily can avoid many calls to Push/Pop Font.
	 *		  This way more advanced UI code doesn't get so clustered.
	 */
	class ScopedFont final
	{
	public:
		ScopedFont(const ScopedFont&) = delete;
		ScopedFont(const ScopedFont&&) = delete;
		ScopedFont& operator=(const ScopedFont&) = delete;
		ScopedFont& operator=(const ScopedFont&&) = delete;

		ScopedFont(ImFont* font) { ImGui::PushFont(font); }

		~ScopedFont() { ImGui::PopFont(); }
	};

	/**
	 * @brief Utility helper class to make use of the scope mechanic
	 *		  so that we easily can avoid many calls to Push/Pop ID.
	 *		  This way more advanced UI code doesn't get so clustered.
	 */
	class ScopedID
	{
	public:
		ScopedID(const ScopedID&) = delete;
		ScopedID(const ScopedID&&) = delete;
		ScopedID& operator=(const ScopedID&) = delete;
		ScopedID& operator=(const ScopedID&&) = delete;

		template<typename T>
		ScopedID(T id) { ImGui::PushID(id); }

		~ScopedID() { ImGui::PopID(); }
	};

	/**
	 * @brief Casts Texture2D to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of greater size
	 * 
	 * @param texture Texture2D
	 * @return ImTextureID 
	 */
	static ImTextureID GetTextureID(const Texture2D& texture)
	{
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture.GetHandle()));
	}

	/**
	 * @brief Casts Texture2D to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of greater size
	 * 
	 * @param texture Texture2D
	 * @return ImTextureID 
	 */
	static ImTextureID GetTextureID(const Texture2D* texture)
	{
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture->GetHandle()));
	}

	/**
	 * @brief Casts u32 to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of greater size
	 * 
	 * @param rendererID u32
	 * @return ImTextureID 
	 */
	static ImTextureID GetTextureID(u32 rendererID)
	{
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(rendererID));
	}

	static Vector2<f32> GetIconButtonSize(const char* icon, const char* label)
	{
		float lineHeight = ImGui::GetTextLineHeight();
		ImVec2 padding = ImGui::GetStyle().FramePadding;

		float width = ImGui::CalcTextSize(icon).x;
		width += ImGui::CalcTextSize(label).x;
		width += padding.x * 2.0f;

		return { width, lineHeight + padding.y * 2.0f };
	}

	/**
	 * @brief Get the rectangle that the current item occupies.
	 * 
	 * @return ImRect 
	 */
	static ImRect GetItemRect()
	{
		return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}

	/**
	 * @brief Expands the rectangle by the given amount.
	 * 
	 * @param rect The rectangle to expand.
	 * @param x X-axis amount to expand by.
	 * @param y Y-axis amount to expand by.
	 * @return ImRect The new rectangle, expanded by the specified amount.
	 */
	static ImRect RectExpanded(const ImRect& rect, f32 x, f32 y)
	{
		ImRect result = rect;
		result.Min.x -= x;
		result.Min.y -= y;
		result.Max.x += x;
		result.Max.y += y;

		return result;
	}

	/**
	 * @brief Creates a new rectangle by offsetting an existing rectangle by a certain amount.
	 * 
	 * @param rect The original rectangle to offset.
	 * @param x The amount to offset the rectangle in the x direction.
	 * @param y The amount to offset the rectangle in the y direction.
	 * @return ImRect The new rectangle, offset from the original by the specified amount.
	 */
	static ImRect RectOffset(const ImRect& rect, f32 x, f32 y)
	{
		ImRect result = rect;
		result.Min.x += x;
		result.Min.y += y;
		result.Max.x += x;
		result.Max.y += y;

		return result;
	}

	/**
	 * @brief Displays a button with an image. (centered image inside button without any text)
	 *
	 * @param texture Texture to be displayed.
	 * @param size Size of the button.
	 * @param tintHovered Color of the button's icon when hovered.
	 * @return bool True if the button was pressed, false otherwise.
	 */
	static bool ImageButton(const Texture2D& texture, const Vector2<f32>& size, ImU32 tintHovered = Color::DarkGray)
	{
		const f32 padding = (size.y - (f32)texture.GetHeight()) / 2.0f;

		if (ImGui::InvisibleButton(std::to_string(texture.GetHandle()).c_str(), ImVec2(size.x, size.y)))
			return true;

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		const ImRect rect = GUI::RectExpanded(GUI::GetItemRect(), -padding, -padding);

		const ImTextureID ID = GUI::GetTextureID(texture);

		if (ImGui::IsItemHovered())
			drawList->AddImage(ID, rect.Min, rect.Max, ImVec2(0, 0), ImVec2(1, 1), tintHovered);
		else
			drawList->AddImage(ID, rect.Min, rect.Max, ImVec2(0, 0), ImVec2(1, 1));

		return false;
	}


	/**
	* @brief Displays a button with an custom text.
	*
	* @param label Button's label. Must be a fmt formattable string literal. e.g. "Hello, {}!" (when want to use args).
	* @param size Size of the button.
	* @param args The arguments to the message to be formatted.
	* @return bool True if the button was pressed, false otherwise.
	*/
	template <typename... Args>
	static bool Button(const char* label, const Vector2<f32>& size, Args&& ...args)
	{
		const std::string formatlbl = fmt::vformat(label, fmt::make_format_args(std::forward<Args>(args)...));

		if (ImGui::Button(formatlbl.c_str(), { size.x, size.y })) {
			return true;
		}

		return false;
	}

	/**
	 * @brief Begins a menu bar and sets its bounding box. (custom menu bar inside specified rectangle bounding box)
	 *		  This function begins a new menu bar context in the GUI. All subsequent GUI elements will be rendered within this
	 *		  menu bar until EndMenuBar() is called. The menu bar is defined by the provided rectangle, which specifies its position
	 *		  and size. The top-left corner of the rectangle is the top-left corner of the menu bar, and the width and height of the
	 *		  rectangle define the size of the menu bar. This function returns a boolean value indicating whether the menu bar is
	 *		  currently open. 
	 * @warning This function must always be paired with a call to EndMenuBar(). Failure to do so will result in undefined behavior.
	 * 
	 * @param barRectangle The rectangle defining the position and size of the menu bar.
	 * @return bool True if the menu bar is open and ready for items to be added, false otherwise.
	 */
	static bool BeginMenuBar(const ImRect& barRectangle)
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

	/**
	 * @brief Ends a menu bar.
	 *		  This function ends the current menu bar context in the GUI.
	 *		  After this function is called, GUI elements will no longer be rendered within the menu bar.
	 *		  You should always call this function after you're done adding items to a menu bar.
	 * @warning This function must always be paired with a call to BeginMenuBar(). Failure to do so will result in undefined behavior.
	 */
	static void EndMenuBar()
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
