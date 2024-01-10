/**
 * @file GUI.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-10
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <imgui.h>

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
	class ScopedID {
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
	inline ImTextureID GetTextureID(const Texture2D& texture) {
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture.GetHandle()));
	}

	/**
	 * @brief Casts Texture2D to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of greater size
	 * 
	 * @param texture Texture2D
	 * @return ImTextureID 
	 */
	inline ImTextureID GetTextureID(const Texture2D* texture) {
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture->GetHandle()));
	}

	/**
	 * @brief Casts u32 to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of greater size
	 * 
	 * @param rendererID u32
	 * @return ImTextureID 
	 */
	inline ImTextureID GetTextureID(u32 rendererID) {
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(rendererID));
	}

	/**
	 * @brief Get the rectangle that the current item occupies.
	 * 
	 * @return ImRect 
	 */
	inline ImRect GetItemRect() {
		return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}

	/**
	 * @brief Expands the rectangle by the given amount.
	 * 
	 * @param rect The rectangle to expand.
	 * @param x X-axis amount to expand by.
	 * @param y Y-axis amount to expand by.
	 * @return ImRect 
	 */
	inline ImRect RectExpanded(const ImRect& rect, float x, float y) {
		ImRect result = rect;
		result.Min.x -= x;
		result.Min.y -= y;
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
		const float padding = (size.y - (f32)texture.GetHeight()) / 2.0f;

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
		const float padding = (size.y - (f32)texture.GetHeight()) / 2.0f;

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
}
