#include "TestLayer.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "Core/ECS/Components.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Math/Math.hpp"
#include "GUI/Appearance.hpp"
#include "GUI/Colors.hpp"
#include "GUI/GUI.hpp"
#include "Core/Renderer/Renderer2D.hpp"

namespace SW {

	void TestLayer::OnAttach() {
		APP_TRACE("TestLayer::OnAttach()");

		m_Shader = std::make_shared<Shader>("assets/shaders/Initial.vert.glsl", "assets/shaders/Initial.frag.glsl");

		boxTexture = new Texture2D("assets/textures/container_512x512.jpg");
		faceTexture = new Texture2D("assets/textures/awesomeface_512x512.png");
		m_IconTexture = new Texture2D("assets/icons/SW_Icon.png", false);
		m_CloseIconTexture = new Texture2D("assets/icons/editor/windows/Close.png", false);
		m_MaximizeIconTexture = new Texture2D("assets/icons/editor/windows/Maximize.png", false);
		m_MinimizeIconTexture = new Texture2D("assets/icons/editor/windows/Minimize.png", false);
		m_RestoreIconTexture = new Texture2D("assets/icons/editor/windows/Restore.png", false);

		const FramebufferSpecification spec = { 1280, 720 };
		framebuffer = new Framebuffer(spec);

		m_SceneCamera = new SceneCamera((f32)(spec.Width / spec.Height));

		const GUI::FontSpecification fontSpec("assets/fonts/Roboto/Roboto-Regular.ttf", "assets/fonts/Roboto/Roboto-Bold.ttf");

		GUI::Appearance::ApplyStyle(GUI::Style());
		GUI::Appearance::ApplyColors(GUI::Colors());
		GUI::Appearance::ApplyFonts(fontSpec);

		EventSystem::Register(EVENT_CODE_MOUSE_WHEEL, nullptr, [this](Event event, void* sender, void* listener) -> bool {
			const f32 xOffset = event.Payload.f32[0];
			const f32 yOffset = event.Payload.f32[1];

			if (!m_IsViewportFocused) return false;

			m_SceneCamera->OnMouseScrolled(xOffset, yOffset);

			return false;
		});

		EventSystem::Register(EVENT_CODE_WINDOW_RESIZED, nullptr, [this](Event event, void* sender, void* listener) -> bool {
			const i32 width = event.Payload.i32[0];
			const i32 height = event.Payload.i32[1];

			if (!m_IsViewportFocused) return false;

			m_SceneCamera->OnViewportResize((f32)width, (f32)height);

			return false;
		});

		//m_CameraEntity = m_Scene.CreateEntity("Camera Entity Test - 1");
		//m_CameraEntity.AddComponent<CameraComponent>();

		m_Square1 = m_Scene.CreateEntity("Square Entity Test - 1");
		m_Square1.AddComponent<SpriteComponent>(Vector4<f32>(0.5f, 0.5f, 0.5f, 1.0f));

		m_Square2 = m_Scene.CreateEntity("Square Entity Test - 1");
		m_Square2.AddComponent<SpriteComponent>(Vector4<f32>(1.f, 0.f, 0.f, 1.0f));

		m_Square2.GetComponent<TransformComponent>().Position = { 0.25f, 0.25f, 0.0f };

		Application::Get()->GetWindow()->Maximize();
		Application::Get()->GetWindow()->SetVSync(true);

		Renderer2D::Init(m_Shader);
	}

	void TestLayer::OnDetach() {
		delete boxTexture;
		delete faceTexture;
		delete m_IconTexture;
		delete m_CloseIconTexture;
		delete m_MaximizeIconTexture;
		delete m_MinimizeIconTexture;
		delete m_RestoreIconTexture;
		delete m_SceneCamera;
		delete framebuffer;

		Renderer2D::Shutdown();
	}

	void TestLayer::OnUpdate(Timestep dt) {
		FramebufferSpecification spec = framebuffer->GetSpecification();

		if (m_IsViewportFocused)
			m_SceneCamera->OnUpdate(dt);

		if (
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // if it's a valid size viewport
			(spec.Width != (u32)m_ViewportSize.x || spec.Height != (u32)m_ViewportSize.y) // if it changed
		) {
			framebuffer->Resize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
			m_SceneCamera->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene.OnViewportResize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();

		framebuffer->Bind();

		framebuffer->Clear();

		m_Scene.OnUpdate(dt, *m_SceneCamera);

		framebuffer->Unbind();
	}

	f32 TestLayer::DrawTitleBar() {
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
		constexpr f32 buttonsAreaWidth = 120;

		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));

