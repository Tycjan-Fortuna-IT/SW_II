#include "Appearance.hpp"

#include <icons/MaterialDesign.inl>
#include <ImGuizmo.h>

namespace SW::GUI {

	Style Appearance::s_Style;
	Colors Appearance::s_Colors;
	Fonts Appearance::s_Fonts;

	void Appearance::ApplyStyle(const Style& style)
	{
		ImGuiStyle& s = ImGui::GetStyle();

		s_Style = style;

		s.Alpha = style.Alpha;
		s.DisabledAlpha = style.DisabledAlpha;

		s.WindowPadding = style.WindowPadding;
		s.WindowRounding = style.WindowRounding;
		s.WindowBorderSize = style.WindowBorderSize;
		s.WindowMinSize = style.WindowMinSize;
		s.WindowTitleAlign = style.WindowTitleAlign;
		s.WindowMenuButtonPosition = style.WindowMenuButtonPosition;

		s.ChildRounding = style.ChildRounding;
		s.ChildBorderSize = style.ChildBorderSize;

		s.PopupRounding = style.PopupRounding;
		s.PopupBorderSize = style.PopupBorderSize;

		s.FramePadding = style.FramePadding;
		s.FrameRounding = style.FrameRounding;
		s.FrameBorderSize = style.FrameBorderSize;

		s.ItemSpacing = style.ItemSpacing;
		s.ItemInnerSpacing = style.ItemInnerSpacing;
		s.CellPadding = style.CellPadding;

		s.IndentSpacing = style.IndentSpacing;

		s.ColumnsMinSpacing = style.ColumnsMinSpacing;

		s.ScrollbarSize = style.ScrollbarSize;
		s.ScrollbarRounding = style.ScrollbarRounding;

		s.GrabMinSize = style.GrabMinSize;
		s.GrabRounding = style.GrabRounding;

		s.TabRounding = style.TabRounding;
		s.TabBorderSize = style.TabBorderSize;

		s.TabMinWidthForCloseButton = style.TabMinWidthForCloseButton;

		s.ColorButtonPosition = style.ColorButtonPosition;
		s.ButtonTextAlign = style.ButtonTextAlign;
		s.SelectableTextAlign = style.SelectableTextAlign;
	}

