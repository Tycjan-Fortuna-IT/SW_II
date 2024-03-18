#include "SpritesheetEditor.hpp"

#include <imgui_canvas.h>

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

namespace SW {

	SpritesheetEditor::SpritesheetEditor(const char* name, const char* icon)
		: AssetEditorPanel(name, icon) {}

	void SpritesheetEditor::OnUpdate(Timestep ts)
	{
		
	}

	void SpritesheetEditor::OnWindowStylePush()
	{

	}

	void SpritesheetEditor::OnWindowStylePop()
	{

	}

	static Texture2D* spritesheet = nullptr;

	void SpritesheetEditor::OnOpen()
	{
		spritesheet = AssetManager::GetTexture2D("assets\\spritesheets\\spritesheet_test.png");
	}

	void SpritesheetEditor::OnClose()
	{

	}

	void SpritesheetEditor::Render()
	{
		auto& io = ImGui::GetIO();

		auto availableRegion = ImGui::GetContentRegionAvail();

		static f32 s_SplitterSize = 6.0f;
		static f32 s_SplitterArea = 0.0f;
		static f32 s_LeftPaneSize = 0.0f;
		static f32 s_RightPaneSize = 0.0f;

		if (s_SplitterArea != availableRegion.x) {
			if (s_SplitterArea == 0.0f) {
				s_SplitterArea = availableRegion.x;
				s_LeftPaneSize = ImFloor(availableRegion.x * 0.25f);
				s_RightPaneSize = availableRegion.x - s_LeftPaneSize - s_SplitterSize;
			}
			else {
				auto ratio = availableRegion.x / s_SplitterArea;
				s_SplitterArea = availableRegion.x;
				s_LeftPaneSize = s_LeftPaneSize * ratio;
				s_RightPaneSize = availableRegion.x - s_LeftPaneSize - s_SplitterSize;
			}
		}

		static ImGuiEx::Canvas canvas;
		static ImVec2 drawStartPoint;
		static bool isDragging = false;
		static ImRect panelRect;

		Splitter(true, s_SplitterSize, &s_LeftPaneSize, &s_RightPaneSize, 100.0f, 100.0f);

		auto canvasRect = canvas.Rect();
		auto viewRect = canvas.ViewRect();
		auto viewOrigin = canvas.ViewOrigin();
		auto viewScale = canvas.ViewScale();

		ImGui::BeginChild("##top", ImVec2(s_LeftPaneSize, -1), false, ImGuiWindowFlags_NoScrollWithMouse);

		GUI::BeginProperties("##spritesheet_editor_properties");
		if (GUI::DrawFloatingPointProperty(viewScale, "View Zoom", nullptr, 0.5f, 15.0f)) {
			canvas.SetView(viewOrigin, viewScale);
		}
		static f32 scale = 100.f;
		GUI::DrawFloatingPointProperty(scale, "Scale", nullptr, 16.f, 1000.f);
		static glm::vec2 sOffset = glm::vec2(0.f);
		GUI::DrawVector2ControlProperty(sOffset, "Offset from center");
		GUI::EndProperties();

		ImGui::EndChild();
		ImGui::SameLine(0.0f, s_SplitterSize);

		if (canvas.Begin("##mycanvas", ImVec2(s_RightPaneSize, 0.0f))) {
			if ((isDragging || ImGui::IsItemHovered()) && ImGui::IsMouseDragging(1, 0.0f)) {
				if (!isDragging) {
					isDragging = true;
					drawStartPoint = viewOrigin;
				}

				canvas.SetView(drawStartPoint + ImGui::GetMouseDragDelta(1, 0.0f) * viewScale, viewScale);
			} else if (isDragging)
				isDragging = false;

			viewRect = canvas.ViewRect();

			ImVec2 offset = canvas.ViewOrigin() * (1.0f / canvas.ViewScale());
			ImVec2 viewPos = canvas.ViewRect().Min;
			ImVec2 viewSize = canvas.ViewRect().GetSize();
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			drawList->AddRectFilled(viewPos, viewPos + viewSize, GUI::Theme::Background);

			for (f32 x = fmodf(offset.x, scale); x < viewSize.x; x += scale)
				drawList->AddLine(ImVec2(x + sOffset.x, 0.0f + sOffset.y) + viewPos, ImVec2(x + sOffset.x, viewSize.y + sOffset.y) + viewPos, GUI::Theme::Selection);
			for (f32 y = fmodf(offset.y, scale); y < viewSize.y; y += scale)
				drawList->AddLine(ImVec2(0.0f + sOffset.x, y + sOffset.y) + viewPos, ImVec2(viewSize.x + sOffset.x, y + sOffset.y) + viewPos, GUI::Theme::Selection);

			if (viewRect.Max.x > 0.0f)
				DrawScale(ImVec2(0.0f, 0.0f), ImVec2(viewRect.Max.x, 0.0f), scale, 10.0f, 0.6f);
			if (viewRect.Min.x < 0.0f)
				DrawScale(ImVec2(0.0f, 0.0f), ImVec2(viewRect.Min.x, 0.0f), scale, 10.0f, 0.6f, -1.0f);
			if (viewRect.Max.y > 0.0f)
				DrawScale(ImVec2(0.0f, 0.0f), ImVec2(0.0f, viewRect.Max.y), scale, 10.0f, 0.6f);
			if (viewRect.Min.y < 0.0f)
				DrawScale(ImVec2(0.0f, 0.0f), ImVec2(0.0f, viewRect.Min.y), scale, 10.0f, 0.6f, -1.0f);

			ImGui::Image(
				GUI::GetTextureID(spritesheet->GetHandle()), { (f32)spritesheet->GetWidth(), (f32)spritesheet->GetHeight() }, { 0, 1 }, { 1, 0 }
			);

			panelRect.Min = ImGui::GetItemRectMin();
			panelRect.Max = ImGui::GetItemRectMax();

			canvas.End();
		}
	}

	void SpritesheetEditor::SetAsset(Asset* asset)
	{

	}

}
