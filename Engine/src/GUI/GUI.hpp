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
	inline ImTextureID GetTextureID(const Texture2D& texture)
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
	inline ImTextureID GetTextureID(const Texture2D* texture)
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
	inline ImTextureID GetTextureID(u32 rendererID)
	{
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(rendererID));
	}

	/**
	 * @brief Get the rectangle that the current item occupies.
	 * 
	 * @return ImRect 
	 */
	inline ImRect GetItemRect()
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
	inline ImRect RectExpanded(const ImRect& rect, f32 x, f32 y)
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
	inline ImRect RectOffset(const ImRect& rect, f32 x, f32 y)
	{
		ImRect result = rect;
		result.Min.x += x;
		result.Min.y += y;
		result.Max.x += x;
		result.Max.y += y;

		return result;
	}

	/**
	 * @brief Displays a button with an icon.
	 * 
	 * @param texture Texture to be displayed.
	 * @param size Size of the button.
	 * @param tintHovered Colour of the button's icon when hovered.
	 * @param paddingX Padding on the X-axis.
	 * @param paddingY Padding on the Y-axis.
	 * @param onClick Callback function to be called when the button is clicked.
	 */
	template <typename Fn>
	void ImageButton(
		const Texture2D& texture, const Vector2<f32>& size, 
		ImU32 tintHovered, f32 paddingX, f32 paddingY,
		Fn onClick
	) {
		if (ImGui::InvisibleButton(std::to_string(texture.GetHandle()).c_str(), ImVec2(size.x, size.y)))
			onClick();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		const ImRect rect = GUI::RectExpanded(GUI::GetItemRect(), -paddingX, -paddingY);

		const ImTextureID ID = GUI::GetTextureID(texture);

		if (ImGui::IsItemHovered())
			drawList->AddImage(ID, rect.Min, rect.Max, ImVec2(0, 0), ImVec2(1, 1), tintHovered);
		else
			drawList->AddImage(ID, rect.Min, rect.Max, ImVec2(0, 0), ImVec2(1, 1));
	}

	/**
	 * @brief Displays a button with an icon.
	 * 
	 * @param texture Texture to be displayed.
	 * @param size Size of the button.
	 * @param tintHovered Colour of the button's icon when hovered.
	 * @param onClick Callback function to be called when the button is clicked.
	 */
	template <typename Fn>
	void ImageButton(
		const Texture2D& texture, const Vector2<f32>& size, ImU32 tintHovered, Fn onClick
	) {
		const f32 padding = (size.y - (f32)texture.GetHeight()) / 2.0f;

		ImageButton(texture, size, tintHovered, padding, padding, onClick);
	}

	/**
	 * @brief Displays a button with an icon.
	 * 
	 * @param texture Texture to be displayed.
	 * @param size Size of the button.
	 * @param onClick Callback function to be called when the button is clicked.
	 */
	template <typename Fn>
	void ImageButton(
		const Texture2D& texture, const Vector2<f32>& size, Fn onClick
	) {
		const f32 padding = (size.y - (f32)texture.GetHeight()) / 2.0f;

		ImageButton(texture, size, Color::DarkGray, padding, padding, onClick);
	}

	/**
	 * @brief Displays a button with an icon.
	 * 
	 * @param texture Texture to be displayed.
	 * @param size Size of the button.
	 * @param paddingX Padding on the X-axis.
	 * @param paddingY Padding on the Y-axis.
	 * @param onClick Callback function to be called when the button is clicked.
	 */
	template <typename Fn>
	void ImageButton(
		const Texture2D& texture, const Vector2<f32>& size,
		 f32 paddingX, f32 paddingY, Fn onClick
	) {
		ImageButton(texture, size, Color::DarkGray, paddingX, paddingY, onClick);
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
	bool BeginMenuBar(const ImRect& barRectangle);

	/**
	 * @brief Ends a menu bar.
	 *		  This function ends the current menu bar context in the GUI.
	 *		  After this function is called, GUI elements will no longer be rendered within the menu bar.
	 *		  You should always call this function after you're done adding items to a menu bar.
	 * @warning This function must always be paired with a call to BeginMenuBar(). Failure to do so will result in undefined behavior.
	 */
	void EndMenuBar();
	
}
