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

		// Headers
		c[ImGuiCol_Header] = s_Colors.Header;
		c[ImGuiCol_HeaderHovered] = s_Colors.HeaderHovered;
		c[ImGuiCol_HeaderActive] = s_Colors.HeaderActive;

		// Buttons
		c[ImGuiCol_Button] = s_Colors.Button;
		c[ImGuiCol_ButtonHovered] = s_Colors.ButtonHovered;
		c[ImGuiCol_ButtonActive] = s_Colors.ButtonActive;

		// Frame BG
		c[ImGuiCol_FrameBg] = s_Colors.FrameBackground;
		c[ImGuiCol_FrameBgHovered] = s_Colors.FrameBackgroundHovered;
		c[ImGuiCol_FrameBgActive] = s_Colors.FrameBackgroundActive;

		// Tabs
		c[ImGuiCol_Tab] = s_Colors.Tab;
		c[ImGuiCol_TabHovered] = s_Colors.TabHovered;
		c[ImGuiCol_TabActive] = s_Colors.TabActive;
		c[ImGuiCol_TabUnfocused] = s_Colors.TabUnfocused;
		c[ImGuiCol_TabUnfocusedActive] = s_Colors.TabUnfocusedActive;

		// Title
		c[ImGuiCol_TitleBg] = s_Colors.TitleBackground;
		c[ImGuiCol_TitleBgActive] = s_Colors.TitleBackgroundActive;
		c[ImGuiCol_TitleBgCollapsed] = s_Colors.TitleBackgroundCollapsed;

		// Resize Grip
		c[ImGuiCol_ResizeGrip] = s_Colors.ResizeGrip;
		c[ImGuiCol_ResizeGripHovered] = s_Colors.ResizeGripHovered;
		c[ImGuiCol_ResizeGripActive] = s_Colors.ResizeGripActive;

		// Scrollbar
		c[ImGuiCol_ScrollbarBg] = s_Colors.ScrollbarBackground;
		c[ImGuiCol_ScrollbarGrab] = s_Colors.ScrollbarGrab;
		c[ImGuiCol_ScrollbarGrabHovered] = s_Colors.ScrollbarGrabHovered;
		c[ImGuiCol_ScrollbarGrabActive] = s_Colors.ScrollbarGrabActive;

		// Check Mark
		c[ImGuiCol_CheckMark] = s_Colors.CheckMark;

		// Slider
		c[ImGuiCol_SliderGrab] = s_Colors.SliderGrab;
		c[ImGuiCol_SliderGrabActive] = s_Colors.SliderGrabActive;

		// Text
		c[ImGuiCol_Text] = s_Colors.Text;
		c[ImGuiCol_TextDisabled] = s_Colors.TextDisabled;

		// Checkbox
		c[ImGuiCol_CheckMark] = s_Colors.CheckMark;

		// Separator
		c[ImGuiCol_Separator] = s_Colors.Separator;
		c[ImGuiCol_SeparatorActive] = s_Colors.SeparatorActive;
		c[ImGuiCol_SeparatorHovered] = s_Colors.SeparatorHovered;

		// Window Background
		c[ImGuiCol_WindowBg] = s_Colors.WindowBackground;
		c[ImGuiCol_ChildBg] = s_Colors.ChildBackground;
		c[ImGuiCol_PopupBg] = s_Colors.PopupBackground;
		c[ImGuiCol_Border] = s_Colors.Border;

		// Tables
		c[ImGuiCol_TableHeaderBg] = s_Colors.TableHeaderBackground;
		c[ImGuiCol_TableBorderLight] = s_Colors.TableBorderLight;

		// Menubar
		c[ImGuiCol_MenuBarBg] = s_Colors.MenuBarBackground;

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