	void Appearance::ApplyColors(const Colors& colors)
	{
		ImGuiStyle& s = ImGui::GetStyle();
		ImVec4* c = s.Colors;

		s_Colors = colors;

		c[ImGuiCol_Text] = colors.Text;
		c[ImGuiCol_TextDisabled] = colors.TextDisabled;

		c[ImGuiCol_WindowBg] = colors.WindowBackground;
		c[ImGuiCol_ChildBg] = colors.ChildBackground;
		c[ImGuiCol_PopupBg] = colors.PopupBackground;

		c[ImGuiCol_Border] = colors.Border;
		
		c[ImGuiCol_FrameBg] = colors.FrameBackground;
		c[ImGuiCol_FrameBgHovered] = colors.FrameBackgroundHovered;
		c[ImGuiCol_FrameBgActive] = colors.FrameBackgroundActive;

		c[ImGuiCol_TitleBg] = colors.TitleBackground;
		c[ImGuiCol_TitleBgActive] = colors.TitleBackgroundActive;
		c[ImGuiCol_TitleBgCollapsed] = colors.TitleBackgroundCollapsed;

		c[ImGuiCol_MenuBarBg] = colors.MenuBarBackground;

		c[ImGuiCol_ScrollbarBg] = colors.ScrollbarBackground;
		c[ImGuiCol_ScrollbarGrab] = colors.ScrollbarGrab;
		c[ImGuiCol_ScrollbarGrabHovered] = colors.ScrollbarGrabHovered;
		c[ImGuiCol_ScrollbarGrabActive] = colors.ScrollbarGrabActive;

		c[ImGuiCol_CheckMark] = colors.CheckMark;

		c[ImGuiCol_SliderGrab] = colors.SliderGrab;
		c[ImGuiCol_SliderGrabActive] = colors.SliderGrabActive;

		c[ImGuiCol_Button] = colors.Button;
		c[ImGuiCol_ButtonHovered] = colors.ButtonHovered;
		c[ImGuiCol_ButtonActive] = colors.ButtonActive;

		c[ImGuiCol_Header] = colors.Header;
		c[ImGuiCol_HeaderHovered] = colors.HeaderHovered;
		c[ImGuiCol_HeaderActive] = colors.HeaderActive;

		c[ImGuiCol_Separator] = colors.Separator;
		c[ImGuiCol_SeparatorHovered] = colors.SeparatorHovered;
		c[ImGuiCol_SeparatorActive] = colors.SeparatorActive;

		c[ImGuiCol_ResizeGrip] = colors.ResizeGrip;
		c[ImGuiCol_ResizeGripHovered] = colors.ResizeGripHovered;
		c[ImGuiCol_ResizeGripActive] = colors.ResizeGripActive;

		c[ImGuiCol_Tab] = colors.Tab;
		c[ImGuiCol_TabHovered] = colors.TabHovered;
		c[ImGuiCol_TabActive] = colors.TabActive;
		c[ImGuiCol_TabUnfocused] = colors.TabUnfocused;
		c[ImGuiCol_TabUnfocusedActive] = colors.TabUnfocusedActive;

		c[ImGuiCol_PlotLines] = colors.PlotLines;
		c[ImGuiCol_PlotLinesHovered] = colors.PlotLinesHovered;
		c[ImGuiCol_PlotHistogram] = colors.PlotHistogram;
		c[ImGuiCol_PlotHistogramHovered] = colors.PlotHistogramHovered;

		c[ImGuiCol_TableHeaderBg] = colors.TableHeaderBackground;
		c[ImGuiCol_TableBorderStrong] = colors.TableBorderStrong;

		c[ImGuiCol_TextSelectedBg] = colors.TextSelectedBackground;

		c[ImGuiCol_DragDropTarget] = colors.DragDropTarget;

		c[ImGuiCol_NavHighlight] = colors.NavHighlight;
		c[ImGuiCol_NavWindowingHighlight] = colors.NavWindowingHighlight;
		c[ImGuiCol_NavWindowingDimBg] = colors.NavWindowingDimBackground;

		c[ImGuiCol_ModalWindowDimBg] = colors.ModalWindowDimBackground;

		ImGuizmo::Style* imguizmoStyle = &ImGuizmo::GetStyle();
		ImVec4* gc = imguizmoStyle->Colors;

		gc[ImGuizmo::DIRECTION_X] = ImVec4(0.858f, 0.243f, 0.113f, 0.929f);
		gc[ImGuizmo::DIRECTION_Y] = ImVec4(0.375f, 0.825f, 0.372f, 0.929f);
		gc[ImGuizmo::DIRECTION_Z] = ImVec4(0.227f, 0.478f, 0.972f, 0.929f);
		gc[ImGuizmo::PLANE_X] = gc[ImGuizmo::DIRECTION_X];
		gc[ImGuizmo::PLANE_Y] = gc[ImGuizmo::DIRECTION_Y];
		gc[ImGuizmo::PLANE_Z] = gc[ImGuizmo::DIRECTION_Z];	

		imguizmoStyle->TranslationLineThickness = 4.f;		// Thickness of lines for translation gizmo
		imguizmoStyle->TranslationLineArrowSize = 4.f;		// Size of arrow at the end of lines for translation gizmo
		imguizmoStyle->RotationLineThickness = 3.f;			// Thickness of lines for rotation gizmo
		imguizmoStyle->RotationOuterLineThickness = 3.f;	// Thickness of line surrounding the rotation gizmo
		imguizmoStyle->ScaleLineThickness = 3.f;			// Thickness of lines for scale gizmo
		imguizmoStyle->ScaleLineCircleSize = 3.f;			// Size of circle at the end of lines for scale gizmo
		imguizmoStyle->HatchedAxisLineThickness = 4.f;		// Thickness of hatched axis lines
		imguizmoStyle->CenterCircleSize = 4.f;				// Size of circle at the center of the translate/scale gizmo
	}

	void Appearance::ApplyFonts(const FontSpecification& spec)
	{
		const ImGuiIO& io = ImGui::GetIO();

		s_Fonts.SmallSize = spec.SmallSize;
		s_Fonts.DefaultSize = spec.DefaultSize;
		s_Fonts.BigSize = spec.BigSize;

		s_Fonts.DefaultFont = io.Fonts->AddFontFromFileTTF(spec.FontPath.c_str(), spec.DefaultSize);
		AppendIconFont(spec.DefaultSize);
		
		s_Fonts.SmallFont = io.Fonts->AddFontFromFileTTF(spec.FontPath.c_str(), spec.SmallSize);
		s_Fonts.BigFont = io.Fonts->AddFontFromFileTTF(spec.FontPath.c_str(), spec.BigSize);

		s_Fonts.DefaultBoldFont = io.Fonts->AddFontFromFileTTF(spec.BoldFontPath.c_str(), spec.DefaultSize);
		s_Fonts.SmallBoldFont = io.Fonts->AddFontFromFileTTF(spec.BoldFontPath.c_str(), spec.SmallSize);
		s_Fonts.BigBoldFont = io.Fonts->AddFontFromFileTTF(spec.BoldFontPath.c_str(), spec.BigSize);
	}

	void Appearance::AppendIconFont(f32 size)
	{
		ImGuiIO& io = ImGui::GetIO();

		static constexpr ImWchar icons_ranges[] = { 0x30, 0xFE7D, 0 };

		ImFontConfig iconsConfig;

		iconsConfig.MergeMode = true;
		iconsConfig.PixelSnapH = true;
		iconsConfig.GlyphOffset.y = 1.0f;
		iconsConfig.OversampleH = iconsConfig.OversampleV = 1;
		iconsConfig.GlyphMinAdvanceX = 4.0f;
		iconsConfig.SizePixels = 12.0f;

		io.Fonts->AddFontFromMemoryCompressedTTF(MaterialDesign_compressed_data, MaterialDesign_compressed_size, size, &iconsConfig, icons_ranges);
	}
}
