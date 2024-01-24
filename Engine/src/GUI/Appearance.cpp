#include "Appearance.hpp"

#include <imgui.h>

#include <icons/MaterialDesign.inl>

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

		s.WindowPadding = ImVec2(style.WindowPadding.x, style.WindowPadding.y);
		s.WindowRounding = style.WindowRounding;
		s.WindowBorderSize = style.WindowBorderSize;
		s.WindowMinSize = ImVec2(style.WindowMinSize.x, style.WindowMinSize.y);
		s.WindowTitleAlign = ImVec2(style.WindowTitleAlign.x, style.WindowTitleAlign.y);
		s.WindowMenuButtonPosition = style.WindowMenuButtonPosition;

		s.ChildRounding = style.ChildRounding;
		s.ChildBorderSize = style.ChildBorderSize;

		s.PopupRounding = style.PopupRounding;
		s.PopupBorderSize = style.PopupBorderSize;

		s.FramePadding = ImVec2(style.FramePadding.x, style.FramePadding.y);
		s.FrameRounding = style.FrameRounding;
		s.FrameBorderSize = style.FrameBorderSize;

		s.ItemSpacing = ImVec2(style.ItemSpacing.x, style.ItemSpacing.y);
		s.ItemInnerSpacing = ImVec2(style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);
		s.CellPadding = ImVec2(style.CellPadding.x, style.CellPadding.y);

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
		s.ButtonTextAlign = ImVec2(style.ButtonTextAlign.x, style.ButtonTextAlign.y);
		s.SelectableTextAlign = ImVec2(style.SelectableTextAlign.x, style.SelectableTextAlign.y);
	}

	void Appearance::ApplyColors(const Colors& colors)
	{
		ImGuiStyle& s = ImGui::GetStyle();
		ImVec4* c = s.Colors;

		s_Colors = colors;

		c[ImGuiCol_Text] = ImVec4(colors.Text.x, colors.Text.y, colors.Text.z, colors.Text.w);
		c[ImGuiCol_TextDisabled] = ImVec4(colors.TextDisabled.x, colors.TextDisabled.y, colors.TextDisabled.z, colors.TextDisabled.w);

		c[ImGuiCol_WindowBg] = ImVec4(colors.WindowBackground.x, colors.WindowBackground.y, colors.WindowBackground.z, colors.WindowBackground.w);
		c[ImGuiCol_ChildBg] = ImVec4(colors.ChildBackground.x, colors.ChildBackground.y, colors.ChildBackground.z, colors.ChildBackground.w);
		c[ImGuiCol_PopupBg] = ImVec4(colors.PopupBackground.x, colors.PopupBackground.y, colors.PopupBackground.z, colors.PopupBackground.w);

		c[ImGuiCol_Border] = ImVec4(colors.Border.x, colors.Border.y, colors.Border.z, colors.Border.w);
		c[ImGuiCol_BorderShadow] = ImVec4(colors.BorderShadow.x, colors.BorderShadow.y, colors.BorderShadow.z, colors.BorderShadow.w);

		c[ImGuiCol_FrameBg] = ImVec4(colors.FrameBackground.x, colors.FrameBackground.y, colors.FrameBackground.z, colors.FrameBackground.w);
		c[ImGuiCol_FrameBgHovered] = ImVec4(colors.FrameBackgroundHovered.x, colors.FrameBackgroundHovered.y, colors.FrameBackgroundHovered.z, colors.FrameBackgroundHovered.w);
		c[ImGuiCol_FrameBgActive] = ImVec4(colors.FrameBackgroundActive.x, colors.FrameBackgroundActive.y, colors.FrameBackgroundActive.z, colors.FrameBackgroundActive.w);

		c[ImGuiCol_TitleBg] = ImVec4(colors.TitleBackground.x, colors.TitleBackground.y, colors.TitleBackground.z, colors.TitleBackground.w);
		c[ImGuiCol_TitleBgActive] = ImVec4(colors.TitleBackgroundActive.x, colors.TitleBackgroundActive.y, colors.TitleBackgroundActive.z, colors.TitleBackgroundActive.w);
		c[ImGuiCol_TitleBgCollapsed] = ImVec4(colors.TitleBackgroundCollapsed.x, colors.TitleBackgroundCollapsed.y, colors.TitleBackgroundCollapsed.z, colors.TitleBackgroundCollapsed.w);

		c[ImGuiCol_MenuBarBg] = ImVec4(colors.MenuBarBackground.x, colors.MenuBarBackground.y, colors.MenuBarBackground.z, colors.MenuBarBackground.w);

		c[ImGuiCol_ScrollbarBg] = ImVec4(colors.ScrollbarBackground.x, colors.ScrollbarBackground.y, colors.ScrollbarBackground.z, colors.ScrollbarBackground.w);
		c[ImGuiCol_ScrollbarGrab] = ImVec4(colors.ScrollbarGrab.x, colors.ScrollbarGrab.y, colors.ScrollbarGrab.z, colors.ScrollbarGrab.w);
		c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(colors.ScrollbarGrabHovered.x, colors.ScrollbarGrabHovered.y, colors.ScrollbarGrabHovered.z, colors.ScrollbarGrabHovered.w);
		c[ImGuiCol_ScrollbarGrabActive] = ImVec4(colors.ScrollbarGrabActive.x, colors.ScrollbarGrabActive.y, colors.ScrollbarGrabActive.z, colors.ScrollbarGrabActive.w);

		c[ImGuiCol_CheckMark] = ImVec4(colors.CheckMark.x, colors.CheckMark.y, colors.CheckMark.z, colors.CheckMark.w);

		c[ImGuiCol_SliderGrab] = ImVec4(colors.SliderGrab.x, colors.SliderGrab.y, colors.SliderGrab.z, colors.SliderGrab.w);
		c[ImGuiCol_SliderGrabActive] = ImVec4(colors.SliderGrabActive.x, colors.SliderGrabActive.y, colors.SliderGrabActive.z, colors.SliderGrabActive.w);

		c[ImGuiCol_Button] = ImVec4(colors.Button.x, colors.Button.y, colors.Button.z, colors.Button.w);
		c[ImGuiCol_ButtonHovered] = ImVec4(colors.ButtonHovered.x, colors.ButtonHovered.y, colors.ButtonHovered.z, colors.ButtonHovered.w);
		c[ImGuiCol_ButtonActive] = ImVec4(colors.ButtonActive.x, colors.ButtonActive.y, colors.ButtonActive.z, colors.ButtonActive.w);

		c[ImGuiCol_Header] = ImVec4(colors.Header.x, colors.Header.y, colors.Header.z, colors.Header.w);
		c[ImGuiCol_HeaderHovered] = ImVec4(colors.HeaderHovered.x, colors.HeaderHovered.y, colors.HeaderHovered.z, colors.HeaderHovered.w);
		c[ImGuiCol_HeaderActive] = ImVec4(colors.HeaderActive.x, colors.HeaderActive.y, colors.HeaderActive.z, colors.HeaderActive.w);

		c[ImGuiCol_Separator] = ImVec4(colors.Separator.x, colors.Separator.y, colors.Separator.z, colors.Separator.w);
		c[ImGuiCol_SeparatorHovered] = ImVec4(colors.SeparatorHovered.x, colors.SeparatorHovered.y, colors.SeparatorHovered.z, colors.SeparatorHovered.w);
		c[ImGuiCol_SeparatorActive] = ImVec4(colors.SeparatorActive.x, colors.SeparatorActive.y, colors.SeparatorActive.z, colors.SeparatorActive.w);

		c[ImGuiCol_ResizeGrip] = ImVec4(colors.ResizeGrip.x, colors.ResizeGrip.y, colors.ResizeGrip.z, colors.ResizeGrip.w);
		c[ImGuiCol_ResizeGripHovered] = ImVec4(colors.ResizeGripHovered.x, colors.ResizeGripHovered.y, colors.ResizeGripHovered.z, colors.ResizeGripHovered.w);
		c[ImGuiCol_ResizeGripActive] = ImVec4(colors.ResizeGripActive.x, colors.ResizeGripActive.y, colors.ResizeGripActive.z, colors.ResizeGripActive.w);

		c[ImGuiCol_Tab] = ImVec4(colors.Tab.x, colors.Tab.y, colors.Tab.z, colors.Tab.w);
		c[ImGuiCol_TabHovered] = ImVec4(colors.TabHovered.x, colors.TabHovered.y, colors.TabHovered.z, colors.TabHovered.w);
		c[ImGuiCol_TabActive] = ImVec4(colors.TabActive.x, colors.TabActive.y, colors.TabActive.z, colors.TabActive.w);
		c[ImGuiCol_TabUnfocused] = ImVec4(colors.TabUnfocused.x, colors.TabUnfocused.y, colors.TabUnfocused.z, colors.TabUnfocused.w);
		c[ImGuiCol_TabUnfocusedActive] = ImVec4(colors.TabUnfocusedActive.x, colors.TabUnfocusedActive.y, colors.TabUnfocusedActive.z, colors.TabUnfocusedActive.w);

		c[ImGuiCol_PlotLines] = ImVec4(colors.PlotLines.x, colors.PlotLines.y, colors.PlotLines.z, colors.PlotLines.w);
		c[ImGuiCol_PlotLinesHovered] = ImVec4(colors.PlotLinesHovered.x, colors.PlotLinesHovered.y, colors.PlotLinesHovered.z, colors.PlotLinesHovered.w);
		c[ImGuiCol_PlotHistogram] = ImVec4(colors.PlotHistogram.x, colors.PlotHistogram.y, colors.PlotHistogram.z, colors.PlotHistogram.w);
		c[ImGuiCol_PlotHistogramHovered] = ImVec4(colors.PlotHistogramHovered.x, colors.PlotHistogramHovered.y, colors.PlotHistogramHovered.z, colors.PlotHistogramHovered.w);

		c[ImGuiCol_TableHeaderBg] = ImVec4(colors.TableHeaderBackground.x, colors.TableHeaderBackground.y, colors.TableHeaderBackground.z, colors.TableHeaderBackground.w);
		c[ImGuiCol_TableBorderStrong] = ImVec4(colors.TableBorderStrong.x, colors.TableBorderStrong.y, colors.TableBorderStrong.z, colors.TableBorderStrong.w);
		c[ImGuiCol_TableBorderLight] = ImVec4(colors.TableBorderLight.x, colors.TableBorderLight.y, colors.TableBorderLight.z, colors.TableBorderLight.w);
		c[ImGuiCol_TableRowBg] = ImVec4(colors.TableRowBackground.x, colors.TableRowBackground.y, colors.TableRowBackground.z, colors.TableRowBackground.w);
		c[ImGuiCol_TableRowBgAlt] = ImVec4(colors.TableRowBackgroundAlt.x, colors.TableRowBackgroundAlt.y, colors.TableRowBackgroundAlt.z, colors.TableRowBackgroundAlt.w);

		c[ImGuiCol_TextSelectedBg] = ImVec4(colors.TextSelectedBackground.x, colors.TextSelectedBackground.y, colors.TextSelectedBackground.z, colors.TextSelectedBackground.w);

		c[ImGuiCol_DragDropTarget] = ImVec4(colors.DragDropTarget.x, colors.DragDropTarget.y, colors.DragDropTarget.z, colors.DragDropTarget.w);

		c[ImGuiCol_NavHighlight] = ImVec4(colors.NavHighlight.x, colors.NavHighlight.y, colors.NavHighlight.z, colors.NavHighlight.w);
		c[ImGuiCol_NavWindowingHighlight] = ImVec4(colors.NavWindowingHighlight.x, colors.NavWindowingHighlight.y, colors.NavWindowingHighlight.z, colors.NavWindowingHighlight.w);
		c[ImGuiCol_NavWindowingDimBg] = ImVec4(colors.NavWindowingDimBackground.x, colors.NavWindowingDimBackground.y, colors.NavWindowingDimBackground.z, colors.NavWindowingDimBackground.w);

		c[ImGuiCol_ModalWindowDimBg] = ImVec4(colors.ModalWindowDimBackground.x, colors.ModalWindowDimBackground.y, colors.ModalWindowDimBackground.z, colors.ModalWindowDimBackground.w);
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
