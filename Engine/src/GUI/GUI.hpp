/**
 * @file GUI.hpp
 * @brief SW Plugin - GUI Utility Library.
 *
 * This file contains declarations for utility functions, components, and widgets
 * designed for use with the ImGui library. The library includes custom-made utilities
 * such as ScalarInput<float>(), GetItemRect(), SearchWidget(), and more, providing enhanced
 * functionality for creating graphical user interfaces.
 *
 * @author SW
 * @version 0.3.0
 * @date 2024-04-28
 * @copyright Copyright (c) 2024 SW
 *
 * @note
 *	This header depends on the ImGui library (https://github.com/ocornut/imgui).
 *  This plugin does not require any additional dependencies nor being initialized.
 * @warning
 *  User of the components is responsible for proper unique ID handling.
 */
#pragma once

#include "Appearance.hpp"
#include "Asset/AssetManager.hpp"
#include "Asset/Texture2DAsset.hpp"
#include "Colors.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "Icons.hpp"
#include "OpenGL/Rendering/Texture2D.hpp"

#define USE_TEXTURE_2D_METHODS
#define USE_ASSET_MANAGER_PLUGIN
#define USE_FILESYSTEM_PLUGIN

namespace SW::GUI
{

	// --------------------------------
	//			  UTILITIES
	// --------------------------------

	/**
	 * @brief Casts Texture2D to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of
	 * greater size
	 * @param texture The texture to cast to ImTextureID.
	 * @return The ImTextureID of the texture.
	 */
	inline static ImTextureID GetTextureID(const Texture2DAsset& texture)
	{
		return (ImTextureID)texture.OpenGL::Texture2D::GetHandle();
	}

	/**
	 * @brief Casts Texture2D to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of
	 * greater size
	 * @param texture The texture to cast to ImTextureID.
	 * @return The ImTextureID of the texture.
	 */
	inline static ImTextureID GetTextureID(const Texture2DAsset* texture)
	{
		return (ImTextureID)texture->OpenGL::Texture2D::GetHandle();
	}

	/**
	 * @brief Casts u32 to ImTextureID so that it can be used in ImGui
	 * @note This is a workaround for the warning C4312: 'reinterpret_cast': conversion from 'u32' to 'ImTextureID' of
	 * greater size
	 * @param rendererID The renderer ID to cast to ImTextureID.
	 * @return The ImTextureID of the renderer ID.
	 */
	inline static ImTextureID GetTextureID(u32 rendererID)
	{
		return (ImTextureID)rendererID;
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
			if constexpr (sizeof(T) == 4)
				return ImGuiDataType_Float;
			else if constexpr (sizeof(T) == 8)
				return ImGuiDataType_Double;
			else if constexpr (std::is_signed_v<T>)
				if constexpr (sizeof(T) == 1)
					return ImGuiDataType_S8;
				else if constexpr (sizeof(T) == 2)
					return ImGuiDataType_S16;
				else if constexpr (sizeof(T) == 4)
					return ImGuiDataType_S32;
				else if constexpr (sizeof(T) == 8)
					return ImGuiDataType_S64;
				else if constexpr (std::is_unsigned_v<T>)
					if constexpr (sizeof(T) == 1)
						return ImGuiDataType_U8;
					else if constexpr (sizeof(T) == 2)
						return ImGuiDataType_U16;
					else if constexpr (sizeof(T) == 4)
						return ImGuiDataType_U32;
					else if constexpr (sizeof(T) == 8)
						return ImGuiDataType_U64;

		return ImGuiDataType_S32;
	}

	/**
	 * @brief Get the current window's content region height.
	 * @return The height of the current window's content region.
	 */
	inline f32 GetCurrentWindowContentRegionHeight()
	{
		return GImGui->CurrentWindow->ContentRegionRect.GetHeight();
	}

	/**
	 * @brief Get the current window's content region width.
	 * @return The width of the current window's content region.
	 */
	inline f32 GetCurrentWindowContentRegionWidth()
	{
		return GImGui->CurrentWindow->ContentRegionRect.GetWidth();
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

	void HelpMarker(const char* desc);

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
		ScopedID(const ScopedID&)             = delete;
		ScopedID(const ScopedID&&)            = delete;
		ScopedID& operator=(const ScopedID&)  = delete;
		ScopedID& operator=(const ScopedID&&) = delete;

		template <typename T>
		ScopedID(T id)
		{
			ImGui::PushID(id);
		}

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
		ScopedStyle(const ScopedStyle&)             = delete;
		ScopedStyle(const ScopedStyle&&)            = delete;
		ScopedStyle& operator=(const ScopedStyle&)  = delete;
		ScopedStyle& operator=(const ScopedStyle&&) = delete;

		template <typename T>
		ScopedStyle(ImGuiStyleVar styleVar, T value)
		{
			ImGui::PushStyleVar(styleVar, value);
		}

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
		ScopedColor(const ScopedColor&)             = delete;
		ScopedColor(const ScopedColor&&)            = delete;
		ScopedColor& operator=(const ScopedColor&)  = delete;
		ScopedColor& operator=(const ScopedColor&&) = delete;

		template <typename T>
		ScopedColor(ImGuiCol colourId, T colour)
		{
			ImGui::PushStyleColor(colourId, ImColor(colour).Value);
		}

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
		ScopedFont(const ScopedFont&)             = delete;
		ScopedFont(const ScopedFont&&)            = delete;
		ScopedFont& operator=(const ScopedFont&)  = delete;
		ScopedFont& operator=(const ScopedFont&&) = delete;

		ScopedFont(ImFont* font) { ImGui::PushFont(font); }

		~ScopedFont() { ImGui::PopFont(); }
	};

	// --------------------------------
	//			   LAYOUT
	// --------------------------------

	namespace Layout
	{

