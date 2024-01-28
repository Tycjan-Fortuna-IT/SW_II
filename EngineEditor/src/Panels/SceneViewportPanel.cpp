#include "SceneViewportPanel.hpp"

#include "Core/Utils/Utils.hpp"
#include "GUI/Icons.hpp"
#include "Core/AssetManager.hpp"

namespace SW {

	SceneViewportPanel::SceneViewportPanel(const char* name)
		: Panel(name, SW_ICON_TERRAIN, true)
	{
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

		const FramebufferSpecification spec = { 
			.Width = 1280,
			.Height = 720,
			.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth }
		};

		m_Framebuffer = new Framebuffer(spec);

		m_SceneCamera = new SceneCamera((f32)(spec.Width / spec.Height));

		m_Scene = new Scene();
		auto entt = m_Scene->CreateEntity();
		entt.AddComponent<SpriteComponent>(AssetManager::GetTexture2D("assets/icons/SW_Icon.png", false));
	}

	SceneViewportPanel::~SceneViewportPanel()
	{
		delete m_SceneCamera;
		delete m_Framebuffer;
		delete m_Scene;
	}

	void SceneViewportPanel::OnUpdate(Timestep dt)
	{
		FramebufferSpecification spec = m_Framebuffer->GetSpecification();

		if (m_IsViewportFocused)
			m_SceneCamera->OnUpdate(dt);

		if (
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // if it's a valid size viewport
			(spec.Width != (u32)m_ViewportSize.x || spec.Height != (u32)m_ViewportSize.y) // if it changed
		) {
			m_Framebuffer->Resize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
			m_SceneCamera->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene->OnViewportResize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();

		m_Framebuffer->Bind();

		m_Framebuffer->Clear();

		m_Scene->OnUpdate(dt, *m_SceneCamera);

		m_Framebuffer->Unbind();
	}

	void SceneViewportPanel::RenderSceneToolbar()
	{
		ImVec2 windowPosition = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImGui::SetNextWindowPos(
			ImVec2(windowPosition.x + windowSize.x / 2.0f - 23.f, windowPosition.y + 40.f)
		);
		ImGui::SetNextWindowSize({ 48.f, 48.f });
		ImGui::SetNextWindowBgAlpha(0.0f);
		
		bool isOpen = true;

		ImGui::Begin("##scene_toolbar", &isOpen, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration);

		Texture2D* buttonTexture = m_Scene->GetCurrentState() == SceneState::Edit ?
			AssetManager::GetTexture2D("assets/icons/editor/PlayButton.png") :
			AssetManager::GetTexture2D("assets/icons/editor/StopButton.png");

		const ImVec2& pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x + 4.f, pos.y + 4.f });

		if (GUI::ImageButton(*buttonTexture, { 40.f, 40.f })) {
			m_Scene->SetNewState(m_Scene->GetCurrentState() == SceneState::Edit ? SceneState::Play : SceneState::Edit);
		}

		ImGui::End();
	}

	void SceneViewportPanel::RenderGizmoToolbar()
	{

	}

	void SceneViewportPanel::OnRender()
	{
		GUI::ScopedStyle NoWindowPadding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f , 0.f });

		if (OnBegin()) {
			m_IsViewportFocused = ImGui::IsWindowFocused();

			const ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
			m_ViewportSize.x = currentViewportSize.x;
			m_ViewportSize.y = currentViewportSize.y;

			const ImTextureID textureID = GUI::GetTextureID(m_Framebuffer->GetColorAttachmentRendererID());
			ImGui::Image(textureID, currentViewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			RenderSceneToolbar();

			OnEnd();
		}
	}

}
