#include "SceneViewportPanel.hpp"

#include <ImGuizmo.h>

#include "Core/Utils/Utils.hpp"
#include "GUI/Icons.hpp"
#include "Core/AssetManager.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Editor/EditorSettings.hpp"
#include "Managers/SelectionManager.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace SW {

	SceneViewportPanel::SceneViewportPanel(const char* name)
		: Panel(name, SW_ICON_TERRAIN, true)
	{
		EventSystem::Register(EVENT_CODE_MOUSE_WHEEL, nullptr, [this](Event event, void* sender, void* listener) -> bool {
			const f32 xOffset = event.Payload.f32[0];
			const f32 yOffset = event.Payload.f32[1];

			if (!m_IsViewportFocused)
				return false;

			if (m_ActiveScene->GetCurrentState() != SceneState::Edit)
				return false;

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

		EventSystem::Register(EVENT_CODE_MOUSE_BUTTON_PRESSED, nullptr, [this](Event event, void* sender, void* listener) -> bool {
			MouseCode code = (MouseCode)event.Payload.u16[0];

			return OnMouseButtonPressed(code);
		});

		const FramebufferSpecification spec = { 
			.Width = 1280,
			.Height = 720,
			.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth }
		};

		m_Framebuffer = new Framebuffer(spec);

		m_SceneCamera = new SceneCamera((f32)(spec.Width / spec.Height));

		m_ActiveScene = new Scene();
	}

	SceneViewportPanel::~SceneViewportPanel()
	{
		delete m_SceneCamera;
		delete m_Framebuffer;
		delete m_ActiveScene;
		delete m_SceneCopy;
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
			m_ActiveScene->OnViewportResize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();

		m_Framebuffer->Bind();

		m_Framebuffer->Clear();

		m_Framebuffer->ClearAttachment(1, -1);

		if (m_ActiveScene->BeginRendering(m_SceneCamera)) {
			m_ActiveScene->OnUpdate(dt, *m_SceneCamera);

			if (EditorSettings::Get().ShowPhysicsColliders) {
			
				// Box Colliders
				{
					for (auto&& [handle, tc, bcc] : m_ActiveScene->GetRegistry().GetEntitiesWith<TransformComponent, BoxCollider2DComponent>().each()) {
						glm::vec3 translation = tc.Position + glm::vec3(bcc.Offset.x, bcc.Offset.y, 0.001f);
						glm::vec3 scale = tc.Scale * glm::vec3(bcc.Size.x * 2.0f, bcc.Size.y * 2.0f, 1.0f);

						glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Position)
							* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
							* glm::translate(glm::mat4(1.0f), glm::vec3(bcc.Offset, 0.001f))
							* glm::scale(glm::mat4(1.0f), scale);

						Renderer2D::DrawRect(transform, glm::vec4(1, 1, 0, 1));
					}
				}
			}

			m_ActiveScene->EndRendering();
		}

		m_Framebuffer->Unbind();
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

			// if (SelectionManager::IsSelected()) {
			// 	ImGuizmo::SetID(1);
			// 	ImGuizmo::SetOrthographic(false);
			// 	ImGuizmo::SetDrawlist();
			// 	ImGuizmo::SetRect(m_ViewportBoundsMin.x, m_ViewportBoundsMin.y, m_ViewportBoundsMax.x - m_ViewportBoundsMin.x, m_ViewportBoundsMax.y - m_ViewportBoundsMin.y);
			// 	ImGuizmo::Enable(true);

			// 	ImGuiIO& io = ImGui::GetIO();
			// 	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

			// 	glm::mat4 cameraView = m_SceneCamera->GetViewMatrix();
			// 	glm::mat4 cameraProjection = m_SceneCamera->GetProjectionMatrix();

			// 	m_GizmoType = ImGuizmo::OPERATION::TRANSLATE; // todo remove

			// 	Entity selected = m_ActiveScene->GetEntityByID(SelectionManager::GetSelectionID());

			// 	TransformComponent& tc = selected.GetComponent<TransformComponent>();

			// 	glm::mat4 transform = tc.GetTransform();

			// 	const bool snap = ImGui::IsKeyDown(ImGuiKey_LeftCtrl);
			// 	f32 snapValue = 0.5f;
			// 	if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			// 		snapValue = 45.0f;

			// 	f32 snapValues[3] = { snapValue, snapValue, snapValue };
			// 	f32 bounds[6] = { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };

			// 	ImGuizmo::Manipulate(
			// 		cameraView.ValuePtrConst(), cameraProjection.ValuePtrConst(),
			// 		static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::LOCAL,
			// 		transform.ValuePtr()
			// 	);

			// 	if (ImGuizmo::IsUsing()) {

			// 	}
			// }

			OnEnd();
		}
	}

	void SceneViewportPanel::RenderSceneToolbar()
	{
		const ImVec2 windowPosition = ImGui::GetWindowPos();
		const ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
		const ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();

		m_ViewportBoundsMin = { viewportMinRegion.x + windowPosition.x, viewportMinRegion.y + windowPosition.y };
		m_ViewportBoundsMax = { viewportMaxRegion.x + windowPosition.x, viewportMaxRegion.y + windowPosition.y };

		ImVec2 windowSize = ImGui::GetWindowSize();

		ImGui::SetNextWindowPos(
			ImVec2(windowPosition.x + windowSize.x / 2.0f - 23.f, windowPosition.y + 40.f)
		);
		ImGui::SetNextWindowSize({ 48.f, 48.f });
		ImGui::SetNextWindowBgAlpha(0.0f);

		bool isOpen = true;

		ImGui::Begin("##scene_toolbar", &isOpen, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration);

		Texture2D* buttonTexture = m_ActiveScene->GetCurrentState() == SceneState::Edit ?
			AssetManager::GetTexture2D("assets/icons/editor/PlayButton.png") :
			AssetManager::GetTexture2D("assets/icons/editor/StopButton.png");

		const ImVec2& pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x + 4.f, pos.y + 4.f });

		if (GUI::ImageButton(*buttonTexture, { 40.f, 40.f })) {
			const SceneState currentState = m_ActiveScene->GetCurrentState();

			if (currentState == SceneState::Edit) {
				m_SceneCopy = m_ActiveScene->DeepCopy();

				m_ActiveScene->SetNewState(SceneState::Play);
				m_ActiveScene->OnRuntimeStart();
			} else if (currentState == SceneState::Play) {
				m_ActiveScene->OnRuntimeStop();

				delete m_ActiveScene;
				m_ActiveScene = nullptr;

				m_ActiveScene = m_SceneCopy->DeepCopy();

				delete m_SceneCopy;
				m_SceneCopy = nullptr;

				m_ActiveScene->SetNewState(SceneState::Edit);
			}
		}

		ImGui::End();
	}

	void SceneViewportPanel::RenderGizmoToolbar()
	{

	}

	bool SceneViewportPanel::OnMouseButtonPressed(MouseCode code)
	{
		if (!m_IsViewportFocused)
			return false;

		f32 mouseX = ImGui::GetMousePos().x;
		f32 mouseY = ImGui::GetMousePos().y;
		
		mouseX -= m_ViewportBoundsMin.x;
		mouseY -= m_ViewportBoundsMin.y;

		const glm::vec2 viewportSize = m_ViewportBoundsMax - m_ViewportBoundsMin;

		mouseY = viewportSize.y - mouseY;

		if (
			mouseX >= 0 && mouseY >= 0 &&
			mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y
		) {
			int pickedID = m_Framebuffer->ReadPixel(1, (int)mouseX, (int)mouseY);
			
			if (pickedID != -1) {
				const IDComponent& idc = m_ActiveScene->GetRegistry().GetRegistryHandle().get<IDComponent>((entt::entity)pickedID);
				
				SelectionManager::SelectByID(idc.ID);
			}
		}
		
		return false;
	}

}
