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
	 * @brief Helper function to get the ImGuiDataType for any scalar type.
	 * @tparam T The scalar type to get the ImGuiDataType for.
	 * @return The ImGuiDataType for the scalar type.
	 */
	template <typename T>
		requires std::is_scalar_v<T>
	static consteval ImGuiDataType GetScalarDatatype()
	{
		if constexpr (std::is_floating_point_v<T>)
			if constexpr (sizeof(T) == 4) return ImGuiDataType_Float;
			else if constexpr (sizeof(T) == 8) return ImGuiDataType_Double;
		else if constexpr (std::is_signed_v<T>)
			if constexpr (sizeof(T) == 1) return ImGuiDataType_S8;
			else if constexpr (sizeof(T) == 2) return ImGuiDataType_S16;
			else if constexpr (sizeof(T) == 4) return ImGuiDataType_S32;
			else if constexpr (sizeof(T) == 8) return ImGuiDataType_S64;
		else if constexpr (std::is_unsigned_v<T>)
			if constexpr (sizeof(T) == 1) return ImGuiDataType_U8;
			else if constexpr (sizeof(T) == 2) return ImGuiDataType_U16;
			else if constexpr (sizeof(T) == 4) return ImGuiDataType_U32;
			else if constexpr (sizeof(T) == 8) return ImGuiDataType_U64;

		return ImGuiDataType_S32;
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
			OutlineFlags flags = OutlineFlags_All, ImColor activeCol = GUI::Theme::ActivityOutline, ImColor hoverCol = GUI::Theme::Outline,
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
		 * @brief Draws a single line text input in the GUI.
		 * @tparam N The size of the buffer used to store the text input.
		 * @param text The reference to the string where the entered text will be stored.
		 * @param flags Additional ImGui input flags.
		 * @return bool Whether something has changed
		 */
		template <int N = 64>
		static bool SingleLineTextInput(
			std::string* text, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None
		) {
			char buffer[N] = {};
			strncpy(buffer, text->c_str(), N);

			const f32 w = ImGui::GetContentRegionAvail().x;
			bool modified = false;

			ImGui::SetNextItemWidth(w);

			if (ImGui::InputText("##input_text", buffer, sizeof(buffer), flags)) {
				*text = buffer;
				modified = true;
			}

			Components::ItemActivityOutline();

			return modified;
		}

		/**
		 * @brief Draws a single line text input in the GUI.
		 * @tparam N The size of the buffer used to store the text input.
		 * @param text The reference to the string where the entered text will be stored.
		 * @param flags Additional ImGui input flags.
		 * @return bool Only when submitted using enter.
		 */
		template <int N = 64>
		static bool SingleLineTextInputDeffered(
			std::string* text, ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue
		) {
			return SingleLineTextInput<N>(text, flags);
		}

		/**
		 * Draws a multiline text input in the GUI.
		 * @tparam N The maximum size of the input buffer. Defaults to 500.
		 * @param text The reference to the string variable that will store the input text.
		 * @param size The desired size of the input field.
		 * @param flags Additional ImGui input flags.
		 * @return bool Whether something has changed
		 */
		template <int N = 128>
		static bool MultiLineTextInput(
			std::string* text, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = ImGuiInputTextFlags_None
		) {
			char buffer[N] = {};
			strncpy(buffer, text->c_str(), N);

			const f32 w = ImGui::GetContentRegionAvail().x;
			bool modified = false;

			ImGui::SetNextItemWidth(w);

			if (ImGui::InputTextMultiline("##input_text_multiline", buffer, sizeof(buffer), size, flags)) {
				*text = buffer;
				modified = true;
			}

			Components::ItemActivityOutline();

			return modified;
		}

		/**
		 * Draws a multiline text input in the GUI.
		 * @tparam N The maximum size of the input buffer. Defaults to 500.
		 * @param text The reference to the string variable that will store the input text.
		 * @param size The desired size of the input field.
		 * @param flags Additional ImGui input flags.
		 * @return bool Only when submitted using enter.
		 */
		template <int N = 128>
		static bool MultiLineTextInputDeffered(
			std::string* text, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue
		) {
			return MultiLineTextInput<N>(text, size, flags);
		}

		/**
		 * @brief Draws a boolean checkbox in the GUI.
		 * @param value The boolean value to represent with the checkbox.
		 * @param center (optional) Whether the checkbox should be centered in the GUI.
		 * @return bool Whether something has changed
		 */
		bool Checkbox(bool* value, bool center = true);

		/**
		 * @brief Draws a boolean toggle in the GUI.
		 * @param value The boolean value to represent with the checkbox.
		 * @param center (optional) Whether the toggle should be centered in the GUI.
		 * @return bool Whether something has changed
		 */
		bool Toggle(bool* value, bool center = true);

		/**
		 * @brief Single option for radio button.
		 */
		template <typename T>
			requires std::is_integral_v<T>
		struct SelectOption
		{
			std::string Label = "No label";
			T value = 0;
		};

		/**
		 * @brief Draws a radio button in the GUI.
		 * @note Radio buttons are not idealy centered, but it's a good start.
		 * @tparam T The type of the value to represent with the radio button.
		 * @param value The value to represent with the radio button.
		 * @param options The list of options to display as radio buttons.
		 * @param center (optional) Whether the radio buttons should be centered in the GUI.
		 * @param flags Additional ImGui combo flags.
		 * @return bool Whether something has changed
		 */
		template <typename T>
		static bool RadioButton(
			T* value, const std::vector<SelectOption<T>>& options, bool center = true, ImGuiComboFlags flags = ImGuiComboFlags_None
		) {
			bool modified = false;
			const f32 w = ImGui::GetContentRegionAvail().x;

			f32 leftOffset = 0.0f;
			for (int i = 0; i < (int)options.size(); i++) {
				const SelectOption<T>& option = options[i];
				leftOffset += ImGui::CalcTextSize(option.Label.c_str()).x;
			}

			f32 width = ImGui::GetIO().FontGlobalScale * ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		
			leftOffset += (f32)options.size() * width;

			if (center)
				GUI2::MoveMousePosX((w - leftOffset) / 2 - 20.f);

			int val = (int)(*value);
			for (int i = 0; i < (int)options.size(); i++) {
				const SelectOption<T>& option = options[i];

				if (ImGui::RadioButton(option.Label.c_str(), &val, (int)option.value)) {
					modified = true;
					*value = (T)val;
				}

				Components::ItemActivityOutline(Components::OutlineFlags_All, GUI::Theme::ActivityOutline,
					GUI::Theme::ActivityOutline, 10.f);

				if (i != options.size() - 1)
					ImGui::SameLine();
			}

			return modified;
		}

		/**
		 * @brief Draws a scalar input field in the GUI.
		 * @tparam T The scalar type of the value to represent with the input field.
		 * @param value The value to represent with the input field.
		 * @param step The step value for the input field.
		 * @param fastStep The fast step value for the input field. Active when holding ctrl.
		 * @param format The format string for the input field.
		 * @param flags Additional ImGui input flags.
		 * @return bool Whether something has changed
		*/
		template <typename T>
			requires std::is_scalar_v<T>
		static bool ScalarInput(T* value, T step = (T)1, T fastStep = (T)10, const char* format = nullptr, ImGuiInputFlags flags = ImGuiInputTextFlags_None)
		{
			bool modified = false;

			constexpr ImGuiDataType dataType = GetScalarDatatype<T>();

			const f32 w = ImGui::GetContentRegionAvail().x;

			ImGui::SetNextItemWidth(w);

			if (ImGui::InputScalar("##input_scalar", dataType, value, &step, &fastStep, format, flags))
				modified = true;

			Components::ItemActivityOutline();

			return modified;
		}

		/**
		 * @brief Draws a scalar slider in the GUI.
		 * @tparam T The scalar type of the value to represent with the slider.
		 * @param value The value to represent with the slider.
		 * @param min The minimum value of the slider.
		 * @param max The maximum value of the slider.
		 * @param format The format string for the slider.
		 * @param flags Additional ImGui input flags.
		 * @return bool Whether something has changed
		 */
		template <typename T>
			requires std::is_scalar_v<T>
		static bool ScalarSlider(T* value, T min = (T)1, T max = (T)10, const char* format = nullptr, ImGuiInputFlags flags = ImGuiInputTextFlags_None)
		{
			bool modified = false;

			constexpr ImGuiDataType dataType = GUI2::GetScalarDatatype<T>();

			const f32 w = ImGui::GetContentRegionAvail().x;

			ImGui::SetNextItemWidth(w);

			if (ImGui::SliderScalar("##input_scalar", dataType, value, &min, &max, format, flags))
				modified = true;

			Components::ItemActivityOutline();

			return modified;
		}

		/**
		 * @brief Draws a scalar drag input field in the GUI.
		 * @tparam T The scalar type of the value to represent with the drag input field.
		 * @param value The value to represent with the drag input field.
		 * @param speed The speed value for the drag input field.
		 * @param min The minimum value of the drag input field.
		 * @param max The maximum value of the drag input field.
		 * @param format The format string for the drag input field.
		 * @param flags Additional ImGui input flags.
		 * @return bool Whether something has changed
		 */
		template <typename T>
			requires std::is_scalar_v<T>
		static bool ScalarDrag(T* value, float speed = 1.f, T min = (T)1, T max = (T)10, const char* format = nullptr, ImGuiInputFlags flags = ImGuiInputTextFlags_None)
		{
			bool modified = false;

			constexpr ImGuiDataType dataType = GUI2::GetScalarDatatype<T>();

			const f32 w = ImGui::GetContentRegionAvail().x;

			ImGui::SetNextItemWidth(w);

			if (ImGui::DragScalar("##input_scalar", dataType, value, speed, &min, &max, format, flags))
				modified = true;

			Components::ItemActivityOutline();

			return modified;
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

				char buffer[N] = {};
				strncpy(buffer, search->c_str(), N);

				if (ImGui::InputText("##input_text", buffer, sizeof(buffer), flags)) {
					*search = buffer;
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

}
