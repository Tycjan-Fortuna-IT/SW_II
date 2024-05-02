/**
 * @file Appearance.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.5
 * @date 2024-03-16
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

struct ImFont;

namespace SW {

	namespace GUI {

		namespace Theme {
			// color of e.g. an active input's outline(border)
			constexpr u32 ActivityOutline = IM_COL32(236, 158, 36, 255);
			
			// color of a casual outline(border)
			constexpr u32 Outline = IM_COL32(60, 60, 60, 255);

			constexpr u32 Selection = IM_COL32(237, 192, 119, 255);

			constexpr u32 SelectionMuted = IM_COL32(237, 201, 142, 23);
			
			constexpr u32 SelectionHalfMuted = IM_COL32(237, 201, 142, 127);

			constexpr u32 SelectionDark = IM_COL32(143, 115, 73, 255);

			constexpr u32 Border = IM_COL32(28, 28, 28, 255);

			constexpr u32 Titlebar = IM_COL32(43, 43, 43, 255);

			constexpr u32 TitlebarActive = IM_COL32(56, 56, 56, 255);

			constexpr u32 Accent = IM_COL32(236, 158, 36, 255);
			constexpr u32 Highlight = IM_COL32(39, 185, 242, 255);
			constexpr u32 NiceBlue = IM_COL32(83, 232, 254, 255);
			constexpr u32 Compliment = IM_COL32(78, 151, 166, 255);
			constexpr u32 Background = IM_COL32(36, 36, 36, 255);
			constexpr u32 BackgroundDark = IM_COL32(26, 26, 26, 255);
			constexpr u32 PropertyField = IM_COL32(15, 15, 15, 255);
			constexpr u32 Text = IM_COL32(192, 192, 192, 255);
			constexpr u32 TextBrighter = IM_COL32(210, 210, 210, 255);
			constexpr u32 TextDarker = IM_COL32(128, 128, 128, 255);
			constexpr u32 TextError = IM_COL32(230, 51, 51, 255);
			constexpr u32 Muted = IM_COL32(77, 77, 77, 255);
			constexpr u32 Header = IM_COL32(47, 47, 47, 255);
			constexpr u32 HeaderHovered = IM_COL32(92, 90, 90, 255);
			constexpr u32 BackgroundPopup = IM_COL32(50, 50, 50, 255);
			constexpr u32 ValidPrefab = IM_COL32(82, 179, 222, 255);
			constexpr u32 InvalidPrefab = IM_COL32(222, 43, 43, 255);
			constexpr u32 MissingMesh = IM_COL32(230, 102, 76, 255);
			constexpr u32 MissingMeshTrans = IM_COL32(230, 102, 76, 150);
			constexpr u32 MeshNotSet = IM_COL32(250, 101, 23, 255);
			constexpr u32 TabActive = IM_COL32(128, 128, 128, 150);
		}

		/**
		 * @brief Position of the element in the window
		 */
		enum ElementPosition : i8
		{
			None = -1,
			Left = 0,
			Right = 1,
			Up = 2,
			Down = 3,
		};

		/**
		 * @brief Style of the GUI
		 */
		struct Style final
		{
			f32 Alpha = 1.0f;
			f32 DisabledAlpha = 1.0f;

			ElementPosition WindowMenuButtonPosition = ElementPosition::None;
			f32 WindowRounding = 0.0f;
			f32 WindowBorderSize = 0.0f;
			ImVec2 WindowPadding = { 12.0f, 12.0f };
			ImVec2 WindowMinSize = { 20.0f, 20.0f };
			ImVec2 WindowTitleAlign = { 0.5f, 0.5f };

			f32 ChildRounding = 0.0f;
			f32 ChildBorderSize = 1.0f;

			f32 PopupRounding = 0.0f;
			f32 PopupBorderSize = 1.0f;

			ImVec2 FramePadding = { 6.0f, 8.0f };
			f32 FrameRounding = 2.5f;
			f32 FrameBorderSize = 1.0f;

			ImVec2 ItemSpacing = { 12.0f, 6.0f };
			ImVec2 ItemInnerSpacing = { 6.0f, 3.0f };
			ImVec2 CellPadding = { 12.0f, 6.0f };

			f32 IndentSpacing = 11.0f;

			f32 ColumnsMinSpacing = 6.0f;

			f32 ScrollbarSize = 12.0f;
			f32 ScrollbarRounding = 0.0f;

			f32 GrabMinSize = 12.0f;
			f32 GrabRounding = 0.0f;

			f32 TabRounding = 0.0f;
			f32 TabBorderSize = 0.0f;

			f32 TabMinWidthForCloseButton = 0.0f;

			ElementPosition ColorButtonPosition = ElementPosition::Right;
			ImVec2 ButtonTextAlign = { 0.5f, 0.5f };
			ImVec2 SelectableTextAlign = { 0.0f, 0.0f };
		};

		/**
		 * @brief Colors of the GUI
		 */
		struct Colors final
		{
			ImVec4 Text = ImGui::ColorConvertU32ToFloat4(Theme::Text);
			ImVec4 TextDisabled = { 0.721030056476593f, 0.7210282683372498f, 0.7210228443145752f, 1.0f };

			ImVec4 WindowBackground = ImGui::ColorConvertU32ToFloat4(Theme::Titlebar);
			ImVec4 ChildBackground = ImGui::ColorConvertU32ToFloat4(Theme::Background);
			ImVec4 PopupBackground = ImGui::ColorConvertU32ToFloat4(Theme::BackgroundPopup);

			ImVec4 Border = ImGui::ColorConvertU32ToFloat4(Theme::Border);

			ImVec4 FrameBackground = ImGui::ColorConvertU32ToFloat4(Theme::PropertyField);
			ImVec4 FrameBackgroundHovered = ImGui::ColorConvertU32ToFloat4(Theme::PropertyField);
			ImVec4 FrameBackgroundActive = ImGui::ColorConvertU32ToFloat4(Theme::PropertyField);

			ImVec4 TitleBackground = ImGui::ColorConvertU32ToFloat4(Theme::Titlebar);
			ImVec4 TitleBackgroundActive = ImGui::ColorConvertU32ToFloat4(Theme::TitlebarActive);
			ImVec4 TitleBackgroundCollapsed = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

			ImVec4 MenuBarBackground = { 0.0f, 0.0f, 0.0f, 0.0f };

			ImVec4 ScrollbarBackground = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			ImVec4 ScrollbarGrab = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
			ImVec4 ScrollbarGrabHovered = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
			ImVec4 ScrollbarGrabActive = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

			ImVec4 CheckMark = ImGui::ColorConvertU32ToFloat4(Theme::Text);

			ImVec4 SliderGrab = { 0.51f, 0.51f, 0.51f, 0.7f };
			ImVec4 SliderGrabActive = { 0.66f, 0.66f, 0.66f, 1.0f };

			ImVec4 Button = ImColor(56, 56, 56, 200);
			ImVec4 ButtonHovered = ImColor(70, 70, 70, 255);
			ImVec4 ButtonActive = ImColor(128, 128, 128, 150);

			ImVec4 Header = ImGui::ColorConvertU32ToFloat4(Theme::Header);
			ImVec4 HeaderHovered = ImGui::ColorConvertU32ToFloat4(Theme::HeaderHovered);
			ImVec4 HeaderActive = ImGui::ColorConvertU32ToFloat4(Theme::Header);

			ImVec4 Separator = ImGui::ColorConvertU32ToFloat4(IM_COL32(44, 44, 44, 255));
			ImVec4 SeparatorActive = ImGui::ColorConvertU32ToFloat4(Theme::Highlight);
			ImVec4 SeparatorHovered = ImColor(39, 185, 242, 150);

			ImVec4 ResizeGrip = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
			ImVec4 ResizeGripHovered = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
			ImVec4 ResizeGripActive = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

			ImVec4 Tab = ImGui::ColorConvertU32ToFloat4(Theme::Titlebar);
			ImVec4 TabHovered = ImColor(128, 128, 128, 150);
			ImVec4 TabActive = ImGui::ColorConvertU32ToFloat4(Theme::TabActive);
			ImVec4 TabUnfocused = ImGui::ColorConvertU32ToFloat4(IM_COL32(47, 47, 47, 255));
			ImVec4 TabUnfocusedActive = ImGui::ColorConvertU32ToFloat4(IM_COL32(53, 53, 53, 255));

			ImVec4 PlotLines = ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 0, 255));
			ImVec4 PlotLinesHovered = { 0.8702952265739441f, 0.8969957232475281f, 0.06929580867290497f, 1.0f };
			ImVec4 PlotHistogram = { 0.6666666865348816f, 0.686274528503418f, 0.0784313753247261f, 1.0f };
			ImVec4 PlotHistogramHovered = { 0.8705882430076599f, 0.8980392217636108f, 0.07058823853731155f, 1.0f };

			ImVec4 TableHeaderBackground = ImGui::ColorConvertU32ToFloat4(Theme::Header);
			ImVec4 TableBorderLight = ImGui::ColorConvertU32ToFloat4(Theme::Border);
			ImVec4 TableBorderStrong = ImGui::ColorConvertU32ToFloat4(Theme::Border);

			ImVec4 TextSelectedBackground = { 0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f };

			ImVec4 DragDropTarget = ImGui::ColorConvertU32ToFloat4(Theme::ActivityOutline);
			ImVec4 NavHighlight = ImGui::ColorConvertU32ToFloat4(Theme::ActivityOutline);
			ImVec4 NavWindowingHighlight = ImGui::ColorConvertU32ToFloat4(Theme::ActivityOutline);
			ImVec4 NavWindowingDimBackground = { 0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f };

			ImVec4 ModalWindowDimBackground = ImGui::ColorConvertU32ToFloat4(IM_COL32(21, 21, 21, 130));

			ImVec4 WindowBackgroundTemp = { 0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f };

			static ImVec4 Darken(ImVec4 c, float p)
			{
				return { std::max(0.f, c.x - 1.0f * p), std::max(0.f, c.y - 1.0f * p), std::max(0.f, c.z - 1.0f * p), c.w };
			}

			static ImVec4 Lighten(ImVec4 c, float p)
			{
				return { std::max(0.f, c.x + 1.0f * p), std::max(0.f, c.y + 1.0f * p), std::max(0.f, c.z + 1.0f * p), c.w };
			}

			static ImVec4 Disabled(ImVec4 c) { return Darken(c, 0.6f); }
			static ImVec4 Hovered(ImVec4 c) { return Lighten(c, 0.2f); }
			static ImVec4 Active(ImVec4 c) { return Lighten(ImVec4(c.x, c.y, c.z, 1.0f), 0.1f); }
			static ImVec4 Collapsed(ImVec4 c) { return Darken(c, 0.2f); }
		};

		/**
		 * @brief Fonts specification, data used while creating fonts.
		 */
		struct FontSpecification
		{
			f32 SmallSize = 12.f;		/** @brief Size of the small font */
			f32 DefaultSize = 16.f;		/** @brief Size of the default font */
			f32 BigSize = 20.f;			/** @brief Size of the big font */
			std::string FontPath;		/** @brief Path to the .ttf file with normal font */
			std::string BoldFontPath;	/** @brief Path to the .ttf file with bold font */

			FontSpecification(const std::string& fontPath, const std::string& boldFontPath)
				: FontPath(fontPath), BoldFontPath(boldFontPath) {} 
		};

		/**
		 * @brief Fonts of the GUI
		 */
		struct Fonts final
		{
			f32 SmallSize = 12.f;		/** @brief Size of the small font */
			f32 DefaultSize = 16.f;		/** @brief Size of the default font */
			f32 BigSize = 20.f;			/** @brief Size of the big font */

			ImFont* SmallFont = nullptr;
			ImFont* DefaultFont = nullptr;
			ImFont* BigFont = nullptr;

			ImFont* SmallBoldFont = nullptr;
			ImFont* DefaultBoldFont = nullptr;
			ImFont* BigBoldFont = nullptr;
		};

		/**
		 * @brief Applies style to the GUI
		 */
		class Appearance final
		{
		public:
			/**
			 * @brief Applies style to the GUI
			 * @param style Style to apply
			 */
			static void ApplyStyle(const Style& style);

			/**
			 * @brief Applies colors to the GUI
			 * @param colors Colors to apply
			 */
			static void ApplyColors(const Colors& colors);

			/**
			 * @brief Applies fonts to the GUI
			 * @param spec Font specification to apply
			 */
			static void ApplyFonts(const FontSpecification& spec);

			/**
			 * @brief Returns style applied to the application
			 * @return Current style
			 */
			static const Style& GetStyle() { return s_Style; }

			/**
			 * @brief Returns colors applied to the application
			 * @return Current colors
			 */
			static const Colors& GetColors() { return s_Colors; }

			/**
			 * @brief Returns fonts applied to the application
			 * @return Current fonts
			 */
			static const Fonts& GetFonts() { return s_Fonts; }

		private:
			static Style s_Style;		/** @brief Current app's style */
			static Colors s_Colors;		/** @brief Current app's colors */
			static Fonts s_Fonts;		/** @brief Current app's fonts */

			/**
			 * @brief Adds icon font to the application for each font size
			 * 
			 * @param size Size of the font
			 */
			static void AppendIconFont(f32 size);
		};
	}

}
