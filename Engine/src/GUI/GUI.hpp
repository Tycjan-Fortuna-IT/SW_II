/**
 * @file GUI.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.2.5
 * @date 2024-04-04
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/OpenGL/Texture2D.hpp"
#include "Colors.hpp"
#include "Core/Utils/Utils.hpp"
#include "GUI/Icons.hpp"
#include "Appearance.hpp"
#include "Core/Asset/AssetManager.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/OpenGL/Font.hpp"
#include "Core/Renderer/Renderer2D.hpp"
#include "Core/Utils/FileSystem.hpp"

namespace SW::GUI {

	/**
	 * @brief Utility helper class to make use of the scope mechanic
	 *		  so that we easily can avoid many calls to Push/Pop StyleVar.
	 *		  This way more advanced UI code doesn't get so clustered.
	 */
	class ScopedStyle final {
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
	class ScopedColor final {
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
	class ScopedFont final {
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
	class ScopedID final {
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
	class TextFilter final {
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

			ImGui::InputTextWithHint("##search", label, m_Buffer, sizeof(m_Buffer));
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
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture.GetTexHandle()));
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
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture->GetTexHandle()));
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
		f32 lineHeight = ImGui::GetTextLineHeight();
		ImVec2 padding = ImGui::GetStyle().FramePadding;

		f32 width = ImGui::CalcTextSize(icon).x;
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
	 * @brief Creates a dockspace with the specified name and options.
	 *
	 * This function creates a dockspace using the Dear ImGui library. The dockspace is created at the position of the main viewport,
	 * and its size is set to the size of the main viewport. The window flags for the dockspace can be customized.
	 *
	 * @param name The name of the dockspace. This is also used as the ID for the dockspace.
	 * @param fn The function to call to draw the contents of the dockspace. This function should return the height of the top of the dockspace.
	 * @param flags The window flags for the dockspace. These determine the behavior and appearance of the dockspace. Default is a combination of flags that result in a dockspace with no title bar, no collapse button, no resize handles, and no move functionality.
	 */
	template <typename T>
	static void CreateDockspace(
		const char* name, T fn, ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
	|	ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
	) {
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigWindowsResizeFromEdges = io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 6.0f));
		ImGui::Begin("DockSpace Demo", nullptr, flags | ImGuiWindowFlags_NoDocking);
		ImGui::PopStyleVar(3);

		f32 topOffset = 0.0f;

		if constexpr (std::is_invocable_v<T>)
			topOffset = fn();

		ImGui::SetCursorPosY(topOffset + ImGui::GetCurrentWindow()->WindowPadding.y);

		ImGui::DockSpace(ImGui::GetID("MyDockspace"));

		ImGui::End();
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
	static void DrawBorder(ImRect rect, f32 thickness = 1.0f, f32 rounding = 0.0f, f32 offsetX = 0.0f, f32 offsetY = 0.0f)
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
		const char* label, bool state, ImVec2 size = { 0, 0 }, f32 alpha = 1.0f,
		f32 pressedAlpha = 1.0f, ImGuiButtonFlags buttonFlags = ImGuiButtonFlags_None
	) {
		if (state) {
			ImVec4 color = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];

			color.w = pressedAlpha;
			ImGui::PushStyleColor(ImGuiCol_Button, color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
		}
		else {
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

		f32 widgetWidth = ImGui::GetContentRegionAvail().x - 40.f;

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
	 * @tparam N The size of the buffer used to store the text input.
	 * @param text The reference to the string where the entered text will be stored.
	 * @param flags Additional ImGui input flags.
	 * 
	 * @return bool Whether something has changed
	 */
	template <int N = 256>
	static bool DrawSingleLineTextInput(
		std::string& text, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None
	) {
		char buffer[N];

		memcpy(buffer, text.c_str(), std::min(sizeof(buffer), text.size() + 1));

		if (ImGui::InputText("##Tag", buffer, sizeof(buffer), flags)) {
			text = buffer;

			return true;
		}
			
		return false;
	}

	/**
	 * @brief Draws a single line text input property in the GUI.
	 *
	 * @tparam N The size of the buffer used to store the text input.
	 * @param text The reference to the string where the entered text will be stored.
	 * @param label The label to display for the text input field.
	 * @param tooltip The optional tooltip to display for the text input field.
	 * 
	 * @return bool Whether something has changed
	 */
	template <int N = 256>
	static bool DrawSingleLineTextInputProperty(
		std::string& text, const char* label, const char* tooltip = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None
	) {
		bool changed = false;

		BeginPropertyGrid(label, tooltip, false);

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

		changed = DrawSingleLineTextInput<N>(text, flags);

		EndPropertyGrid();

		return changed;
	}

	/**
	 * @brief Draws a control for editing a glm::vec2 value in ImGui.
	 *
	 * This function creates a control in ImGui for editing a glm::vec2 value. The control consists of two input fields, one for the X component and one for the Y component of the vector. It also includes buttons for resetting the values to a specified reset value.
	 *
	 * @param vector The glm::vec2 value to be edited.
	 * @param resetValue The value to which the vector components will be reset when the reset button is pressed. Default is 0.0f.
	 * @param min The minimum value allowed for the vector components. Default is -FLT_MAX.
	 * @param max The maximum value allowed for the vector components. Default is FLT_MAX.
	 * @param format The format string used to display the vector components. Default is "%.2f".
	 */
	static bool DrawVector2Control(
		glm::vec2& vector, f32 resetValue = 0.f,
		f32 min = -FLT_MAX, f32 max = FLT_MAX, const std::string& format = "%.2f"
	) {
		bool changed = false;

		const ImGuiIO& io = ImGui::GetIO();

		ImFont* boldFont = GUI::Appearance::GetFonts().DefaultBoldFont;

		constexpr f32 spacingX = 8.0f;
		const ImVec2 size = ImGui::GetContentRegionAvail();

		GUI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
		GUI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

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
				vector.x = resetValue;
				changed = true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(inputItemWidth);
			if (ImGui::DragFloat("##X", &vector.x, 0.05f, min, max, format.c_str()))
				changed = true;
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
				vector.y = resetValue;
				changed = true;
			}
				
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(inputItemWidth);
			if (ImGui::DragFloat("##Y", &vector.y, 0.05f, min, max, format.c_str()))
				changed = true;
			ImGui::PopStyleVar();
		}

		return changed;
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
	static bool DrawVector2ControlProperty(
		glm::vec2& vector, const char* label, const char* tooltip = nullptr, f32 resetValue = 0.f,
		f32 min = -FLT_MAX, f32 max = FLT_MAX, const std::string& format = "%.2f"
	) {
		bool changed = false;

		BeginPropertyGrid(label, tooltip, false);

		changed = DrawVector2Control(vector, resetValue, min, max, format);

		EndPropertyGrid();

		return changed;
	}

	/**
	 * @brief Draws a control for a Vector3.
	 *
	 * This function is used to draw a control for a Vector3, allowing the user to modify its values.
	 * The control consists of three input fields for the X, Y, and Z components of the vector.
	 * Additionally, buttons are provided to reset each component to a specified value.
	 *
	 * @param vector The Vector3 to be modified by the control.
	 * @param resetValue The value to which each component of the vector will be reset when the corresponding reset button is pressed.
	 * @param min The minimum value allowed for each component of the vector.
	 * @param max The maximum value allowed for each component of the vector.
	 * @param format The format string used to display the values of the vector components.
	 */
	static bool DrawVector3Control(
		glm::vec3& vector, f32 resetValue = 0.f,
		f32 min = -FLT_MAX, f32 max = FLT_MAX, const std::string& format = "%.2f"
	) {
		bool changed = false;

		const ImGuiIO& io = ImGui::GetIO();

		ImFont* boldFont = GUI::Appearance::GetFonts().DefaultBoldFont;

		constexpr f32 spacingX = 8.0f;
		const ImVec2 size = ImGui::GetContentRegionAvail();

		GUI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
		GUI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });

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
				vector.x = resetValue;
				changed = true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(inputItemWidth);
			if (ImGui::DragFloat("##X", &vector.x, 0.05f, min, max, format.c_str()))
				changed = true;
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
				vector.y = resetValue;
				changed = true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(inputItemWidth);
			if (ImGui::DragFloat("##Y", &vector.y, 0.05f, min, max, format.c_str()))
				changed = true;
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
				vector.z = resetValue;
				changed = true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(inputItemWidth);
			if (ImGui::DragFloat("##Z", &vector.z, 0.05f, min, max, format.c_str()))
				changed = true;
			ImGui::PopStyleVar();
		}

		return changed;
	}

	/**
	 * @brief Draws a control property for a Vector3.
	 *
	 * This function is used to draw a control property for a Vector3, allowing the user to modify its values.
	 * The control property consists of three input fields for the X, Y and Z components of the vector.
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
	static bool DrawVector3ControlProperty(
		glm::vec3& vector, const char* label, const char* tooltip = nullptr, f32 resetValue = 0.f,
		f32 min = -FLT_MAX, f32 max = FLT_MAX, const std::string& format = "%.2f"
	) {
		bool changed = false;

		BeginPropertyGrid(label, tooltip, false);

		changed = DrawVector3Control(vector, resetValue, min, max, format);

		EndPropertyGrid();

		return changed;
	}

	/**
	 * Draws a color picker property for a Vector4.
	 *
	 * @param vector The Vector4 to edit.
	 * @param label The label for the color picker.
	 * @param tooltip The tooltip for the color picker (optional).
	 */
	static bool DrawVector4ColorPickerProperty(
		glm::vec4& vector, const char* label, const char* tooltip = nullptr
	) {
		bool changed = false;

		BeginPropertyGrid(label, tooltip, true);

		changed = ImGui::ColorEdit4("##Color", glm::value_ptr(vector), ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);

		EndPropertyGrid();
	
		return changed;
	}

	static void DrawVector2TableList(
		std::vector<glm::vec2>& vector, const char* label, const char* tooltip = nullptr, const glm::vec2& defaultValue = { 0.f, 0.f }
	) {
		u64 pointsCount = vector.size();
		u16 step = 1u;

		BeginPropertyGrid("Count", "Amount of max elements in a list", true);
		ImGui::InputScalar("##Count", ImGuiDataType_U64, &pointsCount, &step);
		EndPropertyGrid();

		if (pointsCount > vector.size()) {
			vector.push_back({ 0.f, 0.f });
		}
		else if (pointsCount < vector.size()) {
			vector.pop_back();
		}

		BeginPropertyGrid(label, tooltip, true);

		static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

		int removeAt = -1;

		if (ImGui::BeginTable("##list", 2, flags)) {
			int row = 0;
			for (glm::vec2& vec : vector) {
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				std::string id = "##list_element" + row;

				ImGui::PushID(id.c_str());
				DrawVector2Control(vec);
				ImGui::PopID();

				ImGui::TableNextColumn();

				std::string bid = "##list_button_element" + row;

				ImGui::PushID(bid.c_str());
				if (ImGui::Button("X", { 25.f, 25.f })) {
					removeAt = row;
				}
				ImGui::PopID();

				row++;
			}

			ImGui::EndTable();
		}

		if (removeAt > -1)
			vector.erase(vector.begin() + removeAt);

		EndPropertyGrid();
	}

	static bool DrawFolderPickerProperty(
		std::filesystem::path& path, const char* label, const char* tooltip = nullptr
	) {
		bool changed = false;
		bool isEmpty = path.empty();

		std::string tag = "none";

		if (!isEmpty)
			tag = path.string();

		GUI::BeginPropertyGrid(label, tooltip, true);

		ImVec2 region = ImGui::GetContentRegionAvail();
		region.x -= 20.0f;
		region.y = ImGui::GetFrameHeight();

		ImVec2 pos = ImGui::GetCursorPos();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_Button]);

		if (ImGui::Button("##font_dropdown_property", region)) {
			std::filesystem::path pickedPath = FileSystem::OpenFolderDialog(ProjectContext::Get()->GetAssetDirectory().parent_path().string().c_str());

			path = std::filesystem::relative(pickedPath, ProjectContext::Get()->GetAssetDirectory());
		}

		if (ImGui::IsItemHovered() && !isEmpty) {
			std::filesystem::path fullPath = ProjectContext::Get()->GetAssetDirectory() / path;

			ImGui::BeginTooltip();
			ImGui::TextUnformatted(fullPath.string().c_str());
			ImGui::EndTooltip();
		}

		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });

		if (ImGui::Button("x", { 20.0f, region.y })) {
			path.clear();
			changed = true;
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();

		if (!isEmpty) {
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::Selection);
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::InvalidPrefab);
		}

		ImVec2 padding = ImGui::GetStyle().FramePadding;
		ImGui::SetCursorPos({ pos.x + padding.x, pos.y + padding.y });
		ImGui::Text("%s", tag.c_str());

		ImGui::PopStyleColor();

		GUI::EndPropertyGrid();

		return changed;
	}

	/**
	 * @brief Draws a dropdown property for an entity.
	 *
	 * This function draws a dropdown property for an entity in the GUI.
	 *
	 * @param ID The ID of the entity.
	 * @param scene The scene containing the entity.
	 * @param label The label for the dropdown property.
	 * @param tooltip (optional) The tooltip for the dropdown property.
	 */
	static void DrawEntityDropdownProperty(
		u64& ID, Scene* scene, const char* label, const char* tooltip = nullptr
	) {
		Entity entity;
		std::string tag = "none";

		if (ID)
			entity = scene->GetEntityByID(ID);

		if (entity)
			tag = entity.GetTag();

		BeginPropertyGrid(label, tooltip, true);

		ImVec2 region = ImGui::GetContentRegionAvail();
		region.x -= 20.0f;
		region.y = ImGui::GetFrameHeight();

		ImVec2 pos = ImGui::GetCursorPos();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_Button]);

		ImGui::Button("##entity_dropdown_property", region);

		ImGui::PopStyleColor();

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity")) {
				Entity* payloadEntity = static_cast<Entity*>(payload->Data);

				ID = payloadEntity->GetID();
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });

		if (ImGui::Button("x", { 20.0f, region.y })) {
			ID = 0;
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();

		if (!ID) {
			ImVec4 selectedColor = GUI::Colors::Darken(ImVec4(0.6666666865348816f, 0.686274528503418f, 0.0784313753247261f, 1.0f), 0.05f);
			ImGui::PushStyleColor(ImGuiCol_Text, selectedColor);
		}

		ImVec2 padding = ImGui::GetStyle().FramePadding;
		ImGui::SetCursorPos({ pos.x + padding.x, pos.y + padding.y });
		ImGui::Text("%s", tag.c_str());

		if (ID) {
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay)) {
				ImGui::BeginTooltip();
				ImGui::Text("ID: %llu", ID);
				ImGui::EndTooltip();
			}
		}

		if (!ID)
			ImGui::PopStyleColor();

		EndPropertyGrid();
	}

	/**
	 * @brief Draws a font dropdown property in the GUI.
	 * 
	 * @param font A pointer to a Font object representing the selected font. This pointer will be updated with the user's selection.
	 * @param label The label to display for the font dropdown property.
	 * @param tooltip An optional tooltip to display for the font dropdown property.
	 */
	static void DrawFontDropdownProperty(
		Font** font, const char* label, const char* tooltip = nullptr
	) {
		std::string tag = "none";

		if (*font)
			tag = (*font)->GetFilename();

		BeginPropertyGrid(label, tooltip, true);

		ImVec2 region = ImGui::GetContentRegionAvail();
		region.x -= 20.0f;
		region.y = ImGui::GetFrameHeight();

		ImVec2 pos = ImGui::GetCursorPos();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_Button]);

		ImGui::Button("##font_dropdown_property", region);

		ImGui::PopStyleColor();

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Font")) {
				// *font = AssetManager::GetFont(static_cast<char*>(payload->Data));
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });

		if (ImGui::Button("x", { 20.0f, region.y })) {
			*font = nullptr;
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();

		if (*font) {
			ImVec4 selectedColor = GUI::Colors::Darken(ImVec4(0.6666666865348816f, 0.686274528503418f, 0.0784313753247261f, 1.0f), 0.05f);
			ImGui::PushStyleColor(ImGuiCol_Text, selectedColor);
		}

		ImVec2 padding = ImGui::GetStyle().FramePadding;
		ImGui::SetCursorPos({ pos.x + padding.x, pos.y + padding.y });
		ImGui::Text("%s", tag.c_str());

		if (*font)
			ImGui::PopStyleColor();

		EndPropertyGrid();
	}

	/**
	 * Draws a multiline text input property in the GUI.
	 *
	 * This function allows the user to input and edit multiline text in the GUI.
	 * The text input is displayed as a property grid with a label and an optional tooltip.
	 *
	 * @param text The reference to the string variable that will store the input text.
	 * @param label The label to display for the text input property.
	 * @param tooltip An optional tooltip to display for the text input property.
	 * @tparam N The maximum size of the input buffer. Defaults to 500.
	 */
	template <int N = 500>
	static void DrawMultilineTextInputProperty(
		std::string& text, const char* label, const char* tooltip = nullptr
	) {
		BeginPropertyGrid(label, tooltip, false);

		char buffer[N];

		memcpy(buffer, text.c_str(), std::min(sizeof(buffer), text.size() + 1));

		ImGui::PushItemWidth(-FLT_MIN);

		if (ImGui::InputTextMultiline("##multiline_text_input", buffer, sizeof(buffer))) {
			text = buffer;
		}

		ImGui::PopItemWidth();

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

		f32 frameHeight = ImGui::GetFrameHeight();
		const f32 buttonSize = frameHeight * 3.0f;
		const ImVec2 xButtonSize = { buttonSize / 4.0f, buttonSize };
		const f32 tooltipSize = frameHeight * 11.0f;

		ImGui::SetCursorPos({ ImGui::GetContentRegionMax().x - buttonSize - xButtonSize.x, ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.35f, 0.35f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f });

		Texture2D* textureCopy = *texture;

		if (!textureCopy)
			textureCopy = Renderer2D::BlackTexture;

		u32 textureId = textureCopy->GetTexHandle();

		ImGui::ImageButton(GUI::GetTextureID(textureId), { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 }, 0);

		if (*texture && ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay)) {
			ImGui::BeginTooltip();
			ImGui::TextUnformatted((*texture)->GetPath().c_str());
			ImGui::Spacing();
			ImGui::Image(GUI::GetTextureID(textureId), { tooltipSize, tooltipSize }, { 0, 1 }, { 1, 0 });
			ImGui::EndTooltip();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture")) {
				// *texture = AssetManager::GetTexture2D(static_cast<char*>(payload->Data));
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

	template <typename T>
		requires std::is_base_of_v<Asset, T>
	static bool DrawAssetDropdownProperty(
		AssetHandle& handle, const char* label, const char* tooltip = nullptr
	) {
		bool changed = false;

		std::string tag = "none";
		std::string fullPath = "none";

		if (handle) {
			const AssetMetaData& metadata = AssetManager::GetAssetMetaData(handle);

			tag = metadata.Path.filename().string();
			fullPath = (ProjectContext::Get()->GetAssetDirectory() / metadata.Path).string();
		}

		BeginPropertyGrid(label, tooltip, true);

		ImVec2 region = ImGui::GetContentRegionAvail();
		region.x -= 20.0f;
		region.y = ImGui::GetFrameHeight();

		ImVec2 pos = ImGui::GetCursorPos();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_Button]);

		ImGui::Button("##font_dropdown_property", region);

		if (ImGui::IsItemHovered() && handle) {
			ImGui::BeginTooltip();
			ImGui::TextUnformatted(fullPath.c_str());
			ImGui::EndTooltip();
		}

		ImGui::PopStyleColor();

		if (ImGui::BeginDragDropTarget()) {
			const char* payloadName = Asset::GetStringifiedAssetType(T::GetStaticType());

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadName)) {
				handle = *static_cast<u64*>(payload->Data);

				changed = true;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });

		if (ImGui::Button("x", { 20.0f, region.y })) {
			handle = 0;
			changed = true;
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();

		if (handle) {
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::Selection);
		} else {
			ImGui::PushStyleColor(ImGuiCol_Text, GUI::Theme::InvalidPrefab);
		}

		ImVec2 padding = ImGui::GetStyle().FramePadding;
		ImGui::SetCursorPos({ pos.x + padding.x, pos.y + padding.y });
		ImGui::Text("%s", tag.c_str());

		ImGui::PopStyleColor();

		EndPropertyGrid();

		return changed;
	}

	/**
	 * @brief Draws a part of an image with customizable properties.
	 *
	 * @param wholeImage Pointer to the Texture2D object representing the whole image.
	 * @param label The label for the property grid.
	 * @param tooltip Optional tooltip for the property grid.
	 * @param offset The offset of the image part within the whole image.
	 * @param size The size of the image part.
	 * @param tint The tint color of the image part.
	 * @param showBorder Show border around the image.
	 */
	static void DrawImagePartProperty(
		Texture2D* wholeImage, const char* label, const char* tooltip = nullptr,
		glm::vec2 offset = glm::vec2(0.0f), glm::vec2 size = glm::vec2(0.0f), glm::vec4 tint = glm::vec4(1.0f),
		bool showBorder = false
	) {
		f32 width = (f32)wholeImage->GetWidth();
		f32 height = (f32)wholeImage->GetHeight();

		GUI::BeginPropertyGrid(label, tooltip, false);

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
			GUI::GetTextureID(wholeImage->GetTexHandle()), imagePartSize, uv0, uv1, { tint.r, tint.g, tint.b, tint.a }, borderCol
		);

		GUI::EndPropertyGrid();
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

		GUI::MoveMousePosX(ImGui::GetColumnWidth() / 2.0f - ImGui::GetStyle().FramePadding.x - 7.5f);

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
		}
		else {
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
	 * @brief Draws a f32ing-point property in the GUI.
	 *
	 * This function is used to draw a f32ing-point property in the GUI. It provides options for setting the value, label, tooltip, minimum and maximum values, delta, and format.
	 *
	 * @tparam T The type of the f32ing-point value.
	 * @param value The reference to the f32ing-point value to be displayed and modified.
	 * @param label The label for the property.
	 * @param tooltip The tooltip for the property (optional).
	 * @param min The minimum value for the property (optional).
	 * @param max The maximum value for the property (optional).
	 * @param delta The delta value for the property (optional).
	 * @param format The format string for displaying the value (optional).
	 * 
	 * @return Whether something changed.
	 */
	template<std::floating_point T>
	static bool DrawFloatingPointProperty(
		T& value, const char* label, const char* tooltip = nullptr, T min = 0, T max = 0, f32 delta = 0.1f, const char* format = "%.3f"
	) {
		bool changed = false;

		BeginPropertyGrid(label, tooltip);

		int dataType = ImGuiDataType_Float;

		if constexpr (sizeof(T) == 8)
			dataType = ImGuiDataType_Double;

		//if (max > min) { TODO: Come up with a separate method to once and for all fix min, max problem, and big values drag
			//if (ImGui::SliderScalar("##property_f32ing_point", dataType, &value, &min, &max, format))
				//changed = true;
		//} else {
			if (ImGui::DragScalar("##property_f32ing_point", dataType, &value, delta, &min, &max, format))
				changed = true;
		//}
		
		EndPropertyGrid();

		return changed;
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
	struct SelectOption final {
		std::string Label = "No label";
		T value = 0;
	};

	/**
	 * @brief Draws a selectable widget in the GUI.
	 *
	 * This function displays a selectable widget in the GUI, allowing the user to choose from a list of options.
	 * The selected value is stored in the provided reference variable.
	 *
	 * @tparam T The type of the value.
	 * @param value The reference to the value.
	 * @param options The vector of selectable options.
	 * @param flags Additional ImGui flags (optional).
	 *
	 * @return bool Whether something has changed
	 */
	template <typename T>
	static bool DrawSelectable(
		T& value, const std::vector<SelectOption<T>>& options, ImGuiComboFlags flags = ImGuiComboFlags_None
	) {
		bool changed = false;

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

					changed = true;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		return changed;
	}

	/**
	 * @brief Draws a selectable property in the GUI.
	 *
	 * This function displays a selectable property in the GUI, allowing the user to choose from a list of options.
	 * The selected value is stored in the provided reference variable.
	 *
	 * @tparam T The type of the property value.
	 * @param value The reference to the property value.
	 * @param options The vector of selectable options.
	 * @param label The label for the property.
	 * @param tooltip The tooltip for the property (optional).
	 * @param flags Additional ImGui flags (optional).
	 * 
	 * @return bool Whether something has changed
	 */
	template <typename T>
	static bool DrawSelectableProperty(
		T& value, const std::vector<SelectOption<T>>& options, const char* label,
		const char* tooltip = nullptr, ImGuiComboFlags flags = ImGuiComboFlags_None
	) {
		bool changed = false;

		BeginPropertyGrid(label, tooltip);

		changed = DrawSelectable(value, options, flags);

		EndPropertyGrid();

		return changed;
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
	static void ClippedText(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_display_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect, f32 wrap_width)
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
	static void ClippedText(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect, f32 wrap_width)
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

	static f32 GetWindowContentRegionHeight()
	{
		ImGuiWindow* window = GImGui->CurrentWindow;
		return window->ContentRegionRect.GetHeight();
	}

	static f32 GetWindowContentRegionWidth()
	{
		ImGuiWindow* window = GImGui->CurrentWindow;
		return window->ContentRegionRect.GetWidth();
	}

	static void DrawScale(const ImVec2& from, const ImVec2& to, f32 majorUnit, f32 minorUnit, f32 labelAlignment, f32 sign = 1.0f)
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

	static bool Splitter(bool split_vertically, f32 thickness, f32* size1, f32* size2, f32 min_size1, f32 min_size2, f32 splitter_long_axis_size = -1.0f)
	{
		using namespace ImGui;

		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ImGuiID id = window->GetID("##Splitter");
		ImRect bb;

		bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
		bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);

		return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
	}

}
