/**
 * @file Appearance.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-08
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Math/Vector2.hpp"
#include "Core/Math/Vector4.hpp"

namespace SW {

	namespace GUI {

		/**
		 * @brief Position of the element in the window
		 */
		enum ElementPosition : i8 {
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
			Vector2<f32> WindowPadding = { 12.0f, 12.0f };
			Vector2<f32> WindowMinSize = { 20.0f, 20.0f };
			Vector2<f32> WindowTitleAlign = { 0.5f, 0.5f };

			f32 ChildRounding = 0.0f;
			f32 ChildBorderSize = 1.0f;

			f32 PopupRounding = 0.0f;
			f32 PopupBorderSize = 1.0f;

			Vector2<f32> FramePadding = { 6.0f, 6.0f };
			f32 FrameRounding = 0.0f;
			f32 FrameBorderSize = 0.0f;

			Vector2<f32> ItemSpacing = { 12.0f, 6.0f };
			Vector2<f32> ItemInnerSpacing = { 6.0f, 3.0f };
			Vector2<f32> CellPadding = { 12.0f, 6.0f };

			f32 IndentSpacing = 20.0f;

			f32 ColumnsMinSpacing = 6.0f;

			f32 ScrollbarSize = 12.0f;
			f32 ScrollbarRounding = 0.0f;

			f32 GrabMinSize = 12.0f;
			f32 GrabRounding = 0.0f;

			f32 TabRounding = 0.0f;
			f32 TabBorderSize = 0.0f;

			f32 TabMinWidthForCloseButton = 0.0f;

			ElementPosition ColorButtonPosition = ElementPosition::Right;
			Vector2<f32> ButtonTextAlign = { 0.5f, 0.5f };
			Vector2<f32> SelectableTextAlign = { 0.0f, 0.0f };
		};

		/**
		 * @brief Colors of the GUI
		 */
		struct Colors final
		{
			Vector4<f32> Text = { 1.0f, 1.0f, 1.0f, 1.0f };
			Vector4<f32> TextDisabled = { 0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f };

			Vector4<f32> WindowBackground = { 0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f };
			Vector4<f32> ChildBackground = { 0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f };
			Vector4<f32> PopupBackground = { 0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f };

			Vector4<f32> Border = { 0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f };
			Vector4<f32> BorderShadow = { 0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f };

			Vector4<f32> FrameBackground = { 0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f };
			Vector4<f32> FrameBackgroundHovered = { 0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f };
			Vector4<f32> FrameBackgroundActive = { 0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f };
			
			Vector4<f32> TitleBackground = { 0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f };
			Vector4<f32> TitleBackgroundActive = { 0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f };
			Vector4<f32> TitleBackgroundCollapsed = { 0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f };

			Vector4<f32> MenuBarBackground = { 0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f };

			Vector4<f32> ScrollbarBackground = { 0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f };
			Vector4<f32> ScrollbarGrab = { 0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f };
			Vector4<f32> ScrollbarGrabHovered = { 0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f };
			Vector4<f32> ScrollbarGrabActive = { 0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f };

			Vector4<f32> CheckMark = { 0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f };

			Vector4<f32> SliderGrab = { 0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f };
			Vector4<f32> SliderGrabActive = { 0.5372549295425415f, 0.5529412031173706f, 1.0f, 1.0f };

			Vector4<f32> Button = { 0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f };
			Vector4<f32> ButtonHovered = { 0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f };
			Vector4<f32> ButtonActive = { 0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f };

			Vector4<f32> Header = { 0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f };
			Vector4<f32> HeaderHovered = { 0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f };
			Vector4<f32> HeaderActive = { 0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f };

			Vector4<f32> Separator = { 0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f };
			Vector4<f32> SeparatorHovered = { 0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f };
			Vector4<f32> SeparatorActive = { 0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f }; 

			Vector4<f32> ResizeGrip = { 0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f };
			Vector4<f32> ResizeGripHovered = { 0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f };
			Vector4<f32> ResizeGripActive = { 0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f };

			Vector4<f32> Tab = { 0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f };
			Vector4<f32> TabHovered = { 0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f };
			Vector4<f32> TabActive = { 0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f };
			Vector4<f32> TabUnfocused = { 0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f };
			Vector4<f32> TabUnfocusedActive = { 0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f };

			Vector4<f32> PlotLines = { 0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f };
			Vector4<f32> PlotLinesHovered = { 0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f };
			Vector4<f32> PlotHistogram = { 1.0f, 0.2901960909366608f, 0.5960784554481506f, 1.0f };
			Vector4<f32> PlotHistogramHovered = { 0.9960784316062927f, 0.4745098054409027f, 0.6980392336845398f, 1.0f };

			Vector4<f32> TableHeaderBackground = { 0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f };
			Vector4<f32> TableBorderStrong = { 0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f };
			Vector4<f32> TableBorderLight = { 0.0f, 0.0f, 0.0f, 1.0f };
			Vector4<f32> TableRowBackground = { 0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f };
			Vector4<f32> TableRowBackgroundAlt = { 0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f };

			Vector4<f32> TextSelectedBackground = { 0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f };

			Vector4<f32> DragDropTarget = { 0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f };

			Vector4<f32> NavHighlight = { 0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f };
			Vector4<f32> NavWindowingHighlight = { 0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f };
			Vector4<f32> NavWindowingDimBackground = { 0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f };

			Vector4<f32> ModalWindowDimBackground = { 0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f };
		};

		/**
		 * @brief Applies style to the GUI
		 */
		class Appearance final {
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
		};

	}

}