		/**
		 * @brief Creates a dockspace with the specified name and options.
		 *		  The dockspace is created at the position of the main viewport,
		 *		  and its size is set to the size of the main viewport. The window flags for the dockspace can be
		 *customized.
		 * @param name The name of the dockspace. This is also used as the ID for the dockspace.
		 * @param fn The function to call to draw the contents of the dockspace. This function should return the height
		 *of the top of the dockspace.
		 * @param flags The window flags for the dockspace. These determine the behavior and appearance of the
		 *dockspace. Default is a combination of flags that result in a dockspace with no title bar, no collapse button,
		 *no resize handles, and no move functionality.
		 */
		template <typename T>
		static void CreateDockspace(const char* name, T fn,
		                            ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		                                                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		                                                     ImGuiWindowFlags_NoBringToFrontOnFocus |
		                                                     ImGuiWindowFlags_NoNavFocus)
		{
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
		 *		  This function begins a new menu bar context in the GUI. All subsequent GUI elements will be rendered
		 *within this menu bar until EndMenuBar() is called. The menu bar is defined by the provided rectangle, which
		 *specifies its position and size. The top-left corner of the rectangle is the top-left corner of the menu bar,
		 *and the width and height of the rectangle define the size of the menu bar. This function returns a boolean
		 *value indicating whether the menu bar is currently open.
		 * @warning This function must always be paired with a call to EndMenuBar(). Failure to do so will result in
		 *undefined behavior.
		 * @param barRectangle The rectangle defining the position and size of the menu bar.
		 * @return bool True if the menu bar is open and ready for items to be added, false otherwise.
		 */
		bool BeginMenuBar(const ImRect& barRectangle);

		/**
		 * @brief Ends a menu bar.
		 *		  This function ends the current menu bar context in the GUI.
		 *		  After this function is called, GUI elements will no longer be rendered within the menu bar.
		 *		  You should always call this function after you're done adding items to a menu bar.
		 * @warning This function must always be paired with a call to BeginMenuBar(). Failure to do so will result in
		 *undefined behavior.
		 */
		void EndMenuBar();

		/**
		 * @brief Begins a new collapsible header with the specified name.
		 * @warning This function must always be paired with a call to EndHeaderCollapse(). Failure to do so will result
		 * in undefined behavior.
		 * @param name The name of the collapsible header. This name is used as the label for the header and as the ID
		 * for the header.
		 * @param flags Additional ImGuiTreeNodeFlags to customize the behavior and appearance of the header.
		 * @return bool True if the header is open and ready for items to be added, false otherwise.
		 */
		bool BeginHeaderCollapse(const char* name, ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None);

		/**
		 * @brief Ends the current collapsible header.
		 * @warning This function must always be paired with a call to BeginHeaderCollapse(). Failure to do so will
		 * result in undefined behavior.
		 */
		void EndHeaderCollapse();
	} // namespace Layout

	// --------------------------------
	//			  COMPONENTS
	// --------------------------------

	namespace Components
	{

		typedef int OutlineFlags;
		enum OutlineFlags_
		{
			OutlineFlags_None              = 0,      // draw no activity outline
			OutlineFlags_WhenHovered       = 1 << 1, // draw an outline when item is hovered
			OutlineFlags_WhenActive        = 1 << 2, // draw an outline when item is active
			OutlineFlags_HighlightActive   = 1 << 4, // when active, the outline is in highlight color
			OutlineFlags_NoHighlightActive = OutlineFlags_WhenHovered | OutlineFlags_WhenActive,

			OutlineFlags_NoOutlineInactive =
			    OutlineFlags_WhenHovered | OutlineFlags_WhenActive | OutlineFlags_HighlightActive,
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
		void ItemActivityOutline(OutlineFlags flags = OutlineFlags_All, ImColor activeCol = GUI::Theme::ActivityOutline,
		                         ImColor hoverCol = GUI::Theme::Outline, f32 rounding = GImGui->Style.FrameRounding);

		/**
		 * @brief Draws a border around the specified rectangle.
		 * @param rect The rectangle to draw a border around.
		 * @param color The color of the outline border.
		 * @param thickness The thickness of the border.
		 * @param rounding The rounding of the border.
		 * @param offsetX The offset of the border on the x-axis.
		 * @param offsetY The offset of the border on the y-axis.
		 */
		void RectangleOutline(ImRect rect, ImColor color = GUI::Theme::Outline, f32 thickness = 1.0f,
		                      f32 rounding = 0.0f, f32 offsetX = 0.0f, f32 offsetY = 0.0f);

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
		void ItemOutline(ImColor color = GUI::Theme::Outline, f32 thickness = 1.0f, f32 rounding = 0.0f,
		                 f32 offsetX = 0.0f, f32 offsetY = 0.0f);

