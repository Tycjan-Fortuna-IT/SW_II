#include "SceneViewportPanel.hpp"

#include <ImGuizmo.h>
#include <glm/ext/matrix_transform.hpp>

#include "Core/Utils/Utils.hpp"
#include "GUI/Icons.hpp"
#include "Core/AssetManager.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Editor/EditorSettings.hpp"
#include "Managers/SelectionManager.hpp"
#include "Core/Scene/SceneSerializer.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Project/Project.hpp"
#include "Core/OpenGL/Font.hpp"

namespace SW {

	SceneViewportPanel::SceneViewportPanel(const char* name)
		: Panel(name, SW_ICON_TERRAIN, true)
	{
		EventSystem::Register(EVENT_CODE_WINDOW_RESIZED, nullptr, [this](Event event, void* sender, void* listener) -> bool {
			const i32 width = event.Payload.i32[0];
			const i32 height = event.Payload.i32[1];

			if (!m_IsViewportFocused) return false;

			m_EditorCamera->SetViewportSize((f32)width, (f32)height);

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

		m_EditorCamera = new EditorCamera();
		m_EditorCamera->SetViewportSize((f32)spec.Width, (f32)spec.Height);

		m_ActiveScene = new Scene("");
	}

	SceneViewportPanel::~SceneViewportPanel()
	{
		delete m_EditorCamera;
		delete m_Framebuffer;
		delete m_ActiveScene;
		delete m_SceneCopy;
	}

	void SceneViewportPanel::OnUpdate(Timestep dt)
	{
		FramebufferSpecification spec = m_Framebuffer->GetSpecification();

		if (
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // if it's a valid size viewport
			(spec.Width != (u32)m_ViewportSize.x || spec.Height != (u32)m_ViewportSize.y) // if it changed
		) {
			m_Framebuffer->Resize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
			m_EditorCamera->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
		}

		m_EditorCamera->OnUpdate(dt);

		const glm::vec3& position = m_EditorCamera->GetPosition();
		const glm::vec2 yawPitch = glm::vec2(m_EditorCamera->GetYaw(), m_EditorCamera->GetPitch());
		
		glm::vec3 finalPosition = position;
		glm::vec2 finalYawPitch = yawPitch;

		if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && m_IsViewportFocused) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			glm::vec2 newMousePosition = Input::GetMousePosition();

			if (!m_UsingEditorCamera) {
				m_UsingEditorCamera = true;
				m_LockedMousePosition = newMousePosition;
			}

			Input::SetMousePosition(m_LockedMousePosition);

			const glm::vec2 change = (newMousePosition - m_LockedMousePosition) * m_MouseSensitivity;
			finalYawPitch.x += change.x;
			finalYawPitch.y = glm::clamp(finalYawPitch.y - change.y, -89.9f, 89.9f);
		} else {
			m_UsingEditorCamera = false;
		}

		if (m_IsViewportFocused) {
			f32 maxMoveSpeed = m_MaxMoveSpeed * (ImGui::IsKeyDown(ImGuiKey_LeftShift) ? 3.0f : 1.0f);
			if (ImGui::IsKeyDown(ImGuiKey_W))
				finalPosition += m_EditorCamera->GetForward() * maxMoveSpeed;
			else if (ImGui::IsKeyDown(ImGuiKey_S))
				finalPosition -= m_EditorCamera->GetForward() * maxMoveSpeed;
			if (ImGui::IsKeyDown(ImGuiKey_D))
				finalPosition += m_EditorCamera->GetRight() * maxMoveSpeed;
			else if (ImGui::IsKeyDown(ImGuiKey_A))
				finalPosition -= m_EditorCamera->GetRight() * maxMoveSpeed;
			else if (ImGui::IsKeyDown(ImGuiKey_Space))
				finalPosition += m_EditorCamera->GetUp() * maxMoveSpeed;
			else if (ImGui::IsKeyDown(ImGuiKey_C))
				finalPosition -= m_EditorCamera->GetUp() * maxMoveSpeed;
		}

		glm::vec3 dampedPosition = Math::SmoothDamp(position, finalPosition, m_TranslationVelocity, m_TranslationDampening, 10000.0f, dt);
		glm::vec2 dampedYawPitch = Math::SmoothDamp(yawPitch, finalYawPitch, m_RotationVelocity, m_RotationDampening, 1000.0f, dt);
		
		m_EditorCamera->SetPosition(dampedPosition);
		m_EditorCamera->SetYaw(dampedYawPitch.x);
		m_EditorCamera->SetPitch(dampedYawPitch.y);

		Renderer2D::ResetStats();

		m_Framebuffer->Bind();

		m_Framebuffer->Clear();

		m_Framebuffer->ClearAttachment(1, -1);

		if (m_ActiveScene->BeginRendering(m_EditorCamera)) {
			m_ActiveScene->OnUpdate(dt);

			if (EditorSettings::Get().ShowPhysicsColliders) {
			
				// Box Colliders Visualization
				{
					for (auto&& [handle, bcc] : m_ActiveScene->GetRegistry().GetEntitiesWith<BoxCollider2DComponent>().each()) {
						Entity entity = { handle, m_ActiveScene };

						glm::mat4 transform = entity.GetWorldSpaceTransformMatrix();

						transform *= glm::translate(glm::mat4(1.0f), glm::vec3(bcc.Offset, 0.0f)) 
							* glm::scale(glm::mat4(1.0f), glm::vec3(2.0f * bcc.Size, 1.0f));

						Renderer2D::DrawRect(transform, glm::vec4(1, 1, 0, 1));
					}
				}

				// Circle Colliders Visualization
				{
					for (auto&& [handle, ccc] : m_ActiveScene->GetRegistry().GetEntitiesWith<CircleCollider2DComponent>().each()) {
						Entity entity = { handle, m_ActiveScene };

						TransformComponent transform = entity.GetWorldSpaceTransform();
						transform.Position += glm::vec3(ccc.Offset.x, ccc.Offset.y, 0.001f);
						transform.Scale *= glm::vec3(ccc.Radius * 2.0f);

						Renderer2D::DrawCircle(transform.GetTransform(), glm::vec4(1, 1, 0, 1), 0.02f);
					}
				}

				// Polygon Colliders Visualization
				{
					for (auto&& [handle, pcc] : m_ActiveScene->GetRegistry().GetEntitiesWith<PolygonCollider2DComponent>().each()) {
						Entity entity = { handle, m_ActiveScene };

						glm::mat4 transform = entity.GetWorldSpaceTransformMatrix();

						glm::vec3 translation = glm::vec3(0.0f);
						glm::vec3 rotation = glm::vec3(0.0f);
						glm::vec3 scale = glm::vec3(0.0f);

						Math::DecomposeTransform(transform, translation, rotation, scale);

						transform = glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(glm::quat(rotation));

						if (m_ActiveScene->GetCurrentState() == SceneState::Play) { // TODO: Investigate this visualization offset difference between play/edit state
							transform *= glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.001f));
						} else {
							transform *= glm::translate(glm::mat4(1.0f), glm::vec3(pcc.Offset.x, pcc.Offset.y, 0.001f));
						}

						for (u64 i = 0; i < pcc.Vertices.size(); ++i) {
							const glm::vec4 p0 = transform * glm::vec4(pcc.Vertices[i], 0.0f, 1.0f);
							const glm::vec4 p1 = transform * glm::vec4(pcc.Vertices[(i + 1) % pcc.Vertices.size()], 0.0f, 1.0f);

							Renderer2D::DrawLine(p0, p1, glm::vec4(1, 1, 0, 1));
						}
					}
				}

				// Distance Joint Visualization
				{
					for (auto&& [handle, djc] : m_ActiveScene->GetRegistry().GetEntitiesWith<DistanceJoint2DComponent>().each()) {
						Entity originEntity = { handle, m_ActiveScene };

						if (!djc.ConnectedEntityID)
							continue;

						Entity connectedEntity = m_ActiveScene->GetEntityByID(djc.ConnectedEntityID);

						if (!connectedEntity)
							continue;

						TransformComponent originTransform = originEntity.GetWorldSpaceTransform();
						TransformComponent connectedTransform = connectedEntity.GetWorldSpaceTransform();

						glm::vec3 origin = originTransform.Position + glm::vec3(djc.OriginAnchor.x, djc.OriginAnchor.y, 0.001f);
						glm::vec3 connected = connectedTransform.Position + glm::vec3(djc.ConnectedAnchor.x, djc.ConnectedAnchor.y, 0.001f);

						Renderer2D::DrawLine(origin, connected, glm::vec4(1.f, 0.5f, 0.f, 1.f));

						glm::mat4 originTransformMatrix = glm::translate(glm::mat4(1.0f), origin) 
							* glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 1.0f));
						glm::mat4 connectedTransformMatrix = glm::translate(glm::mat4(1.0f), connected)
							* glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 1.0f));

						Renderer2D::DrawCircle(originTransformMatrix, glm::vec4(1.f, 0.5f, 0.f, 1.f));
						Renderer2D::DrawCircle(connectedTransformMatrix, glm::vec4(1.f, 0.5f, 0.f, 1.f));
					}
				}

				// Revolution Joint Visualization
				{
					for (auto&& [handle, rjc] : m_ActiveScene->GetRegistry().GetEntitiesWith<RevolutionJoint2DComponent>().each()) {
						if (!rjc.ConnectedEntityID)
							continue;

						Entity originEntity = { handle, m_ActiveScene };

						TransformComponent originTransform = originEntity.GetWorldSpaceTransform();

						glm::vec3 origin = originTransform.Position + glm::vec3(rjc.OriginAnchor.x, rjc.OriginAnchor.y, 0.001f);

						glm::mat4 smallPointMatrix = glm::translate(glm::mat4(1.0f), origin)
							* glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 1.0f));
						glm::mat4 circleOutlineMatrix = glm::translate(glm::mat4(1.0f), origin)
							* glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));

						Renderer2D::DrawCircle(smallPointMatrix, glm::vec4(1.f, 0.5f, 0.f, 1.f));
						Renderer2D::DrawCircle(circleOutlineMatrix, glm::vec4(1.f, 0.5f, 0.f, 1.f), 0.1f);
					}
				}

				// Prismatic Joint Visualization
				{
					for (auto&& [handle, pjc] : m_ActiveScene->GetRegistry().GetEntitiesWith<PrismaticJoint2DComponent>().each()) {
						if (!pjc.ConnectedEntityID)
							continue;

						Entity originEntity = { handle, m_ActiveScene };

						TransformComponent originTransform = originEntity.GetWorldSpaceTransform();

						glm::vec3 moving = originTransform.Position + glm::vec3(pjc.OriginAnchor.x, pjc.OriginAnchor.y, 0.001f);

						glm::mat4 movingPointMatrix = glm::translate(glm::mat4(1.0f), moving)
							* glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 1.0f));

						Renderer2D::DrawCircle(movingPointMatrix, glm::vec4(1.f, 0.5f, 0.f, 1.f));

						f32 lower = -1.f;
						f32 upper = 1.f;
						f32 angle = -pjc.Angle;

						glm::vec3 lowerPoint = moving + glm::vec3(lower * glm::cos(angle), lower * glm::sin(angle), 0.001f);
						glm::vec3 upperPoint = moving + glm::vec3(upper * glm::cos(angle), upper * glm::sin(angle), 0.001f);

						Renderer2D::DrawLine(lowerPoint, upperPoint, glm::vec4(1.f, 0.5f, 0.f, 1.f));
					}
				}

				// Spring Joint Visualization
				{
					for (auto&& [handle, sjc] : m_ActiveScene->GetRegistry().GetEntitiesWith<SpringJoint2DComponent>().each()) {
						Entity originEntity = { handle, m_ActiveScene };

						if (!sjc.ConnectedEntityID)
							continue;

						Entity connectedEntity = m_ActiveScene->GetEntityByID(sjc.ConnectedEntityID);

						if (!connectedEntity)
							continue;

						TransformComponent originTransform = originEntity.GetWorldSpaceTransform();
						TransformComponent connectedTransform = connectedEntity.GetWorldSpaceTransform();

						glm::vec3 origin = originTransform.Position + glm::vec3(sjc.OriginAnchor.x, sjc.OriginAnchor.y, 0.001f);
						glm::vec3 connected = connectedTransform.Position + glm::vec3(sjc.ConnectedAnchor.x, sjc.ConnectedAnchor.y, 0.001f);

						Renderer2D::DrawLine(origin, connected, glm::vec4(1.f, 0.5f, 0.f, 1.f));

						glm::mat4 originTransformMatrix = glm::translate(glm::mat4(1.0f), origin)
							* glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 1.0f));
						glm::mat4 connectedTransformMatrix = glm::translate(glm::mat4(1.0f), connected)
							* glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 1.0f));

						Renderer2D::DrawCircle(originTransformMatrix, glm::vec4(1.f, 0.5f, 0.f, 1.f));
						Renderer2D::DrawCircle(connectedTransformMatrix, glm::vec4(1.f, 0.5f, 0.f, 1.f));
					}
				}

				// Wheel Joint Visualization
				{
					for (auto&& [handle, wjc] : m_ActiveScene->GetRegistry().GetEntitiesWith<WheelJoint2DComponent>().each()) {
						if (!wjc.ConnectedEntityID)
							continue;

						Entity originEntity = { handle, m_ActiveScene };

						TransformComponent originTransform = originEntity.GetWorldSpaceTransform();

						glm::vec3 origin = originTransform.Position + glm::vec3(wjc.OriginAnchor.x, wjc.OriginAnchor.y, 0.001f);

						glm::mat4 smallPointMatrix = glm::translate(glm::mat4(1.0f), origin)
							* glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 1.0f));
						glm::mat4 circleOutlineMatrix = glm::translate(glm::mat4(1.0f), origin)
							* glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));

						Renderer2D::DrawCircle(smallPointMatrix, glm::vec4(1.f, 0.5f, 0.f, 1.f));
						Renderer2D::DrawCircle(circleOutlineMatrix, glm::vec4(1.f, 0.5f, 0.f, 1.f), 0.1f);
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

		if (OnBegin(ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
			ImVec2 startCursorPos = ImGui::GetCursorPos();

			m_IsViewportFocused = ImGui::IsWindowFocused();

			const ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
			m_ViewportSize.x = currentViewportSize.x;
			m_ViewportSize.y = currentViewportSize.y;

			const ImTextureID textureID = GUI::GetTextureID(m_Framebuffer->GetColorAttachmentRendererID());
			ImGui::Image(textureID, currentViewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			// TODO perform validation of the file
			// TODO automatically stop scene if user wants to switch it when original is playing
			if (
				m_ActiveScene->GetCurrentState() == SceneState::Edit &&
				ImGui::BeginDragDropTarget()
			) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					std::filesystem::path path = ProjectContext::Get()->GetAssetDirectory() / static_cast<char*>(payload->Data);

					if (SelectionManager::IsSelected())
						SelectionManager::Deselect();

					delete GetCurrentScene();

					Scene* newScene = SceneSerializer::Deserialize(path.string());

					SetCurrentScene(newScene);
				}
				ImGui::EndDragDropTarget();
			}

			RenderSceneToolbar();

			glm::mat4 cameraProjection = m_EditorCamera->GetProjectionMatrix();
			glm::mat4 cameraView = m_EditorCamera->GetViewMatrix();
			
			ImGuizmo::SetID(1);
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportBoundsMin.x, m_ViewportBoundsMin.y, m_ViewportBoundsMax.x - m_ViewportBoundsMin.x, m_ViewportBoundsMax.y - m_ViewportBoundsMin.y);

			glm::mat4 finalCameraProjection = m_EditorCamera->GetProjectionMatrix();
			glm::mat4 finalCameraView = m_EditorCamera->GetViewMatrix();

			ImGuizmo::ViewManipulate(
				glm::value_ptr(finalCameraView), glm::value_ptr(finalCameraProjection),
				ImGuizmo::OPERATION::ROTATE_SCREEN, ImGuizmo::MODE::WORLD, glm::value_ptr(m_CubeViewMatrix), 8.0f,
				ImVec2(m_ViewportBoundsMax.x - 128, m_ViewportBoundsMin.y), ImVec2(128, 128), 0x10101010
			);

			if (!m_UsingEditorCamera) {
				const glm::mat4 inverted = glm::inverse(finalCameraView);
				const glm::vec3 direction = -glm::vec3(inverted[2]);
				f32 yaw = glm::atan(direction.z, direction.x);
				f32 pitch = glm::asin(direction.y);
				m_EditorCamera->SetPitch(pitch);
				m_EditorCamera->SetYaw(yaw);
			}

			if (SelectionManager::IsSelected() && m_ActiveScene->GetCurrentState() != SceneState::Play) {
				Entity selectedEntity = m_ActiveScene->GetEntityByID(SelectionManager::GetSelectionID());

				TransformComponent& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = selectedEntity.GetWorldSpaceTransformMatrix();

				// Snapping
				const bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
				f32 snapValue = 0.5f; // Snap to 0.5m for translation/scale
				
				// Snap to 45 degrees for rotation
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				f32 snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(
					glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr
				);

				if (ImGuizmo::IsUsing()) {
					Entity parent = selectedEntity.GetParent();
					const glm::mat4& parentWorldTransform = parent ? parent.GetWorldSpaceTransformMatrix() : glm::mat4(1.0f);

					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(glm::inverse(parentWorldTransform) * transform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Position = translation;
					tc.Rotation += deltaRotation;
					tc.Scale = scale;
				}
			}

			// Transform Gizmos Button Group
			const f32 frameHeight = 1.3f * ImGui::GetFrameHeight();
			const ImVec2 framePadding = ImGui::GetStyle().FramePadding;
			const ImVec2 buttonSize = { frameHeight, frameHeight };
			const f32 buttonCount = 6.0f;
			const ImVec2 gizmoPosition = { m_ViewportBoundsMin.x + m_GizmoPosition.x, m_ViewportBoundsMin.y + m_GizmoPosition.y };
			const ImRect bb(gizmoPosition.x, gizmoPosition.y, gizmoPosition.x + buttonSize.x + 8, gizmoPosition.y + (buttonSize.y + 2) * (buttonCount + 0.5f));
			ImVec4 frameColor = ImGui::GetStyleColorVec4(ImGuiCol_Tab);
			frameColor.w = 0.5f;
			
			ImGui::RenderFrame(bb.Min, bb.Max, ImGui::GetColorU32(frameColor), false, ImGui::GetStyle().FrameRounding);
			m_IsGizmoBarHovered = ImGui::IsMouseHoveringRect(bb.Min, bb.Max);

			glm::vec2 tempGizmoPosition = m_GizmoPosition;

			ImGui::SetCursorPos({ startCursorPos.x + tempGizmoPosition.x + framePadding.x, startCursorPos.y + tempGizmoPosition.y });
			ImGui::BeginGroup();
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 1, 1 });

				ImVec2 draggerCursorPos = ImGui::GetCursorPos();
				ImGui::SetCursorPosX(draggerCursorPos.x + framePadding.x);
				ImGui::TextUnformatted(SW_ICON_DOTS_HORIZONTAL);
				ImVec2 draggerSize = ImGui::CalcTextSize(SW_ICON_DOTS_HORIZONTAL);
				draggerSize.x *= 2.0f;
				ImGui::SetCursorPos(draggerCursorPos);
				ImGui::InvisibleButton("GizmoDragger", draggerSize);
				static ImVec2 lastMousePosition = ImGui::GetMousePos();
				ImVec2 mousePos = ImGui::GetMousePos();
				if (ImGui::IsItemActive()) {
					m_GizmoPosition.x += mousePos.x - lastMousePosition.x;
					m_GizmoPosition.y += mousePos.y - lastMousePosition.y;
				}
				lastMousePosition = mousePos;

				constexpr f32 alpha = 0.6f;
				if (GUI::ToggleButton(SW_ICON_AXIS_ARROW, m_GizmoType == ImGuizmo::TRANSLATE, buttonSize, alpha, alpha))
					m_GizmoType = ImGuizmo::TRANSLATE;
				if (GUI::ToggleButton(SW_ICON_ROTATE_3D, m_GizmoType == ImGuizmo::ROTATE, buttonSize, alpha, alpha))
					m_GizmoType = ImGuizmo::ROTATE;
				if (GUI::ToggleButton(SW_ICON_ARROW_EXPAND, m_GizmoType == ImGuizmo::SCALE, buttonSize, alpha, alpha))
					m_GizmoType = ImGuizmo::SCALE;
				if (GUI::ToggleButton(SW_ICON_VECTOR_SQUARE, m_GizmoType == ImGuizmo::BOUNDS, buttonSize, alpha, alpha))
					m_GizmoType = ImGuizmo::BOUNDS;
				if (GUI::ToggleButton(SW_ICON_ARROW_EXPAND_ALL, m_GizmoType == ImGuizmo::UNIVERSAL, buttonSize, alpha, alpha))
					m_GizmoType = ImGuizmo::UNIVERSAL;
				if (GUI::ToggleButton(m_GizmoMode == ImGuizmo::WORLD ? SW_ICON_EARTH : SW_ICON_EARTH_OFF, m_GizmoMode == ImGuizmo::WORLD, buttonSize, alpha, alpha))
					m_GizmoMode = m_GizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;

				ImGui::PopStyleVar(2);
			}
			ImGui::EndGroup();

			OnEnd();
		}
	}

	void SceneViewportPanel::RenderSceneToolbar()
	{
		GUI::ScopedStyle Spacing(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });
		const ImVec2 windowPosition = ImGui::GetWindowPos();
		const ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
		const ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();

		m_ViewportBoundsMin = { viewportMinRegion.x + windowPosition.x, viewportMinRegion.y + windowPosition.y };
		m_ViewportBoundsMax = { viewportMaxRegion.x + windowPosition.x, viewportMaxRegion.y + windowPosition.y };

		const ImVec2 windowSize = ImGui::GetWindowSize();
		const ImVec2 buttonsPosition = { windowPosition.x + windowSize.x / 6.f, windowPosition.y - 15.f };

		const f32 buttonSize = 30.f;

		const SceneState currentState = m_ActiveScene->GetCurrentState();

		ImGui::SetCursorPos(buttonsPosition);
		ImGui::BeginGroup();
		if (GUI::ToggleButton(SW_ICON_PLAY, currentState == SceneState::Play, { buttonSize, buttonSize }, 0.6f, 0.6f)) {
			if (currentState != SceneState::Play) {
				m_SceneCopy = m_ActiveScene->DeepCopy();

				m_ActiveScene->SetNewState(SceneState::Play);
				m_ActiveScene->OnRuntimeStart();
			}
		}

		GUI::MoveMousePosX(buttonSize);
		GUI::MoveMousePosY(-buttonSize);

		if (GUI::ToggleButton(SW_ICON_STOP, currentState == SceneState::Edit, { buttonSize, buttonSize }, 0.6f, 0.6f)) {
			if (currentState != SceneState::Edit) {
				m_ActiveScene->OnRuntimeStop();

				delete m_ActiveScene;
				m_ActiveScene = nullptr;

				m_ActiveScene = m_SceneCopy->DeepCopy();

				delete m_SceneCopy;
				m_SceneCopy = nullptr;

				m_ActiveScene->SetNewState(SceneState::Edit);
			}
		}
		ImGui::EndGroup();
	}

	void SceneViewportPanel::RenderGizmoToolbar()
	{

	}

	bool SceneViewportPanel::OnMouseButtonPressed(MouseCode code)
	{
		if (!m_IsViewportFocused)
			return false;

		if (code != MouseCode::ButtonLeft)
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
			} else {
				if (!ImGuizmo::IsOver() && !ImGuizmo::IsUsingAny() && !m_IsGizmoBarHovered)
					SelectionManager::Deselect();
			}
		}
		
		return false;
	}

}
