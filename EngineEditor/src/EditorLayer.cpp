#include "EditorLayer.hpp"

#include <filesystem>

#include "Panels/StatisticsPanel.hpp"
#include "Panels/ConsolePanel.hpp"
#include "Panels/AssetPanel.hpp"
#include "Panels/SceneHierarchyPanel.hpp"
#include "Panels/PropertiesPanel.hpp"
#include "Core/Scene/SceneSerializer.hpp"
#include "Core/Utils/Filesystem.hpp"

namespace SW {

	void EditorLayer::OnAttach()
	{
		m_Shader = std::make_shared<Shader>("assets/shaders/Initial.vert.glsl", "assets/shaders/Initial.frag.glsl");

		m_IconTexture = new Texture2D("assets/icons/SW_Icon.png", false);
		m_CloseIconTexture = new Texture2D("assets/icons/editor/windows/Close.png", false);
		m_MaximizeIconTexture = new Texture2D("assets/icons/editor/windows/Maximize.png", false);
		m_MinimizeIconTexture = new Texture2D("assets/icons/editor/windows/Minimize.png", false);
		m_RestoreIconTexture = new Texture2D("assets/icons/editor/windows/Restore.png", false);

		const GUI::FontSpecification fontSpec("assets/fonts/Roboto/Roboto-Regular.ttf", "assets/fonts/Roboto/Roboto-Bold.ttf");

		GUI::Appearance::ApplyStyle(GUI::Style());
		GUI::Appearance::ApplyColors(GUI::Colors());
		GUI::Appearance::ApplyFonts(fontSpec);

		EventSystem::Register(EVENT_CODE_KEY_PRESSED, nullptr, [this](Event event, void* sender, void* listener) -> bool {
			KeyCode code = (KeyCode)event.Payload.u16[0];

			return OnKeyPressed(code);
		});

		m_Panels.emplace_back(new StatisticsPanel("Statistics"));
		m_Panels.emplace_back(new ConsolePanel());
		m_Panels.emplace_back(new AssetPanel());

		m_Viewport = new SceneViewportPanel();
		
		m_Panels.emplace_back(m_Viewport);
		m_Panels.emplace_back(new PropertiesPanel(m_Viewport->GetCurrentScene()));
		m_Panels.emplace_back(new SceneHierarchyPanel(m_Viewport->GetCurrentScene()));

		Application::Get()->GetWindow()->SetVSync(true);

		Renderer2D::Init(m_Shader);
	}

	void EditorLayer::OnDetach()
	{
		delete m_IconTexture;
		delete m_CloseIconTexture;
		delete m_MaximizeIconTexture;
		delete m_MinimizeIconTexture;
		delete m_RestoreIconTexture;

		for (Panel* panel : m_Panels) {
			delete panel;
		}

		Renderer2D::Shutdown();
	}

	void EditorLayer::OnUpdate(Timestep dt)
	{
		for (Panel* panel : m_Panels) {
			if (panel->IsShowing())
				panel->OnUpdate(dt);
		}
	}