		/**
		 * @brief Draws a horizontal line with a given thickness. Wrapper for C style formatting.
		 * @param thickness The thickness of the line.
		 */
		template <typename... Args>
		void Text(std::string_view fmt, const Args&&... args)
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
		static bool SingleLineTextInput(std::string* text, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
		{
			char buffer[N] = {};
			strncpy(buffer, text->c_str(), N);

			const f32 w   = ImGui::GetContentRegionAvail().x;
			bool modified = false;

			ImGui::SetNextItemWidth(w);

			if (ImGui::InputText("##input_text", buffer, sizeof(buffer), flags))
			{
				*text    = buffer;
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
		static bool SingleLineTextInputDeffered(std::string* text,
		                                        ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue)
		{
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
		static bool MultiLineTextInput(std::string* text, const ImVec2& size = ImVec2(0, 0),
		                               ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
		{
			char buffer[N] = {};
			strncpy(buffer, text->c_str(), N);

			const f32 w   = ImGui::GetContentRegionAvail().x;
			bool modified = false;

			ImGui::SetNextItemWidth(w);

			if (ImGui::InputTextMultiline("##input_text_multiline", buffer, sizeof(buffer), size, flags))
			{
				*text    = buffer;
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
		static bool MultiLineTextInputDeffered(std::string* text, const ImVec2& size = ImVec2(0, 0),
		                                       ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue)
		{
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
		 * @brief Creates a toggle button with customizable appearance and behavior.
		 * @param value The current state of the button (true for toggled, false for not toggled).
		 * @param whenOnLabel The label to display when the button is toggled.
		 * @param whenOffLabel The label to display when the button is not toggled.
		 * @param buttonFlags Flags to customize the button's behavior (e.g., ImGuiButtonFlags_Repeat).
		 * @return bool Whether something has changed
		 */
		bool ToggleButton(bool* value, const char* whenOnLabel, const char* whenOffLabel, bool center = true,
		                  ImGuiButtonFlags buttonFlags = ImGuiButtonFlags_None);

		/**
		 * @brief Displays a button with an image. (centered image inside button without any text)
		 *
		 * @param texture Texture to be displayed.
		 * @param size Size of the button.
		 * @param tintHovered Color of the button's icon when hovered.
		 * @return bool True if the button was pressed, false otherwise.
		 */
		bool ImageButton(const Texture2DAsset& texture, const glm::vec2& size, ImU32 tintHovered = Color::DarkGray);

		/**
		 * @brief Single option for radio button.
		 */
		template <typename T>
		struct SelectOption
		{
			std::string Label = "No label";
			T value           = 0;
		};

		/**
		 * @brief Draws a radio button in the GUI.
		 * @note Radio buttons are not idealy centered, but it's a good start.
		 * @tparam T The type of the value to represent with the radio button.
		 * @param value The value to represent with the radio button.
		 * @param options The list of options to display as radio buttons.
		 * @param center (optional) Whether the radio buttons should be centered in the GUI.
		 * @return bool Whether something has changed
		 */
		template <typename T>
		static bool RadioButton(T* value, const std::vector<SelectOption<T>>& options, bool center = true)
		{
			bool modified = false;
			const f32 w   = ImGui::GetContentRegionAvail().x;

			f32 leftOffset = 0.0f;
			for (size_t i = 0; i < options.size(); i++)
			{
				const SelectOption<T>& option = options[i];
				leftOffset += ImGui::CalcTextSize(option.Label.c_str()).x;
			}

			f32 width = ImGui::GetIO().FontGlobalScale * ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

			leftOffset += (f32)options.size() * width;

			if (center)
				GUI::MoveMousePosX((w - leftOffset) / 2 - 20.f);

			int val = (int)(*value);
			for (size_t i = 0; i < options.size(); i++)
			{
				const SelectOption<T>& option = options[i];

				if (ImGui::RadioButton(option.Label.c_str(), &val, (int)option.value))
				{
					modified = true;
					*value   = (T)val;
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
		static bool ScalarInput(T* value, T step = (T)1, T fastStep = (T)10, T min = std::numeric_limits<T>::lowest(),
		                        T max = std::numeric_limits<T>::max(), const char* format = nullptr,
		                        ImGuiInputFlags flags = ImGuiInputTextFlags_None)
		{
			bool modified = false;

			constexpr ImGuiDataType dataType = GetScalarDatatype<T>();

			const f32 w = ImGui::GetContentRegionAvail().x;

			ImGui::SetNextItemWidth(w);

			T temp = *value;
			if (ImGui::InputScalar("##input_scalar", dataType, &temp, &step, &fastStep, format, flags))
			{
				modified = true;

				*value = std::clamp(temp, min, max);
			}

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
		static bool ScalarSlider(T* value, T min = (T)1, T max = (T)10, const char* format = nullptr,
		                         ImGuiInputFlags flags = ImGuiInputTextFlags_None)
		{
			bool modified = false;

			constexpr ImGuiDataType dataType = GUI::GetScalarDatatype<T>();

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
		static bool ScalarDrag(T* value, float speed = 1.f, T min = std::numeric_limits<T>::lowest(),
		                       T max = std::numeric_limits<T>::max(), const char* format = nullptr,
		                       ImGuiInputFlags flags = ImGuiInputTextFlags_None)
		{
			bool modified = false;

			constexpr ImGuiDataType dataType = GUI::GetScalarDatatype<T>();

			const f32 w = ImGui::GetContentRegionAvail().x;

			ImGui::SetNextItemWidth(w);

			if (ImGui::DragScalar("##input_scalar", dataType, value, speed, &min, &max, format, flags))
				modified = true;

			Components::ItemActivityOutline();

			return modified;
		}

		bool Vector2Input(glm::vec2* vector, f32 resetValue = 0.f, f32 min = -FLT_MAX, f32 max = FLT_MAX,
		                  const std::string& format = "%.2f");

		bool Vector3Input(glm::vec3* vector, f32 resetValue = 0.f, f32 min = -FLT_MAX, f32 max = FLT_MAX,
		                  const std::string& format = "%.2f");

		bool Vector3ColorPicker(glm::vec3* vector, ImGuiColorEditFlags flags = ImGuiColorEditFlags_None);

		bool Vector4ColorPicker(glm::vec4* vector, ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar |
		                                                                       ImGuiColorEditFlags_AlphaPreviewHalf);

		/**
		 * @brief Draws a selectable widget in the GUI.
		 * @tparam T The type of the value.
		 * @param value The reference to the value.
		 * @param options The vector of selectable options.
		 * @param flags Additional ImGui flags (optional).
		 * @return bool Whether something has changed
		 */
		template <typename T>
		static bool Selectable(T* value, const std::vector<SelectOption<T>>& options,
		                       ImGuiComboFlags flags = ImGuiComboFlags_None)
		{
			bool modified = false;

			std::string chosenName = "";

			for (SelectOption<T> option : options)
			{
				if (*value == option.value)
				{
					chosenName = option.Label;
				}
			}

			if (ImGui::BeginCombo("##selectable_input", chosenName.data(), flags))
			{
				for (SelectOption<T> option : options)
				{
					const bool isSelected = *value == option.value;

					if (ImGui::Selectable(option.Label.c_str(), isSelected))
					{
						*value = option.value;

						modified = true;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			Components::ItemActivityOutline(Components::OutlineFlags_All, GUI::Theme::ActivityOutline,
			                                GUI::Theme::ActivityOutline);

			return modified;
		}

	} // namespace Components

	// --------------------------------
	//			  WIDGETS
	// --------------------------------

	namespace Widgets
	{

		/**
		 * @brief Search input field with a search icon and a clear button.
		 * @warning Caller is responsible for pushing unique ID if needed.
		 * @tparam N The maximum length of the search string. Higher values will not be accepted. Safe method.
		 * @param search The search string to be modified.
		 * @param flags The ImGui input text flags.
		 * @return Whether search widget was used and the value was modified.
		 */
		template <int N = 64>
		static bool SearchInput(std::string* search, const std::string& label = "",
		                        ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
		{
			const f32 posx                 = ImGui::GetCursorPosX();
			const f32 framePaddingY        = ImGui::GetStyle().FramePadding.y;
			constexpr f32 bw               = 28.f;
			constexpr f32 buttonOffset     = 11.f;
			constexpr f32 searchIconOffset = 1.f;
			constexpr f32 searchHintOffset = 20.f;

			bool modified = false;

			const bool searching = !search->empty();

			const f32 w = ImGui::GetContentRegionAvail().x;

			f32 inputWidth = 0.0f;

			{
				ScopedStyle FrameRounding(ImGuiStyleVar_FrameRounding, 3.0f);
				ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(28.0f, framePaddingY));

				ImGui::SetNextItemWidth(searching || !label.empty() ? w - bw : w);

				char buffer[N] = {};
				strncpy(buffer, search->c_str(), N);

				if (ImGui::InputText("##input_text", buffer, sizeof(buffer), flags))
				{
					*search  = buffer;
					modified = true;
				}
				inputWidth = ImGui::GetItemRectSize().x;

				Components::ItemActivityOutline();
			}

			if (searching || !label.empty())
			{
				ImGui::SameLine(w - bw + buttonOffset);
				ImGui::SetNextItemAllowOverlap();

				ScopedColor Border(ImGuiCol_Border, ImVec4{1.f, 1.f, 1.f, 0.f});
				ScopedColor Button(ImGuiCol_Button, ImVec4{1.f, 1.f, 1.f, 0.f});

				if (ImGui::Button(SW_ICON_CLOSE_OCTAGON, ImVec2{bw, ImGui::GetFrameHeight()}))
				{
					search->clear();
					modified = true;
				}

				if (IsItemHovered())
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			bool additionalOffset = searching || !label.empty();

			ImGui::SameLine(w - inputWidth - searchIconOffset + additionalOffset ? searchHintOffset : 0.f);
			ImGui::TextUnformatted(SW_ICON_MAGNIFY);

			if (!label.empty())
			{
				ImGui::SameLine(posx + searchHintOffset + additionalOffset ? 0.f : searchHintOffset);
				ImGui::TextUnformatted(searching ? "" : label.c_str());
			}
			else if (!searching)
			{
				ImGui::SameLine(posx + searchHintOffset + additionalOffset ? 0.f : searchHintOffset);
				ImGui::TextUnformatted("Type and search ...");
			}

			return modified;
		}

		/**
		 * @brief Search for assets in the GUI.
		 * @tparam T The type of the asset to search for.
		 * @param handle The handle to the asset to search for.
		 * @return bool Whether the asset was modified.
		 */
		template <typename T>
		    requires std::is_base_of_v<Asset, T>
		static bool AssetSearch(AssetHandle* handle)
		{
			bool modified = false;

			static std::string search;
			std::string tag;
			std::string fullPath;

			if (*handle)
			{
				if (AssetManager::IsValid(*handle))
				{
					const AssetMetaData& metadata = AssetManager::GetAssetMetaData(*handle);

					tag      = metadata.Path.filename().string();
					fullPath = (ProjectContext::Get()->GetAssetDirectory() / metadata.Path).string();
				}
				else
				{
					tag = "Invalid";
				}
			}

			ImRect droppable;
			droppable.Min = {ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y};
			droppable.Max = {ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
			                 ImGui::GetCursorScreenPos().y + ImGui::GetFrameHeight()};

			if (GUI::Widgets::SearchInput<128>(&search, tag))
			{
				if (search.empty())
				{
					*handle  = 0;
					modified = true;
				}
			}

			if (ImGui::BeginDragDropTargetCustom(droppable, ImGui::GetCurrentWindow()->ID))
			{
				const char* payloadName = Asset::GetStringifiedAssetType(T::GetStaticType());

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadName))
				{
					*handle = *static_cast<u64*>(payload->Data);

					modified = true;
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::IsItemHovered() && *handle)
			{
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(fullPath.c_str());
				ImGui::EndTooltip();
			}

			const ImGuiID listID = ImGui::GetID("##search_list_box");

			if (!search.empty())
			{
				if (ImGui::BeginListBox("##search_list_box", ImVec2(-FLT_MIN, 0.0f)))
				{
					const AssetRegistry& registry = AssetManager::GetRegistry();

					for (auto& [id, metadata] : registry.GetAvailableAssets())
					{
						GUI::ScopedID ID((int)id);

						if (metadata.Type != T::GetStaticType())
							continue;

						if (metadata.Path.filename().string().find(search) != std::string::npos)
						{
							const bool isSelected = *handle == id;

							if (ImGui::Selectable(metadata.Path.filename().string().c_str(), isSelected,
							                      ImGuiSelectableFlags_SpanAllColumns))
							{
								*handle = id;
								search.clear();
								modified = true;
							}
						}
					}
				}

				ImGui::EndListBox();
			}

			return modified;
		}

		/*
		 * @brief Draws a dropdown for selecting an entity in the GUI.
		 * @param ID The ID of the entity to select.
		 * @param scene The scene to select the entity from.
		 * @return bool Whether the entity was modified.
		 */
		static bool EntityDropdown(u64* ID, Scene* scene)
		{
			bool modified = false;

			const f32 framePaddingY    = ImGui::GetStyle().FramePadding.y;
			constexpr f32 bw           = 28.f;
			constexpr f32 buttonOffset = 11.f;
			const f32 w                = ImGui::GetContentRegionAvail().x;
			constexpr f32 hintOffset   = 28.f;

			Entity entity;
			std::string tag = "none";

			if (*ID)
			{
				entity = scene->TryGetEntityByID(*ID);
				tag    = entity.GetTag();
			}

			f32 inputWidth = 0.f;

			{
				GUI::ScopedStyle FrameRounding(ImGuiStyleVar_FrameRounding, 3.0f);
				GUI::ScopedStyle FramePadding(ImGuiStyleVar_FramePadding, ImVec2(28.0f, framePaddingY));

				ImGui::Button("##entity_dropdown_property", {(*ID ? w - bw : w) - 2.f, ImGui::GetFrameHeight()});
				inputWidth = ImGui::GetItemRectSize().x;

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
					{
						Entity* payloadEntity = static_cast<Entity*>(payload->Data);

						*ID = payloadEntity->GetID();

						modified = true;
					}
					ImGui::EndDragDropTarget();
				}

				if (ImGui::IsItemHovered() && entity)
				{
					ImGui::BeginTooltip();
					ImGui::Text("ID: %llu", *ID);
					ImGui::Text("Tag: %s", tag.c_str());
					ImGui::EndTooltip();
				}

				GUI::Components::ItemActivityOutline();
			}

			if (*ID)
			{
				ImGui::SameLine(w - bw + buttonOffset);
				ImGui::SetNextItemAllowOverlap();

				GUI::ScopedColor Border(ImGuiCol_Border, ImVec4{1.f, 1.f, 1.f, 0.f});
				GUI::ScopedColor Button(ImGuiCol_Button, ImVec4{1.f, 1.f, 1.f, 0.f});

				if (ImGui::Button(SW_ICON_CLOSE_OCTAGON, ImVec2{bw, ImGui::GetFrameHeight()}))
				{
					*ID      = 0;
					modified = true;
				}

				if (GUI::IsItemHovered())
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			if (entity)
			{
				ImGui::SameLine(w - inputWidth);
				ImGui::TextUnformatted(tag.c_str());
			}
			else
			{
				ImGui::SameLine(w - inputWidth + hintOffset);
				ImGui::TextUnformatted("None");
			}

			return modified;
		}

		/**
		 * @brief Draws a dropdown table for a vector of assets in the GUI.
		 * @tparam T The type of the asset to display in the dropdown table.
		 * @param vector The vector of assets to display in the dropdown table.
		 * @return bool Whether the dropdown table was used and the value was modified.
		 */
		template <typename T>
		    requires std::is_base_of_v<Asset, T>
		static bool AssetDropdownTable(std::vector<T**>* vector)
		{
			bool modified = false;

			constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg |
			                                  ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable |
			                                  ImGuiTableFlags_ScrollY;

			int removeAt = -1;

			if (ImGui::BeginTable("asset_dropdown_table", 2, flags))
			{
				ImGui::TableSetupColumn(Asset::GetStringifiedAssetType(T::GetStaticType()));
				ImGui::TableSetupColumn("");
				ImGui::TableHeadersRow();

				for (int i = 0; i < (int)vector->size(); i++)
				{
					T** element = vector->at(i);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					const std::string id = "##table_element" + i;

					const AssetMetaData& metadata = AssetManager::GetAssetMetaData((*element)->GetHandle());

					ImGui::PushID(id.c_str());

					ImGui::Text("%s", metadata.Path.string().c_str());

					ImGui::TableNextColumn();

					if (ImGui::Button(SW_ICON_CLOSE_OCTAGON, {ImGui::GetFrameHeight(), ImGui::GetFrameHeight()}))
						removeAt = i;

					ImGui::SameLine();

					if (i > 0)
					{
						if (ImGui::Button(SW_ICON_ARROW_UP_BOLD, {ImGui::GetFrameHeight(), ImGui::GetFrameHeight()}))
						{
							std::swap(vector->at(i), vector->at(i - 1));
							modified = true;
						}
					}
					else
					{
						ImGui::Dummy({ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
					}

					ImGui::SameLine();

					if (i < (int)vector->size() - 1)
					{
						if (ImGui::Button(SW_ICON_ARROW_DOWN_BOLD, {ImGui::GetFrameHeight(), ImGui::GetFrameHeight()}))
						{
							std::swap(vector->at(i), vector->at(i + 1));
							modified = true;
						}
					}
					else
					{
						ImGui::Dummy({ImGui::GetFrameHeight(), ImGui::GetFrameHeight()});
					}

					ImGui::PopID();
				}

				ImGui::EndTable();
			}

			if (ImGui::BeginDragDropTarget())
			{
				const char* payloadName = Asset::GetStringifiedAssetType(T::GetStaticType());

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadName))
				{
					u64 handle = *static_cast<u64*>(payload->Data);

					T** newElement = AssetManager::GetAssetRaw<T>(handle);
					vector->emplace_back(newElement);

					modified = true;
				}
				ImGui::EndDragDropTarget();
			}

			if (removeAt > -1)
			{
				vector->erase(vector->begin() + removeAt);
				modified = true;
			}

			return modified;
		}

		template <typename K, typename V>
		    requires std::is_base_of_v<Asset, V>
		static bool AssetDropdownTableMap(std::unordered_map<K, V**>* map)
		{
			bool modified = false;

			constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg |
			                                  ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

			if (ImGui::BeginTable("##asset_dropdown_table_map", 3, flags))
			{
				ImGui::TableSetupColumn("Key");
				ImGui::TableSetupColumn(Asset::GetStringifiedAssetType(V::GetStaticType()));
				ImGui::TableSetupColumn("");
				ImGui::TableHeadersRow();

				for (auto it = map->begin(); it != map->end(); /* no increment here */)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					std::string key = it->first;

					ImGui::PushID(it->first.c_str());
					if (GUI::Components::SingleLineTextInputDeffered<64>(&key))
					{
						auto value  = it->second;
						it          = map->erase(it);
						(*map)[key] = value;
						modified    = true;
						ImGui::PopID();
						continue; // Skip the increment because we've modified the map
					}

					ImGui::TableNextColumn();

					const AssetMetaData& metadata = AssetManager::GetAssetMetaData((*it->second)->GetHandle());

					ImGui::Text("%s", metadata.Path.stem().string().c_str());

					if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
					{
						GUI::ScopedStyle Padding(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
						ImGui::BeginTooltip();
						ImGui::Text("ID: %llu", metadata.Handle);
						ImGui::Text("Path: %s", metadata.Path.string().c_str());
						ImGui::EndTooltip();
					}

					ImGui::TableNextColumn();

					if (ImGui::Button(SW_ICON_CLOSE_OCTAGON, {ImGui::GetFrameHeight(), ImGui::GetFrameHeight()}))
					{
						auto value = it->second;
						it         = map->erase(it);
						modified   = true;
						ImGui::PopID();
						continue; // Skip the increment because we've modified the map
					}

					ImGui::PopID();

					++it;
				}

				ImGui::EndTable();
			}

			if (ImGui::BeginDragDropTarget())
			{
				const char* payloadName = Asset::GetStringifiedAssetType(V::GetStaticType());

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadName))
				{
					u64 handle = *static_cast<u64*>(payload->Data);

					V** newElement                 = AssetManager::GetAssetRaw<V>(handle);
					(*map)[std::to_string(rand())] = newElement;

					modified = true;
				}
				ImGui::EndDragDropTarget();
			}

			return modified;
		}

		/**
		 * @brief Draws a file picker in the GUI.
		 * @param path The path to the file to be selected.
		 * @param relative The relative path to the file.
		 * @return bool Whether the file picker was used and the value was modified.
		 */
		bool DrawFolderPicker(std::filesystem::path* path, const std::filesystem::path& relative);

		/**
		 * @brief Draws a file picker in the GUI.
		 * @param path The path to the file to be selected.
		 * @param relative The relative path to the file.
		 * @param filters The list of filters to apply to the file picker. eg. { { "All Files", "*" } } or { { "Text
		 * Files", "txt" } }
		 * @return bool Whether the file picker was used and the value was modified.
		 */
		bool DrawFilePicker(std::filesystem::path* path, const std::filesystem::path& relative,
		                    const std::initializer_list<FileDialogFilterItem> filters);

		/**
		 * @brief Draws a table of Vector2 values in the GUI.
		 * @param vector The vector of Vector2 values to display in the table.
		 * @param defaultValue The default value to use for empty cells in the table.
		 * @return bool Whether the table was used and the value was modified.
		 */
		bool Vector2Table(std::vector<glm::vec2>* vector, f32 defaultValue = 0.f);

		/**
		 * @brief Draws a table of Vector3 values in the GUI.
		 * @param vector The vector of Vector3 values to display in the table.
		 * @param defaultValue The default value to use for empty cells in the table.
		 * @return bool Whether the table was used and the value was modified.
		 */
		bool Vector3Table(std::vector<glm::vec3>* vector, f32 defaultValue = 0.f);

		/**
		 * @brief Draws a scale in the GUI.
		 * @param from The starting point of the scale.
		 * @param to The ending point of the scale.
		 * @param majorUnit The major unit of the scale.
		 * @param minorUnit The minor unit of the scale.
		 * @param labelAlignment The alignment of the labels.
		 * @param sign The sign of the scale.
		 */
		void DrawScale(const ImVec2& from, const ImVec2& to, f32 majorUnit, f32 minorUnit, f32 labelAlignment,
		               f32 sign = 1.0f);

		/**
		 * @brief Draws a splitter in the GUI.
		 * @param splitVertically Whether the splitter should split vertically.
		 * @param thickness The thickness of the splitter.
		 * @param size1 The size of the first split.
		 * @param size2 The size of the second split.
		 * @param minSize1 The minimum size of the first split.
		 * @param minSize2 The minimum size of the second split.
		 * @param splitterLongAxisSize The long axis size of the splitter.
		 * @return bool Whether the splitter was used and the value was modified.
		 */
		bool Splitter(bool splitVertically, f32 thickness, f32* size1, f32* size2, f32 minSize1, f32 minSize2,
		              f32 splitterLongAxisSize = -1.0f);

		/**
		 * @brief Renders clipped text on the GUI.
		 * @param drawList Pointer to the ImDrawList object used for rendering.
		 * @param posMin The minimum position of the text bounding box.
		 * @param posMax The maximum position of the text bounding box.
		 * @param text The text to render.
		 * @param textDisplayEnd Optional pointer to the end of the text to render.
		 * @param textSizeIfKnown Optional pointer to the known size of the text.
		 * @param align The alignment of the text within the bounding box.
		 * @param cliprect Optional pointer to the clipping rectangle.
		 * @param wrapWidth The width at which the text should wrap.
		 */
		void ClippedText(ImDrawList* drawList, const ImVec2& posMin, const ImVec2& posMax, const char* text,
		                 const char* textDisplayEnd, const ImVec2* textSizeIfKnown, const ImVec2& align,
		                 const ImRect* cliprect, f32 wrapWidth);

		/**
		 * @brief Renders clipped text on the GUI.
		 * @param posMin The minimum position of the text bounding box.
		 * @param posMax The maximum position of the text bounding box.
		 * @param text The text to render.
		 * @param textEnd Optional pointer to the end of the text to render.
		 * @param textSizeIfKnown Optional pointer to the known size of the text.
		 * @param align The alignment of the text within the bounding box.
		 * @param clipRect Optional pointer to the clipping rectangle.
		 * @param wrapWidth The width at which the text should wrap.
		 */
		void ClippedText(const ImVec2& posMin, const ImVec2& posMax, const char* text, const char* textEnd,
		                 const ImVec2* textSizeIfKnown, const ImVec2& align, const ImRect* clipRect, f32 wrapWidth);
	} // namespace Widgets

	// --------------------------------
	//			  PROPERTIES
	// --------------------------------

	namespace Properties
	{

		/**
		 * Begins a table for displaying properties in the GUI.
		 * @warning This function must always be paired with a call to EndProperties(). Failure to do so will result in
		 * undefined behavior.
		 *
		 * @param name The name of the table.
		 * @param flags The flags to customize the table's behavior (optional).
		 */
		inline static void BeginProperties(const char* name)
		{
			constexpr ImGuiTableFlags flags =
			    ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInner;

			ImGui::BeginTable(name, 2, flags);
			ImGui::TableSetupColumn("PropertyName", 0, 0.5f);
			ImGui::TableSetupColumn("Property");
		}

		/**
		 * Ends a table for displaying properties in the GUI.
		 * @warning This function must always be paired with a call to BeginProperties(). Failure to do so will result
		 * in undefined behavior.
		 */
		inline static void EndProperties()
		{
			ImGui::EndTable();
		}

		/**
		 * @brief Begins a property grid section in the GUI. This function starts a new row in the ImGui table
		 * 		  and sets up the necessary layout for a property grid.
		 * @warning This function must always be paired with a call to EndPropertyGrid(). Failure to do so will result
		 * in undefined behavior.
		 * @param label The label for the property grid section.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param rightAlignNextColumn Whether to right-align the next column in the table.
		 */
		inline static void BeginPropertyGrid(const char* label, const char* tooltip = nullptr)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::PushID(label);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y * 0.5f);
			ImGui::TextUnformatted(label);

			if (tooltip)
			{
				ImGui::SameLine();
				HelpMarker(tooltip);
			}

			ImGui::TableNextColumn();

			ImGui::SetNextItemWidth(-FLT_MIN);
		}

		/**
		 * @brief Ends a property grid section in the GUI. This function ends the current row in the ImGui table.
		 * @warning This function must always be paired with a call to BeginPropertyGrid(). Failure to do so will result
		 * in undefined behavior.
		 */
		inline static void EndPropertyGrid()
		{
			ImGui::PopID();
		}

		/**
		 * @brief Draws a single-line text input property in the GUI.
		 * @tparam N The maximum length of the text input.
		 * @param text The text to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param flags Additional ImGui flags (optional).
		 * @returns bool Whether the text was modified.
		 */
		template <int N = 64>
		static bool SingleLineTextInputProperty(std::string* text, const char* label, const char* toooltip = nullptr,
		                                        ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
		{
			Properties::BeginPropertyGrid(label, toooltip);

			bool modified = Components::SingleLineTextInput<N>(text, flags);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws a single-line text input property in the GUI with deferred input.
		 * @tparam N The maximum length of the text input.
		 * @param text The text to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param flags Additional ImGui flags (optional).
		 * @returns bool Whether the text was modified.
		 */
		template <int N = 64>
		static bool SingleLineTextInputDefferedProperty(
		    std::string* text, const char* label, const char* tooltip = nullptr,
		    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::SingleLineTextInput<N>(text, flags);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws a multi-line text input property in the GUI.
		 * @tparam N The maximum length of the text input.
		 * @param text The text to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param size The size of the text input box.
		 * @param flags Additional ImGui flags (optional).
		 * @returns bool Whether the text was modified.
		 */
		template <int N = 128>
		static bool MultiLineTextInputProperty(std::string* text, const char* label, const char* tooltip = nullptr,
		                                       const ImVec2& size        = ImVec2(0, 0),
		                                       ImGuiInputTextFlags flags = ImGuiInputTextFlags_None)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::MultiLineTextInput<N>(text, size, flags);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws a multi-line text input property in the GUI with deferred input.
		 * @tparam N The maximum length of the text input.
		 * @param text The text to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param size The size of the text input box.
		 * @param flags Additional ImGui flags (optional).
		 * @returns bool Whether the text was modified.
		 */
		template <int N = 128>
		static bool MultiLineTextInputDefferedProperty(std::string* text, const char* label,
		                                               const char* tooltip = nullptr, const ImVec2& size = ImVec2(0, 0),
		                                               ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::MultiLineTextInput<N>(text, size, flags);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws a checkbox property in the GUI.
		 * @param value The value to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param center Whether to center the checkbox.
		 * @returns bool Whether the checkbox was modified.
		 */
		bool CheckboxProperty(bool* value, const char* label, const char* tooltip = nullptr, bool center = true);

		/**
		 * @brief Draws a toggle property in the GUI.
		 * @param value The value to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param center Whether to center the toggle.
		 * @returns bool Whether the toggle was modified.
		 */
		bool ToggleProperty(bool* value, const char* label, const char* tooltip = nullptr, bool center = true);

		/**
		 * @brief Draws a button property in the GUI.
		 * @param icon The icon to display on the button.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param size The size of the button.
		 * @returns bool Whether the button was modified.
		 */
		bool ButtonProperty(const char* icon, const char* label, const char* tooltip = nullptr,
		                    glm::vec2 size = glm::vec2(40.0f));

		/**
		 * @brief Draws a toggle button property in the GUI.
		 * @param value The value to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param whenOnLabel The label to display when the toggle is on.
		 * @param whenOffLabel The label to display when the toggle is off.
		 * @param center Whether to center the toggle button.
		 * @param buttonFlags Additional ImGui button flags (optional).
		 * @returns bool Whether the toggle button was modified.
		 */
		bool ToggleButtonProperty(bool* value, const char* label, const char* tooltip = nullptr,
		                          const char* whenOnLabel = nullptr, const char* whenOffLabel = nullptr,
		                          bool center = true, ImGuiButtonFlags buttonFlags = ImGuiButtonFlags_None);

		/**
		 * @brief Draws a radio button property in the GUI.
		 * @tparam T The type of the value to modify.
		 * @param value The value to modify.
		 * @param options The list of options to display in the radio button.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param center Whether to center the radio button.
		 * @returns bool Whether the radio button was modified.
		 */
		template <typename T>
		static bool RadioButtonProperty(T* value, const std::vector<Components::SelectOption<T>>& options,
		                                const char* label, const char* tooltip = nullptr, bool center = true)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::RadioButton<T>(value, options, center);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws a scalar input property in the GUI.
		 * @tparam T The type of the value to modify.
		 * @param value The value to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param step The step value for the scalar input.
		 * @param fastStep The fast step value for the scalar input.
		 * @param format The format string for the scalar input.
		 * @param flags Additional ImGui input flags (optional).
		 * @returns bool Whether the scalar input was modified.
		 */
		template <typename T>
		    requires std::is_scalar_v<T>
		static bool ScalarInputProperty(T* value, const char* label, const char* tooltip = nullptr, T step = (T)1,
		                                T fastStep = (T)10, T min = std::numeric_limits<T>::lowest(),
		                                T max = std::numeric_limits<T>::max(), const char* format = nullptr,
		                                ImGuiInputFlags flags = ImGuiInputTextFlags_None)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::ScalarInput(value, step, fastStep, min, max, format, flags);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws a scalar slider property in the GUI.
		 * @tparam T The type of the value to modify.
		 * @param value The value to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param min The minimum value for the scalar slider.
		 * @param max The maximum value for the scalar slider.
		 * @param format The format string for the scalar slider.
		 * @param flags Additional ImGui input flags (optional).
		 * @returns bool Whether the scalar slider was modified.
		 */
		template <typename T>
		    requires std::is_scalar_v<T>
		static bool ScalarSliderProperty(T* value, const char* label, const char* tooltip = nullptr, T min = (T)1,
		                                 T max = (T)10, const char* format = nullptr,
		                                 ImGuiInputFlags flags = ImGuiInputTextFlags_None)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::ScalarSlider(value, min, max, format, flags);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws a scalar drag property in the GUI.
		 * @tparam T The type of the value to modify.
		 * @param value The value to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param speed The speed value for the scalar drag.
		 * @param min The minimum value for the scalar drag.
		 * @param max The maximum value for the scalar drag.
		 * @param format The format string for the scalar drag.
		 * @param flags Additional ImGui input flags (optional).
		 * @returns bool Whether the scalar drag was modified.
		 */
		template <typename T>
		    requires std::is_scalar_v<T>
		static bool ScalarDragProperty(T* value, const char* label, const char* tooltip = nullptr, float speed = 1.f,
		                               T min = std::numeric_limits<T>::lowest(), T max = std::numeric_limits<T>::max(),
		                               const char* format = nullptr, ImGuiInputFlags flags = ImGuiInputTextFlags_None)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::ScalarDrag(value, speed, min, max, format, flags);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws a vector2 input property in the GUI.
		 * @param vector The vector to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param resetValue The reset value for the vector input.
		 * @param min The minimum value for the vector input.
		 * @param max The maximum value for the vector input.
		 * @param format The format string for the vector input.
		 * @returns bool Whether the vector input was modified.
		 */
		bool Vector2InputProperty(glm::vec2* vector, const char* label, const char* tooltip = nullptr,
		                          f32 resetValue = 0.f, f32 min = -FLT_MAX, f32 max = FLT_MAX,
		                          const std::string& format = "%.2f");

		/**
		 * @brief Draws a vector3 input property in the GUI.
		 * @param vector The vector to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param resetValue The reset value for the vector input.
		 * @param min The minimum value for the vector input.
		 * @param max The maximum value for the vector input.
		 * @param format The format string for the vector input.
		 * @returns bool Whether the vector input was modified.
		 */
		bool Vector3InputProperty(glm::vec3* vector, const char* label, const char* tooltip = nullptr,
		                          f32 resetValue = 0.f, f32 min = -FLT_MAX, f32 max = FLT_MAX,
		                          const std::string& format = "%.2f");

		/**
		 * @brief Draws a vector4 input property in the GUI.
		 * @param vector The vector to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param resetValue The reset value for the vector input.
		 * @param min The minimum value for the vector input.
		 * @param max The maximum value for the vector input.
		 * @param format The format string for the vector input.
		 * @returns bool Whether the vector input was modified.
		 */
		bool Vector3ColorPickerProperty(glm::vec3* vector, const char* label, const char* tooltip = nullptr,
		                                ImGuiColorEditFlags flags = ImGuiColorEditFlags_None);

		/**
		 * @brief Draws a vector4 input property in the GUI.
		 * @param vector The vector to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param flags Additional ImGui color edit flags (optional).
		 * @returns bool Whether the vector input was modified.
		 */
		bool Vector4ColorPickerProperty(glm::vec4* vector, const char* label, const char* tooltip = nullptr,
		                                ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar |
		                                                            ImGuiColorEditFlags_AlphaPreviewHalf);

		/**
		 * @brief Draws a selectable property in the GUI.
		 * @tparam T The type of the value to modify.
		 * @param value The value to modify.
		 * @param options The list of options to display in the selectable.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @param flags Additional ImGui combo flags (optional).
		 * @returns bool Whether the selectable was modified.
		 */
		template <typename T>
		static bool SelectableProperty(T* value, const std::vector<Components::SelectOption<T>>& options,
		                               const char* label, const char* tooltip = nullptr,
		                               ImGuiComboFlags flags = ImGuiComboFlags_None)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Components::Selectable<T>(value, options, flags);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws an asset search property in the GUI.
		 * @tparam T The type of the asset to search for.
		 * @param handle The handle to the asset to modify.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @returns bool Whether the asset search was modified.
		 */
		template <typename T>
		    requires std::is_base_of_v<Asset, T>
		static bool AssetSearchProperty(AssetHandle* handle, const char* label, const char* tooltip = nullptr)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Widgets::AssetSearch<T>(handle);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws an entity dropdown property in the GUI.
		 * @param ID The ID of the entity to modify.
		 * @param scene The scene to search for the entity.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @returns bool Whether the entity dropdown was modified.
		 */
		bool EntityDropdownProperty(u64* ID, Scene* scene, const char* label, const char* tooltip = nullptr);

		/**
		 * @brief Draws an entity dropdown table property in the GUI.
		 * @param vector The vector of entity IDs to display in the dropdown table.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @returns bool Whether the entity dropdown table was modified.
		 */
		template <typename T>
		    requires std::is_base_of_v<Asset, T>
		static bool AssetDropdownTableProperty(std::vector<T**>* vector, const char* label,
		                                       const char* tooltip = nullptr)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Widgets::AssetDropdownTable<T>(vector);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws an asset dropdown table map property in the GUI.
		 * @tparam K The type of the key in the map.
		 * @tparam V The type of the value in the map.
		 * @param map The map of asset pointers to display in the dropdown table.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @returns bool Whether the asset dropdown table map was modified.
		 */
		template <typename K, typename V>
		    requires std::is_base_of_v<Asset, V>
		static bool AssetDropdownTableMapProperty(std::unordered_map<K, V**>* map, const char* label,
		                                          const char* tooltip = nullptr)
		{
			Properties::BeginPropertyGrid(label, tooltip);

			bool modified = Widgets::AssetDropdownTableMap<K, V>(map);

			Properties::EndPropertyGrid();

			return modified;
		}

		/**
		 * @brief Draws a folder picker property in the GUI.
		 * @param path The path to the folder to modify.
		 * @param relative The relative path to the folder.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @returns bool Whether the folder picker was modified.
		 */
		bool DrawFolderPickerProperty(std::filesystem::path* path, const std::filesystem::path& relative,
		                              const char* label = "Folder", const char* tooltip = nullptr);

		/**
		 * @brief Draws a file picker property in the GUI.
		 * @param path The path to the file to modify.
		 * @param relative The relative path to the file.
		 * @param filters The list of filters to apply to the file picker.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @returns bool Whether the file picker was modified.
		 */
		bool DrawFilePickerProperty(std::filesystem::path* path, const std::filesystem::path& relative,
		                            const std::initializer_list<FileDialogFilterItem> filters,
		                            const char* label = "File", const char* tooltip = nullptr);

		/**
		 * @brief Draws a table of Vector2 values in the GUI.
		 * @param vector The vector of Vector2 values to display in the table.
		 * @param defaultValue The default value to use for empty cells in the table.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @returns bool Whether the table was modified.
		 */
		bool Vector2TableProperty(std::vector<glm::vec2>* vector, const char* label = "File",
		                          const char* tooltip = nullptr, f32 defaultValue = 0.f);

		/**
		 * @brief Draws a table of Vector3 values in the GUI.
		 * @param vector The vector of Vector3 values to display in the table.
		 * @param defaultValue The default value to use for empty cells in the table.
		 * @param label The label for the property.
		 * @param tooltip An optional tooltip to display when hovering over the label.
		 * @returns bool Whether the table was modified.
		 */
		bool Vector3TableProperty(std::vector<glm::vec3>* vector, const char* label = "File",
		                          const char* tooltip = nullptr, f32 defaultValue = 0.f);

		/**
		 * @brief Draws a part of an image with customizable properties.
		 *
		 * @param wholeImage Pointer to the Texture2DAsset object representing the whole image.
		 * @param label The label for the property grid.
		 * @param tooltip Optional tooltip for the property grid.
		 * @param offset The offset of the image part within the whole image.
		 * @param size The size of the image part.
		 * @param tint The tint color of the image part.
		 * @param showBorder Show border around the image.
		 */
		void DrawImagePartProperty(Texture2DAsset* wholeImage, const char* label, const char* tooltip = nullptr,
		                           glm::vec2 offset = glm::vec2(0.0f), glm::vec2 size = glm::vec2(0.0f),
		                           glm::vec4 tint = glm::vec4(1.0f), bool showBorder = false);
	} // namespace Properties

} // namespace SW::GUI