		if (ImGui::IsItemHovered()) {
			Application::Get()->GetWindow()->RegisterOverTitlebar(true);
		}

		// Minimize Button
		{
			const float rightOffset = ImGui::GetWindowWidth() - 120.0f;
			ImGui::SameLine();
			ImGui::SetCursorPosX(rightOffset - windowPadding.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f + windowPadding.y);
			ImGui::SetItemAllowOverlap();
			GUI::ImageButton(*m_MinimizeIconTexture, { 40.f, 40.f }, [&]() {
				Application::Get()->GetWindow()->Minimize();
			});
		}

		// Maximize / shrink button
		{
			const float rightOffset = ImGui::GetWindowWidth() - 80.0f;
			ImGui::SameLine();
			ImGui::SetCursorPosX(rightOffset - windowPadding.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f + windowPadding.y);
			ImGui::SetItemAllowOverlap();
			if (m_WindowMaximized) {
				GUI::ImageButton(*m_RestoreIconTexture, { 40.f, 40.f }, [&]() {
					Application::Get()->GetWindow()->Restore();
					m_WindowMaximized = false;
				});
			} else {
				GUI::ImageButton(*m_MaximizeIconTexture, { 40.f, 40.f }, [&]() {
					Application::Get()->GetWindow()->Maximize();
					m_WindowMaximized = true;
				});
			}
		}

		// Close button
		{
			const float rightOffset = ImGui::GetWindowWidth() - 40.0f;
			ImGui::SameLine();
			ImGui::SetCursorPosX(rightOffset - windowPadding.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f + windowPadding.y);
			ImGui::SetItemAllowOverlap();

			GUI::ImageButton(*m_CloseIconTexture, { 40.f, 40.f }, []() {
				Application::Get()->Close();
			});
		}

		return titlebarHeight;
	}

	void TestLayer::DrawMenuBar() {
		
	}

	void TestLayer::OnRender() {
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

		const bool isMaximized = Application::Get()->GetWindow()->IsCurrentlyMaximized();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
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

		ImGui::Begin("Scene");

		static bool check = true;
		ImGui::Checkbox("The label", &check);

		ImGui::End();

		ImGui::Begin("Viewport");

		m_IsViewportFocused = ImGui::IsWindowFocused();

		const ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
		m_ViewportSize.x = currentViewportSize.x;
		m_ViewportSize.y = currentViewportSize.y;

		const ImTextureID textureID = GUI::GetTextureID(framebuffer->GetColorAttachmentRendererID());
		ImGui::Image(textureID, currentViewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

		ImGui::Begin("Components");

		ImGui::End();

		ImGui::Begin("Assets");

		ImGui::End();

		ImGui::Begin("Console");

		ImGui::End();

		ImGui::Begin("Statistics");

		static float m_FpsValues[200];
		static std::vector<f32> m_FrameTimes;

		f32 avg = 0.0f;

		const size_t size = m_FrameTimes.size();
		if (size >= 200)
			m_FrameTimes.erase(m_FrameTimes.begin());

		m_FrameTimes.emplace_back(ImGui::GetIO().Framerate);

		for (u32 i = 0; i < size; i++) {
			const float frameTime = m_FrameTimes[i];
			m_FpsValues[i] = frameTime;
			avg += frameTime;
		}

		avg /= static_cast<f32>(size);

		ImGui::PushItemWidth(-1);
		ImGui::PlotLines("##FPS", m_FpsValues, static_cast<int>(size));
		ImGui::PopItemWidth();

		ImGui::Text("FPS: %lf", static_cast<f64>(avg));
		ImGui::Text("Frame time (ms): %lf", (1.0 / static_cast<f64>(avg)) * 1000.0f);

		static bool VSync = true;
		if (ImGui::Checkbox("VSync", &VSync)) {
			Application::Get()->GetWindow()->SetVSync(VSync);
		}

		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Quads: %d", Renderer2D::GetStats().QuadCount);
		ImGui::Text("Vertices: %d", Renderer2D::GetStats().GetTotalVertexCount());
		ImGui::Text("Indices: %d", Renderer2D::GetStats().GetTotalIndexCount());

		ImGui::End();

		//ImGui::ShowDemoWindow();
	}

}
