/**
 * @file GUI.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-01-28
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/OpenGL/Texture2D.hpp"
#include "Colors.hpp"
#include "Core/Utils/Utils.hpp"
#include "GUI/Icons.hpp"
#include "Appearance.hpp"
#include "Core/AssetManager.hpp"

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
	class ScopedID final
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
	 * @brief Utility helper class to create a fast search input field.
	 */
	class TextFilter final
	{
	public:
		TextFilter() = default;
		~TextFilter() = default;

		/**
		 * @brief Whether the user has typed anything into the search field.
		 * 
		 * @return true If the user has typed anything into the search field.
		 */
		bool IsDirty() const { return m_Buffer[0] != '\0'; }

		/**
		 * @brief Draws the search field.
		 * 
		 * @param label The label to display next to the search field.
		 * @param padding The padding to apply to the search field.
		 */
		void OnRender(const char* label, const ImVec2& padding = { 0.f, 6.f })
		{
			GUI::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, padding);
			GUI::ScopedColor TextColor(ImGuiCol_Text, Color::White);

			ImGui::InputTextWithHint("##search", "  " SW_ICON_MAGNIFY "  Search ... ", m_Buffer, sizeof(m_Buffer));
		}

		/**
		 * @brief Checks if the given string passes the filter.
		 * @note Only a partial match is required for the string to pass the filter.
		 * 		 We don't require a full match from the beginning of the string.
		 * 
		 * @param check The string to check.
		 * @return true If the string passes the filter.
		 */
		bool FilterPass(const std::string& check) const
		{
			if (!IsDirty())
				return true;

			if (check.find(m_Buffer) == std::string::npos)
				return false;

			return true;
		}

	private:
		char m_Buffer[256] = {};   /**< The buffer containing the search string. */
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

	static glm::vec2 GetIconButtonSize(const char* icon, const char* label)
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
	 * @brief Moves the mouse cursor to the specified position on the x-axis.
	 * 
	 * @param x The x position to move the mouse cursor to.
	 */
	static void MoveMousePosX(f32 x)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x);
	}

	/**
	 * @brief Moves the mouse cursor to the specified position on the y-axis.
	 * 
	 * @param y The y position to move the mouse cursor to.
	 */
	static void MoveMousePosY(f32 y)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + y);
	}

	/**
	 * @brief Draws a border around the specified rectangle.
	 * 
	 * @param rect The rectangle to draw a border around.
	 * @param thickness The thickness of the border.
	 * @param rounding The rounding of the border.
	 * @param offsetX The offset of the border on the x-axis.
	 * @param offsetY The offset of the border on the y-axis.
	 */
	static void DrawBorder(ImRect rect, float thickness = 1.0f, float rounding = 0.0f, float offsetX = 0.0f, float offsetY = 0.0f)
	{
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
	};

	/**
	 * @brief Displays a button with an image. (centered image inside button without any text)
	 *
	 * @param texture Texture to be displayed.
	 * @param size Size of the button.
	 * @param tintHovered Color of the button's icon when hovered.
	 * @return bool True if the button was pressed, false otherwise.
	 */
	static bool ImageButton(const Texture2D& texture, const glm::vec2& size, ImU32 tintHovered = Color::DarkGray)
	{
		const f32 padding = (size.y - (f32)texture.GetHeight()) / 2.0f;

		if (ImGui::InvisibleButton(std::to_string(texture.GetHandle()).c_str(), ImVec2(size.x, size.y)))
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

	/**
	* @brief Displays a button with an custom text.
	*
	* @param label Button's label. Must be a fmt formattable string literal. e.g. "Hello, {}!" (when want to use args).
	* @param size Size of the button.
	* @param args The arguments to the message to be formatted.
	* @return bool True if the button was pressed, false otherwise.
	*/
	template <typename... Args>
	static bool Button(const char* label, const glm::vec2& size, Args&& ...args)
	{
		const std::string formatlbl = fmt::vformat(label, fmt::make_format_args(std::forward<Args>(args)...));

		if (ImGui::Button(formatlbl.c_str(), { size.x, size.y })) {
			return true;
		}

		return false;
	}

	/**
	 * @brief Creates a toggle button with customizable appearance and behavior.
	 *
	 * @param label The label text displayed on the button.
	 * @param state The current state of the button (true for pressed, false for not pressed).
	 * @param size The size of the button (width and height). Use {0, 0} for automatic sizing.
	 * @param alpha The transparency of the button when not pressed (0.0f to 1.0f).
	 * @param pressedAlpha The transparency of the button when pressed (0.0f to 1.0f).
	 * @param buttonFlags Flags to customize the button's behavior (e.g., ImGuiButtonFlags_Repeat).
	 *
	 * @return true if the button was clicked, false otherwise.
	 */
	static bool ToggleButton(
		const char* label, bool state, ImVec2 size = { 0, 0 }, float alpha = 1.0f,
		float pressedAlpha = 1.0f, ImGuiButtonFlags buttonFlags = ImGuiButtonFlags_None
	) {
		if (state) {
			ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];

			color.w = pressedAlpha;
			ImGui::PushStyleColor(ImGuiCol_Button, color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
		} else {
			ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_Button];
			ImVec4 hoveredColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
			color.w = alpha;
			hoveredColor.w = pressedAlpha;
			ImGui::PushStyleColor(ImGuiCol_Button, color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
			color.w = pressedAlpha;
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
		}

		bool clicked = ImGui::ButtonEx(label, size, buttonFlags);

		ImGui::PopStyleColor(3);

		return clicked;
	}

	/**
	 * @brief Displays a nice modifiable 3 sliders for a Vector3.
	 * 
	 * @param vec The vector to display.
	 * @param label The label to display next to the vector.
	 * @param resetValue The value to reset the vector to when the reset button is pressed.
	 * @param labelWidth The width of the label.
	 * @param min The minimum value for the vector.
	 * @param max The maximum value for the vector.
	 * @param format The format to use when displaying the vector values.
	 */
	static void DisplayVector3Slider(
		glm::vec3& vec, const std::string& label, f32 resetValue, 
		f32 labelWidth = 80.0f, f32 min = -FLT_MAX, f32 max = FLT_MAX, const std::string& format = "%.3f"
	) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, labelWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		if (ImGui::Button("X"))
			vec.x = resetValue;
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &vec.x, 0.01f, min, max, format.c_str());
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
		if (ImGui::Button("Y"))
			vec.y = resetValue;
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &vec.y, 0.01f, min, max, format.c_str());

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
		if (ImGui::Button("Z"))
			vec.z = resetValue;
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &vec.z, 0.01f, min, max, format.c_str());

		ImGui::PopItemWidth();
		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	/**
	 * @brief Displays a nice color picker for a Vector4.
	 * 
	 * @param color The color to display.
	 * @param label The label to display next to the color.
	 */
	static void DisplayColorPicker(glm::vec4& color, const std::string& label)
	{
		ImGui::Columns(1);

		float widgetWidth = ImGui::GetContentRegionAvail().x - 40.f;

		ImGui::PushID("Color: ");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));

		ImGui::Text("%s", "Color");
		ImGui::SameLine();

		ImGui::SetNextItemWidth(widgetWidth);
		ImGui::ColorEdit4("##Color", glm::value_ptr(color), ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

		ImGui::PopStyleVar();
		ImGui::PopID();
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

	/**
	 * Begins a table for displaying properties in the GUI.
	 * @warning This function must always be paired with a call to EndProperties(). Failure to do so will result in undefined behavior.
	 * 
	 * @param name The name of the table.
	 * @param flags The flags to customize the table's behavior (optional).
	 */
	static void BeginProperties(
		const char* name,
		ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp |
								ImGuiTableFlags_BordersInner
	) {
		constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_PadOuterX;
		ImGui::BeginTable(name, 2, tableFlags | flags);
		ImGui::TableSetupColumn("PropertyName", 0, 0.5f);
		ImGui::TableSetupColumn("Property");
	}

	/**
	 * Ends a table for displaying properties in the GUI.
	 * @warning This function must always be paired with a call to BeginProperties(). Failure to do so will result in undefined behavior.
	 */
	static void EndProperties()
	{
		ImGui::EndTable();
	}

	/**
	 * @brief Begins a property grid section in the GUI. This function starts a new row in the ImGui table
	 * 		  and sets up the necessary layout for a property grid.
	 * @warning This function must always be paired with a call to EndPropertyGrid(). Failure to do so will result in undefined behavior.
	 *
	 * @param label The label for the property grid section.
	 * @param tooltip An optional tooltip to display when hovering over the label.
	 * @param rightAlignNextColumn Whether to right-align the next column in the table.
	 */
	static void BeginPropertyGrid(const char* label, const char* tooltip = nullptr, bool rightAlignNextColumn = true)
	{
		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		ImGui::PushID(label);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y * 0.5f);
		ImGui::TextUnformatted(label);
		if (tooltip && ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay)) {
			ImGui::BeginTooltip();
			ImGui::TextUnformatted(tooltip);
			ImGui::EndTooltip();
		}

		ImGui::TableNextColumn();

		if (rightAlignNextColumn)
			ImGui::SetNextItemWidth(-FLT_MIN);
	}

	/**
	 * @brief Ends a property grid section in the GUI. This function ends the current row in the ImGui table.
	 * @warning This function must always be paired with a call to BeginPropertyGrid(). Failure to do so will result in undefined behavior.
	 */
	static void EndPropertyGrid()
	{
		ImGui::PopID();
	}

	/**
	 * @brief Draws a single line text input property in the GUI.
	 * 
	 * This function is used to draw a single line text input property in the GUI.
	 * It takes a reference to a string, a label, and an optional tooltip as parameters.
	 * The text input field is displayed with the specified label and tooltip.
	 * The text entered by the user is stored in the provided string reference.
	 * 
	 * @tparam N The size of the buffer used to store the text input.
	 * @param text The reference to the string where the entered text will be stored.
	 * @param label The label to display for the text input field.
	 * @param tooltip The optional tooltip to display for the text input field.
	 */
	template <int N>
	static void DrawSingleLineTextInputProperty(
		std::string& text, const char* label, const char* tooltip = nullptr
	) {
		BeginPropertyGrid(label, tooltip, false);

		char buffer[N];

		memcpy(buffer, text.c_str(), std::min(sizeof(buffer), text.size() + 1));

		ImGui::SetNextItemWidth(ImGui::CalcItemWidth() * 1.5f);

		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			text = buffer;

		EndPropertyGrid();
	}

	/**
	 * @brief Draws a control property for a Vector2.
	 *
	 * This function is used to draw a control property for a Vector2, allowing the user to modify its values.
	 * The control property consists of three input fields for the X and Y components of the vector.
	 * Additionally, buttons are provided to reset each component to a specified value.
	 *
	 * @param vector The Vector2 to be modified by the control property.
	 * @param label The label to be displayed for the control property.
	 * @param tooltip An optional tooltip to be displayed when hovering over the control property.
	 * @param resetValue The value to which each component of the vector will be reset when the corresponding reset button is pressed.
	 * @param min The minimum value allowed for each component of the vector.
	 * @param max The maximum value allowed for each component of the vector.
	 * @param format The format string used to display the values of the vector components.
	 */
	static void DrawVector2ControlProperty(
		glm::vec2& vector, const char* label, const char* tooltip = nullptr, f32 resetValue = 0.f,
		f32 min = -FLT_MAX, f32 max = FLT_MAX, const std::string& format = "%.2f"
	) {
		BeginPropertyGrid(label, tooltip, false);

		ImGuiIO& io = ImGui::GetIO();

		ImFont* boldFont = GUI::Appearance::GetFonts().DefaultBoldFont;

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth() + ImGui::GetStyle().ItemSpacing.x * 2.f + 1.f);

		float frameHeight = ImGui::GetFrameHeight();
		ImVec2 buttonSize = { frameHeight + 3.0f, frameHeight };

		ImVec2 innerItemSpacing = ImGui::GetStyle().ItemInnerSpacing;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, innerItemSpacing);

		// X
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				vector.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &vector.x, 0.05f, min, max, format.c_str());
			ImGui::PopItemWidth();
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
			if (ImGui::Button("Y", buttonSize))
				vector.y = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &vector.y, 0.05f, min, max, format.c_str());
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();
		}

		ImGui::PopStyleVar();

		EndPropertyGrid();
	}

	/**
	 * @brief Draws a control property for a Vector3.
	 *
	 * This function is used to draw a control property for a Vector3, allowing the user to modify its values.
	 * The control property consists of three input fields for the X, Y, and Z components of the vector.
	 * Additionally, buttons are provided to reset each component to a specified value.
	 *
	 * @param vector The Vector3 to be modified by the control property.
	 * @param label The label to be displayed for the control property.
	 * @param tooltip An optional tooltip to be displayed when hovering over the control property.
	 * @param resetValue The value to which each component of the vector will be reset when the corresponding reset button is pressed.
	 * @param min The minimum value allowed for each component of the vector.
	 * @param max The maximum value allowed for each component of the vector.
	 * @param format The format string used to display the values of the vector components.
	 */
	static void DrawVector3ControlProperty(
		glm::vec3& vector, const char* label, const char* tooltip = nullptr, f32 resetValue = 0.f,
		f32 min = -FLT_MAX, f32 max = FLT_MAX, const std::string& format = "%.2f"
	) {
		BeginPropertyGrid(label, tooltip, false);

		ImGuiIO& io = ImGui::GetIO();
		
		ImFont* boldFont = GUI::Appearance::GetFonts().DefaultBoldFont;

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() + ImGui::GetStyle().ItemSpacing.x - 4.f);

		float frameHeight = ImGui::GetFrameHeight();
		ImVec2 buttonSize = { frameHeight + 3.0f, frameHeight };

		ImVec2 innerItemSpacing = ImGui::GetStyle().ItemInnerSpacing;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, innerItemSpacing);

		// X
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				vector.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &vector.x, 0.05f, min, max, format.c_str());
			ImGui::PopItemWidth();
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
			if (ImGui::Button("Y", buttonSize))
				vector.y = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &vector.y, 0.05f, min, max, format.c_str());
			ImGui::PopItemWidth();
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
			if (ImGui::Button("Z", buttonSize))
				vector.z = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &vector.z, 0.05f, min, max, format.c_str());
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();
		}

		ImGui::PopStyleVar();

		EndPropertyGrid();
	}

	/**
	 * Draws a color picker property for a Vector4.
	 *
	 * @param vector The Vector4 to edit.
	 * @param label The label for the color picker.
	 * @param tooltip The tooltip for the color picker (optional).
	 */
	static void DrawVector4ColorPickerProperty(
		glm::vec4& vector, const char* label, const char* tooltip = nullptr
	) {
		BeginPropertyGrid(label, tooltip, true);

		ImGui::ColorEdit4("##Color", glm::value_ptr(vector), ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

		EndPropertyGrid();
	}

	/**
	 * @brief Draws a texture property in the GUI.
	 * 
	 * This function displays a texture property in the GUI, allowing the user to select a texture from the content browser.
	 * The selected texture is assigned to the provided texture pointer.
	 * 
	 * @param texture A pointer to a pointer to a Texture2D object. This is the texture property to be displayed and modified.
	 * @param label The label to be displayed for the texture property.
	 * @param tooltip (optional) The tooltip to be displayed when hovering over the texture property.
	 */
	static void DrawTextureProperty(Texture2D** texture, const char* label, const char* tooltip = nullptr)
	{
		BeginPropertyGrid(label, tooltip);

		bool changed = false;

		float frameHeight = ImGui::GetFrameHeight();
		const float buttonSize = frameHeight * 3.0f;
		const ImVec2 xButtonSize = { buttonSize / 4.0f, buttonSize };
		const float tooltipSize = frameHeight * 11.0f;

		ImGui::SetCursorPos({ ImGui::GetContentRegionMax().x - buttonSize - xButtonSize.x, ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.35f, 0.35f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f });

		Texture2D* textureCopy = *texture;

		if (!textureCopy)
			textureCopy = AssetManager::GetBlackTexture();

		u32 textureId = textureCopy->GetHandle();

		ImGui::ImageButton(GUI::GetTextureID(textureId), { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 }, 0);

		if (*texture && ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay)) {
			ImGui::BeginTooltip();
			ImGui::TextUnformatted((*texture)->GetPath().c_str());
			ImGui::Spacing();
			ImGui::Image(GUI::GetTextureID(textureId), { tooltipSize, tooltipSize }, { 0, 1 }, { 1, 0 });
			ImGui::EndTooltip();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				*texture = AssetManager::GetTexture2D(static_cast<char*>(payload->Data)); // todo perform validation of the file
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });

		if (ImGui::Button("x", xButtonSize)) {
			*texture = nullptr; // TODO: maybe remove unused textures from memory
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();

		EndPropertyGrid();
	}

	/**
	 * @brief Draws a boolean property in the GUI.
	 *
	 * @param value The boolean value to represent with the checkbox.
	 * @param label The label text to display next to the checkbox.
	 * @param tooltip (optional) Additional information to display as a tooltip.
	 */
	static bool DrawBooleanProperty(bool& value, const char* label, const char* tooltip = nullptr)
	{
		bool changed = false;

		BeginPropertyGrid(label, tooltip);

		GUI::MoveMousePosX(ImGui::GetColumnWidth() / 2.0f - ImGui::GetStyle().FramePadding.x - 15.f);
		
		changed = ImGui::Checkbox("##property_checkbox", &value);

		EndPropertyGrid();

		return changed;
	}

	/**
	 * @brief Draws a readonly text property in the property grid.
	 * 
	 * @param value The value to display as text.
	 * @param label The label for the property.
	 * @param tooltip (optional) The tooltip for the property.
	 */
	static void DrawReadonlyTextProperty(const char* value, const char* label, const char* tooltip = nullptr)
	{
		BeginPropertyGrid(label, tooltip);

		ImGui::TextUnformatted(value);

		EndPropertyGrid();
	}

	/**
	 * @brief Draws an integral property in the GUI.
	 * 
	 * This function is used to draw an integral property in the GUI using ImGui. It provides a slider or a drag input depending on the range of values specified.
	 * 
	 * @tparam T The integral type of the property.
	 * @param value The reference to the integral property value.
	 * @param label The label for the property.
	 * @param tooltip The tooltip for the property (optional).
	 * @param min The minimum value for the property (optional).
	 * @param max The maximum value for the property (optional).
	 */
	template<std::integral T>
	static void DrawIntegralProperty(
		T& value, const char* label, const char* tooltip = nullptr, T min = 0, T max = 0
	) {
		BeginPropertyGrid(label, tooltip);

		int dataType = ImGuiDataType_S32;

		if constexpr (std::is_signed_v<T>) {
			if constexpr (sizeof(T) == 1)
				dataType = ImGuiDataType_S8;
			else if constexpr (sizeof(T) == 2)
				dataType = ImGuiDataType_S16;
			else if constexpr (sizeof(T) == 4)
				dataType = ImGuiDataType_S32;
			else if constexpr (sizeof(T) == 8)
				dataType = ImGuiDataType_S64;
		} else {
			if constexpr (sizeof(T) == 1)
				dataType = ImGuiDataType_U8;
			else if constexpr (sizeof(T) == 2)
				dataType = ImGuiDataType_U16;
			else if constexpr (sizeof(T) == 4)
				dataType = ImGuiDataType_U32;
			else if constexpr (sizeof(T) == 8)
				dataType = ImGuiDataType_U64;
		}

		if (max > min)
			ImGui::SliderScalar("##property_integral", dataType, &value, &min, &max);
		else
			ImGui::DragScalar("##property_integral", dataType, &value);

		EndPropertyGrid();
	}

	/**
	 * @brief Draws a floating-point property in the GUI.
	 *
	 * This function is used to draw a floating-point property in the GUI. It provides options for setting the value, label, tooltip, minimum and maximum values, delta, and format.
	 *
	 * @tparam T The type of the floating-point value.
	 * @param value The reference to the floating-point value to be displayed and modified.
	 * @param label The label for the property.
	 * @param tooltip The tooltip for the property (optional).
	 * @param min The minimum value for the property (optional).
	 * @param max The maximum value for the property (optional).
	 * @param delta The delta value for the property (optional).
	 * @param format The format string for displaying the value (optional).
	 */
	template<std::floating_point T>
	static void DrawFloatingPointProperty(
		T& value, const char* label, const char* tooltip = nullptr, T min = 0, T max = 0, float delta = 0.1f, const char* format = "%.3f"
	) {
		BeginPropertyGrid(label, tooltip);
		
		int dataType = ImGuiDataType_Float;

		if constexpr (sizeof(T) == 8)
			dataType = ImGuiDataType_Double;			

		if (max > min)
			ImGui::SliderScalar("##property_floating_point", dataType, &value, &min, &max, format);
		else
			ImGui::DragScalar("##property_floating_point", dataType, &value, delta, nullptr, nullptr, format);

		EndPropertyGrid();
	}

	/**
	 * @brief A struct representing a selectable option.
	 * 
	 * This struct is used to define an option that can be selected in a GUI.
	 * It contains a label and a value of type T.
	 * 
	 * @tparam T The type of the value associated with the option.
	 */
	template <typename T>
	struct SelectOption final
	{
		std::string Label = "No label";
		T value = 0;
	};

	/**
	 * @brief Draws a selectable property in the GUI.
	 * 
	 * This function displays a selectable property in the GUI, allowing the user to choose from a list of options.
	 * The selected value is stored in the provided reference variable.
	 * 
	 * @tparam T The type of the property value.
	 * @param value The reference to the property value.
	 * @param options The list of selectable options.
	 * @param label The label for the property.
	 * @param tooltip The tooltip for the property (optional).
	 */
	template<typename T>
	static void DrawSelectableProperty(
		T& value, std::initializer_list<SelectOption<T>> options, const char* label, const char* tooltip = nullptr
	) {
		BeginPropertyGrid(label, tooltip);

		std::string chosenName = "";

		for (SelectOption<T> option : options) {
			if (value == option.value) {
				chosenName = option.Label;
			}
		}

		if (ImGui::BeginCombo("##property_selectable", chosenName.data())) {
			for (SelectOption<T> option : options) {
				const bool isSelected = value == option.value;

				if (ImGui::Selectable(option.Label.c_str(), isSelected)) {
					value = option.value;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		EndPropertyGrid();
	}

	/**
	 * @brief Renders clipped text on the GUI.
	 *
	 * This function performs CPU side clipping for a single clipped element to avoid using scissor state.
	 * It calculates the position and size of the text based on the given parameters and aligns it within the specified bounds.
	 * The rendered text is added to the draw list.
	 *
	 * @param draw_list Pointer to the ImDrawList object used for rendering.
	 * @param pos_min The minimum position of the text bounding box.
	 * @param pos_max The maximum position of the text bounding box.
	 * @param text The text to render.
	 * @param text_display_end Optional pointer to the end of the text to render.
	 * @param text_size_if_known Optional pointer to the known size of the text.
	 * @param align The alignment of the text within the bounding box.
	 * @param clip_rect Optional pointer to the clipping rectangle.
	 * @param wrap_width The width at which the text should wrap.
	 */
	static void ClippedText(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_display_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect, float wrap_width)
	{
		// Perform CPU side clipping for single clipped element to avoid using scissor state
		ImVec2 pos = pos_min;
		const ImVec2 text_size = text_size_if_known ? *text_size_if_known : ImGui::CalcTextSize(text, text_display_end, false, wrap_width);

		const ImVec2* clip_min = clip_rect ? &clip_rect->Min : &pos_min;
		const ImVec2* clip_max = clip_rect ? &clip_rect->Max : &pos_max;

		// Align whole block. We should defer that to the better rendering function when we'll have support for individual line alignment.
		if (align.x > 0.0f)
			pos.x = ImMax(pos.x, pos.x + (pos_max.x - pos.x - text_size.x) * align.x);

		if (align.y > 0.0f)
			pos.y = ImMax(pos.y, pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

		// Render
		ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
		draw_list->AddText(nullptr, 0.0f, pos, ImGui::GetColorU32(ImGuiCol_Text), text, text_display_end, wrap_width, &fine_clip_rect);
	}

	/**
	 * @brief Renders clipped text on the GUI.
	 *
	 * This function performs CPU side clipping for a single clipped element to avoid using scissor state.
	 * It calculates the position and size of the text based on the given parameters and aligns it within the specified bounds.
	 * The rendered text is added to the draw list.
	 *
	 * @param pos_min The minimum position of the text bounding box.
	 * @param pos_max The maximum position of the text bounding box.
	 * @param text The text to render.
	 * @param text_end Optional pointer to the end of the text to render.
	 * @param text_size_if_known Optional pointer to the known size of the text.
	 * @param align The alignment of the text within the bounding box.
	 * @param clip_rect Optional pointer to the clipping rectangle.
	 * @param wrap_width The width at which the text should wrap.
	 */
	static void ClippedText(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect, float wrap_width)
	{
		// Hide anything after a '##' string
		const char* text_display_end = ImGui::FindRenderedTextEnd(text, text_end);
		const int text_len = static_cast<int>(text_display_end - text);
		if (text_len == 0)
			return;

		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ClippedText(window->DrawList, pos_min, pos_max, text, text_display_end, text_size_if_known, align, clip_rect, wrap_width);
		if (g.LogEnabled)
			ImGui::LogRenderedText(&pos_min, text, text_display_end);
	}
}