	f32 EditorLayer::DrawTitleBar()
	{
		constexpr f32 titlebarHeight = 57.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		ImGui::SetCursorPos(windowPadding);

		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = {
			ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
			ImGui::GetCursorScreenPos().y + titlebarHeight
		};

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->AddRectFilled(titlebarMin, titlebarMax, Color::TitleBar);
		drawList->AddRectFilledMultiColor(titlebarMin, ImVec2(titlebarMin.x + 600.0f, titlebarMax.y), Color::LightBlack, Color::TitleBar, Color::TitleBar, Color::LightBlack);

		{
			const f32 logoWidth = (f32)m_IconTexture->GetWidth() / 3.f;
			const f32 logoHeight = (f32)m_IconTexture->GetHeight() / 3.f;
			const ImVec2 logoOffset(20.0f + windowPadding.x, windowPadding.y - 1.2f);
			const ImVec2 logoRectStart = { ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y };
			const ImVec2 logoRectMax = { logoRectStart.x + logoWidth, logoRectStart.y + logoHeight };

			drawList->AddImage(GUI::GetTextureID(m_IconTexture), logoRectStart, logoRectMax);
		}

		const f32 w = ImGui::GetContentRegionAvail().x;
		constexpr f32 buttonsAreaWidth = 350; // just so it doesn't interfere with buttons on the right

		ImGui::SetCursorPosX(230); // just so it doesn't interfere with the menubar on the left
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));

		if (ImGui::IsItemHovered()) {
			Application::Get()->GetWindow()->RegisterOverTitlebar(true);
		}

		{
			const float logoOffset = 16.0f * 2.0f + 41.0f + windowPadding.x;
			ImGui::SetCursorPos(ImVec2(logoOffset, 4.0f));

			DrawMenuBar();
		}

		// Minimize Button
		{
			const float rightOffset = ImGui::GetWindowWidth() - 120.0f;
			ImGui::SameLine();
			ImGui::SetCursorPosX(rightOffset - windowPadding.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f + windowPadding.y);
			ImGui::SetItemAllowOverlap();

			if (GUI::ImageButton(*m_MinimizeIconTexture, { 40.f, 40.f })) {
				Application::Get()->GetWindow()->Minimize();
			}
		}

		// Maximize / shrink button
		{
			const float rightOffset = ImGui::GetWindowWidth() - 80.0f;
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(rightOffset - windowPadding.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f + windowPadding.y);
			ImGui::SetItemAllowOverlap();

			if (m_WindowMaximized) {
				if (GUI::ImageButton(*m_RestoreIconTexture, { 40.f, 40.f })) {
					Application::Get()->GetWindow()->Restore();
				}
			} else {
				if (GUI::ImageButton(*m_MaximizeIconTexture, { 40.f, 40.f })) {
					Application::Get()->GetWindow()->Maximize();
				}
			}
		}

		// Close button
		{
			const float rightOffset = ImGui::GetWindowWidth() - 40.0f;
			ImGui::SameLine();
			ImGui::SetCursorPosX(rightOffset - windowPadding.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f + windowPadding.y);
			ImGui::SetItemAllowOverlap();

			if (GUI::ImageButton(*m_CloseIconTexture, { 40.f, 40.f })) {
				Application::Get()->Close();
			}
		}

		return titlebarHeight;
	}

	void EditorLayer::DrawMenuBar()
	{
		ImGui::SetCursorPosX(120); // just so it doesn't interfere with the SW logo

		const ImRect menuBarRect = { 
			ImGui::GetCursorPos(), { ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing() }
		};

		ImGui::BeginGroup();

		if (GUI::BeginMenuBar(menuBarRect)) {
			GUI::ScopedColor HeaderColor(ImGuiCol_Header, Color::DarkGray);
			GUI::ScopedColor HeaderHoveredColor(ImGuiCol_HeaderHovered, Color::DarkGray);

			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Create Project...", "Ctrl+N")) {
					
				}

				if (ImGui::MenuItem("Open Project...", "Ctrl+O")) {
					std::filesystem::path filepath = FileSystem::OpenFileDialog({ { "SW Engine Scene file", "sw" } });

					if (!filepath.empty()) {
						SceneSerializer::Deserialize(m_Viewport->GetCurrentScene(), filepath.string());
					}
				}

				if (ImGui::MenuItem("Save Project", "Ctrl+S")) {

				}

				if (ImGui::MenuItem("Save Project As...", "Ctrl+Shift+S")) {
					std::filesystem::path filepath = FileSystem::SaveFileDialog({ { "SW Engine Scene file", "sw" } });

					if (!filepath.empty()) {
						SceneSerializer::Serialize(m_Viewport->GetCurrentScene(), filepath.string());
					}
				}

				if (ImGui::MenuItem("Close Editor", "Esc")) {
					Application::Get()->Close();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {
				if (ImGui::MenuItem("Show viewport")) {

				}

				ImGui::EndMenu();
			}
		}
		GUI::EndMenuBar();


		ImGui::EndGroup();
	}

	void EditorLayer::OnRender()
	{
		Application::Get()->GetWindow()->RegisterOverTitlebar(false);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		io.ConfigWindowsResizeFromEdges = io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		m_WindowMaximized = Application::Get()->GetWindow()->IsCurrentlyMaximized();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, m_WindowMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin("DockSpace Demo", nullptr, window_flags);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::PopStyleVar(2);

		f32 titlebarHeight = DrawTitleBar();

		ImGui::SetCursorPosY(titlebarHeight + ImGui::GetCurrentWindow()->WindowPadding.y);

		// Dockspace
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));
		style.WindowMinSize.x = minWinSizeX;

		ImGui::End();

		for (Panel* panel : m_Panels) {
			if (panel->IsShowing())
				panel->OnRender();
		}

		//ImGui::ShowDemoWindow();
	}

	bool EditorLayer::OnKeyPressed(KeyCode code)
	{
		const bool shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);
		const bool ctrl = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
		const bool alt = Input::IsKeyPressed(KeyCode::LeftAlt) || Input::IsKeyPressed(KeyCode::RightAlt);

		switch (code) {
			case KeyCode::Escape:
				Application::Get()->Close();
			default:
				break;
		}

		return true;
	}

}
