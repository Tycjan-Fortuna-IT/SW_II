#include "TestLayer.hpp"

#include <glad/glad.h>
#include <imgui.h>

#include "imgui_internal.h"
#include "Core/Debug/LogSystem.hpp"
#include "Core/ECS/Components.hpp"
#include "GLFW/glfw3.h"
#include "GUI/Appearance.hpp"
#include "GUI/Colors.hpp"

namespace SW {

	void TestLayer::OnAttach() {
		APP_TRACE("TestLayer::OnAttach()");

		shader = new Shader("assets/shaders/Initial.vert.glsl", "assets/shaders/Initial.frag.glsl");

		boxTexture = new Texture2D("assets/textures/container_512x512.jpg");
		faceTexture = new Texture2D("assets/textures/awesomeface_512x512.png");
		m_IconTexture = new Texture2D("assets/icons/SW_Icon.png", false);

		const FramebufferSpecification spec = { 1280, 720 };
		framebuffer = new Framebuffer(spec);

		m_CameraController = new OrthographicCameraController((f32)spec.Width / (f32)spec.Height);

		GUI::Appearance::ApplyStyle(GUI::Style());
		GUI::Appearance::ApplyColors(GUI::Colors());

		float vertices[] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
		};

		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		shader->Bind();
		shader->UploadUniformInt("u_Texture1", 0);
		shader->UploadUniformInt("u_Texture2", 1);

		shader->UploadUniformMat4("u_Transform", testMatrix);
	}

	void TestLayer::OnDetach() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		delete shader;
		delete boxTexture;
		delete faceTexture;
	}

	void TestLayer::OnUpdate(float dt) {
		FramebufferSpecification spec = framebuffer->GetSpecification();

		m_CameraController->OnUpdate(dt);

		Matrix4<f32> testTransform = { 1.0f };
		testTransform.Translate(m_CameraController->GetCamera().GetPosition());
		testTransform.RotateZ(m_CameraController->GetCamera().GetRotation());

		shader->UploadUniformMat4("u_Transform", testTransform);

		if (
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // if it's a valid size viewport
			(spec.Width != (u32)m_ViewportSize.x || spec.Height != (u32)m_ViewportSize.y) // if it changed
		) {
			framebuffer->Resize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
			m_CameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y);
			shader->UploadUniformMat4("u_ViewProjection", m_CameraController->GetCamera().GetViewProjectionMatrix());
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->Bind();
		boxTexture->Bind(0);
		faceTexture->Bind(1);

		framebuffer->Bind();
		framebuffer->Clear();

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

			drawList->AddImage((ImTextureID)m_IconTexture->GetHandle(), logoRectStart, logoRectMax);
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
			if (ImGui::Button("_", { 40.f, 40.f })) {
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
			if (ImGui::Button("O", { 40.f, 40.f })) {
				if (m_WindowMaximized) {
					Application::Get()->GetWindow()->Restore();
					m_WindowMaximized = false;
				} else {
					Application::Get()->GetWindow()->Maximize();
					m_WindowMaximized = true;
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
			if (ImGui::Button("X", { 40.f, 40.f })) {
				Application::Get()->Close();
			}
		}

		return titlebarHeight;
	}

	void TestLayer::OnRender() {
		Application::Get()->GetWindow()->RegisterOverTitlebar(false);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		io.ConfigWindowsResizeFromEdges = io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		const bool isMaximized = (bool)glfwGetWindowAttrib(Application::Get()->GetWindow()->GetHandle(), GLFW_MAXIMIZED);

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

		ImGui::Begin("Panel");

		static bool check = true;
		ImGui::Checkbox("The label", &check);

		ImGui::End();

		ImGui::Begin("Viewport");

		const ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
		m_ViewportSize.x = currentViewportSize.x;
		m_ViewportSize.y = currentViewportSize.y;

		u32 textureID = framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, currentViewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

		ImGui::ShowDemoWindow();
	}

}
