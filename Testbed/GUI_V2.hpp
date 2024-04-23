#pragma once

namespace SW::GUI2 {

	// --------------------------------
	//			  UTILITIES
	// --------------------------------

	/**
	 * @brief Casts Texture2D to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of greater size
	 * @param texture The texture to cast to ImTextureID.
	 * @return The ImTextureID of the texture.
	 */
	inline static ImTextureID GetTextureID(const Texture2D& texture)
	{
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture.GetTexHandle()));
	}

	/**
	 * @brief Casts Texture2D to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of greater size
	 * @param texture The texture to cast to ImTextureID.
	 * @return The ImTextureID of the texture.
	 */
	inline static ImTextureID GetTextureID(const Texture2D* texture)
	{
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture->GetTexHandle()));
	}

	/**
	 * @brief Casts u32 to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of greater size
	 * @param rendererID The renderer ID to cast to ImTextureID.
	 * @return The ImTextureID of the renderer ID.
	 */
	inline static ImTextureID GetTextureID(u32 rendererID)
	{
		return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(rendererID));
	}

	/**
	 * @brief Get the rectangle that the current item occupies.
	 */
	inline static ImRect GetItemRect()
	{
		return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}

	/**
	 * @brief Check whether last item is in a disabled state.
	 * @example
	 *		ImGui::InputText(...);
	 *		GUI::IsItemDisabled(); -> Checks whether preceeding InputText is in a disabled state.
	 */
	inline bool IsItemDisabled()
	{
		return ImGui::GetItemFlags() & ImGuiItemFlags_Disabled;
	}

	/**
	 * @brief Check whether last item is being hovered.
	 * @example
	 *		ImGui::InputText(...);
	 *		GUI::IsItemHovered(); -> Checks whether preceeding InputText is being hovered.
	 */
	inline bool IsItemHovered()
	{
		return ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	}

	/**
	 * @brief Expands the rectangle by the given amount. Expanded in all directions.
	 * @param rect The rectangle to expand.
	 * @param x X-axis amount to expand by.
	 * @param y Y-axis amount to expand by.
	 * @return The expanded rectangle.
	 */
	inline static ImRect RectExpanded(const ImRect& rect, f32 x, f32 y)
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
	 * @param rect The original rectangle to offset.
	 * @param x The amount to offset the rectangle in the x direction.
	 * @param y The amount to offset the rectangle in the y direction.
	 * @return The offset rectangle.
	 */
	inline static ImRect RectOffset(const ImRect& rect, f32 x, f32 y)
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
	 */
	inline static void MoveMousePosX(f32 x)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x);
	}

	/**
	 * @brief Moves the mouse cursor to the specified position on the y-axis.
	 */
	inline static void MoveMousePosY(f32 y)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + y);
	}

	/**
	 * @brief Utility helper class to make use of the scope mechanic
	 *		  so that we easily can avoid many calls to Push/Pop ID.
	 *		  This way more advanced UI code doesn't get so clustered.
	 * @example
	 * 		{
	 * 			ScopedID id(1);
	 * 		}
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
	 * @brief Utility helper class to make use of the scope mechanic
	 *		  so that we easily can avoid many calls to Push/Pop StyleVar.
	 *		  This way more advanced UI code doesn't get so clustered.
	 * @example
	 *		{
	 *			ScopedStyle FrameRounding(ImGuiStyleVar_FrameRounding, 3.0f);
	 *		}
	 */
	class ScopedStyle
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
	 * @example
	 *		{
	 *			ScopedColor Border(ImGuiCol_Border, ImVec4{ 1.f, 1.f , 1.f , 0.f });
	 *		}
	 */
	class ScopedColor
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
	 * @example
	 * 		{
	 * 			ScopedFont font(ImGui::GetIO().Fonts->Fonts[1]);
	 * 		}
	 */
	class ScopedFont
	{
	public:
		ScopedFont(const ScopedFont&) = delete;
		ScopedFont(const ScopedFont&&) = delete;
		ScopedFont& operator=(const ScopedFont&) = delete;
		ScopedFont& operator=(const ScopedFont&&) = delete;

		ScopedFont(ImFont* font) { ImGui::PushFont(font); }

		~ScopedFont() { ImGui::PopFont(); }
	};

	// --------------------------------
	//			   LAYOUT
	// --------------------------------

	namespace Layout {

		/**
		 * @brief Creates a dockspace with the specified name and options.
		 *		  The dockspace is created at the position of the main viewport,
		 *		  and its size is set to the size of the main viewport. The window flags for the dockspace can be customized.
		 * @param name The name of the dockspace. This is also used as the ID for the dockspace.
		 * @param fn The function to call to draw the contents of the dockspace. This function should return the height of the top of the dockspace.
		 * @param flags The window flags for the dockspace. These determine the behavior and appearance of the dockspace. Default is a combination of flags that result in a dockspace with no title bar, no collapse button, no resize handles, and no move functionality.
		 */
		template <typename T>
		static void CreateDockspace(
			const char* name, T fn, ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
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
		 * @brief Begins a menu bar and sets its bounding box. (custom menu bar inside specified rectangle bounding box)
		 *		  This function begins a new menu bar context in the GUI. All subsequent GUI elements will be rendered within this
		 *		  menu bar until EndMenuBar() is called. The menu bar is defined by the provided rectangle, which specifies its position
		 *		  and size. The top-left corner of the rectangle is the top-left corner of the menu bar, and the width and height of the
		 *		  rectangle define the size of the menu bar. This function returns a boolean value indicating whether the menu bar is
		 *		  currently open.
		 * @warning This function must always be paired with a call to EndMenuBar(). Failure to do so will result in undefined behavior.
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

	// --------------------------------
	//			  COMPONENTS
	// --------------------------------

	namespace Components {

		typedef int OutlineFlags;
		enum OutlineFlags_
		{
			OutlineFlags_None = 0,					// draw no activity outline
			OutlineFlags_WhenHovered = 1 << 1,		// draw an outline when item is hovered
			OutlineFlags_WhenActive = 1 << 2,		// draw an outline when item is active
			OutlineFlags_HighlightActive = 1 << 4,  // when active, the outline is in highlight color
			OutlineFlags_NoHighlightActive = OutlineFlags_WhenHovered | OutlineFlags_WhenActive,

			OutlineFlags_NoOutlineInactive = OutlineFlags_WhenHovered | OutlineFlags_WhenActive | OutlineFlags_HighlightActive,
			OutlineFlags_All = OutlineFlags_WhenHovered | OutlineFlags_WhenActive | OutlineFlags_HighlightActive,
		};

		/**
		 * @brief Draws an outline around the last item.
		 * @example
		 *		ImGui::InputText(...);
		 *		GUI::Components::ItemActivityOutline(); -> outline for the InputText
		 * @param flags The flags to determine when to draw the outline.
		 * @param color The color of the outline when active.
		 * @param rounding The rounding of the outline.
		 */
		void ItemActivityOutline(
			OutlineFlags flags = OutlineFlags_All, ImColor color = GUI::Theme::ActivityOutline,
			f32 rounding = GImGui->Style.FrameRounding
		);

		/**
		 * @brief Draws a border around the specified rectangle.
		 * @param rect The rectangle to draw a border around.
		 * @param color The color of the outline border.
		 * @param thickness The thickness of the border.
		 * @param rounding The rounding of the border.
		 * @param offsetX The offset of the border on the x-axis.
		 * @param offsetY The offset of the border on the y-axis.
		 */
		void RectangleOutline(
			ImRect rect, ImColor color = GUI::Theme::Outline, f32 thickness = 1.0f, f32 rounding = 0.0f, f32 offsetX = 0.0f,
			f32 offsetY = 0.0f
		);

		/**
		 * @brief Draws a border around the last item.
		 * @example
		 *		ImGui::InputText(...);
		 *		GUI::Components::ItemOutline(); -> outline for the InputText
		 * @param color The color of the outline border.
		 * @param thickness The thickness of the border.
		 * @param rounding The rounding of the border.
		 * @param offsetX The offset of the border on the x-axis.
		 * @param offsetY The offset of the border on the y-axis.
		 */
		void ItemOutline(
			ImColor color = GUI::Theme::Outline, f32 thickness = 1.0f, f32 rounding = 0.0f, f32 offsetX = 0.0f,
			f32 offsetY = 0.0f
		);

		/**
		 * @brief Draws a horizontal line with a given thickness. Wrapper for C style formatting.
		 * @param thickness The thickness of the line.
		 */
		template <typename... Args>
		void Text(std::string_view fmt, const Args&& ... args)
		{
			const std::string str = std::vformat(fmt, std::make_format_args(args...));

			ImGui::TextUnformatted(str.c_str());
		}

		/**
		 * @brief Draws a single line text input property in the GUI.
		 * @tparam N The size of the buffer used to store the text input.
		 * @param text The reference to the string where the entered text will be stored.
		 * @param flags Additional ImGui input flags.
		 * @return bool Whether something has changed
		 */
		template <int N = 256>
		static bool SingleLineTextInput(
			std::string* text, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None
		) {
			char buffer[N] = {};
			strncpy(buffer, text->c_str(), N);

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer), flags)) {
				*text = buffer;

				return true;
			}

			return false;
		}

	}

	// --------------------------------
	//			  WIDGETS
	// --------------------------------

	namespace Widgets {

		/**
		 * @brief Search input field with a search icon and a clear button.
		 * @warning Caller is responsible for pushing unique ID if needed.
		 * @tparam N The maximum length of the search string. Higher values will not be accepted. Safe method.
		 * @param search The search string to be modified.
		 * @param flags The ImGui input text flags.
		 * @return Whether search widget was used and the value was modified.
		 */
		template <int N = 64>
		static bool SearchInput(std::string* search, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
		{
			const f32 posx = ImGui::GetCursorPosX();
			const f32 framePaddingY = ImGui::GetStyle().FramePadding.y;
			constexpr f32 bw = 28.f;
			constexpr f32 buttonOffset = 11.f;
			constexpr f32 searchIconOffset = 4.f;
			constexpr f32 searchHintOffset = 28.f;

			bool modified = false;

			const bool searching = !search->empty();

			const f32 w = ImGui::GetContentRegionAvail().x;

			if (searching) {
				ImGui::SameLine(w - bw + buttonOffset);
				ImGui::SetNextItemAllowOverlap();

				ScopedColor Border(ImGuiCol_Border, ImVec4{ 1.f, 1.f , 1.f , 0.f });
				ScopedColor Button(ImGuiCol_Button, ImVec4{ 1.f, 1.f , 1.f , 0.f });

				if (ImGui::Button(SW_ICON_CLOSE_OCTAGON, ImVec2{ bw, ImGui::GetFrameHeight() })) {
					search->clear();
					modified = true;
				}

				if (IsItemHovered())
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			ImGui::SameLine(posx);

			{
				ScopedStyle FrameRounding(ImGuiStyleVar_FrameRounding, 3.0f);
				ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(28.0f, framePaddingY));

				ImGui::SetNextItemWidth(searching ? w - bw : w);

				if (Components::SingleLineTextInput<N>(search, flags)) {
					modified = true;
				}

				Components::ItemActivityOutline();
			}

			ImGui::SameLine(posx + searchIconOffset);
			ImGui::TextUnformatted(SW_ICON_MAGNIFY);

			if (!searching) {
				ImGui::SameLine(posx + searchHintOffset);
				ImGui::TextUnformatted("Search ...");
			}

			return modified;
		}

	}

	//static void ToggleButton(const char* str_id, bool* v)
	//{
	//	ImVec4* colors = ImGui::GetStyle().Colors;
	//	ImVec2 p = ImGui::GetCursorScreenPos();
	//	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	//	float height = ImGui::GetFrameHeight();
	//	float width = height * 2.f;
	//	float radius = height * 0.50f;
	//	float rounding = 0.2f;

	//	ImGui::InvisibleButton(str_id, ImVec2(width, height));
	//	if (ImGui::IsItemClicked()) *v = !*v;
	//	ImGuiContext& gg = *GImGui;
	//	float ANIM_SPEED = 0.03f;
	//	if (gg.LastActiveId == gg.CurrentWindow->GetID(str_id))
	//		float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
	//	if (ImGui::IsItemHovered())
	//		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_ButtonActive] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * rounding);
	//	else
	//		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * rounding);

	//	ImVec2 center = ImVec2(radius + (*v ? 1 : 0) * (width - radius * 2.0f), radius);
	//	draw_list->AddRectFilled(ImVec2((p.x + center.x) - 9.0f, p.y + 1.5f),
	//		ImVec2((p.x + (width / 2) + center.x) - 9.0f, p.y + height - 1.5f), IM_COL32(255, 255, 255, 255), height * rounding);
	//}

	//static void ToggleButton2(const char* str_id, bool* v)
	//{
	//	ImVec2 p = ImGui::GetCursorScreenPos();
	//	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	//	ImVec4* colors = ImGui::GetStyle().Colors;

	//	float height = ImGui::GetFrameHeight();
	//	float width = height * 2.f;
	//	float radius = height * 0.50f;

	//	ImGui::InvisibleButton(str_id, ImVec2(width, height));
	//	if (ImGui::IsItemClicked())
	//		*v = !*v;

	//	float t = *v ? 1.0f : 0.0f;

	//	ImGuiContext& g = *GImGui;
	//	float ANIM_SPEED = 0.03f;
	//	if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
	//	{
	//		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
	//		t = *v ? (t_anim) : (1.0f - t_anim);
	//	}

	//	ImU32 col_bg;
	//	if (ImGui::IsItemHovered())
	//		col_bg = ImGui::GetColorU32(*v ? ImVec4(0.78f, 0.78f, 0.78f, 1.0f) : colors[ImGuiCol_ButtonActive]);
	//	else
	//		col_bg = ImGui::GetColorU32(*v ? ImVec4(0.85f, 0.85f, 0.85f, 1.0f) : colors[ImGuiCol_Button]);

	//	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.2f);
	//	draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	//}

}
