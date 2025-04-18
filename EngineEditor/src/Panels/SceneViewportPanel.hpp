/**
 * @file SceneViewportPanel.hpp
 * @version 0.2.4
 * @date 2024-05-12
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#include "Core/Editor/EditorCamera.hpp"
#include "GUI/Panel.hpp"
#include "GUI/Popups.hpp"

namespace SW
{
	/**
	 * @brief The SceneViewportPanel class represents a panel that displays the current scene viewport.
	 */
	class SceneViewportPanel final : public Panel
	{
	public:
		SceneViewportPanel();
		~SceneViewportPanel() override;

		SceneViewportPanel(const SceneViewportPanel& other)            = delete;
		SceneViewportPanel(SceneViewportPanel&& other)                 = delete;
		SceneViewportPanel& operator=(const SceneViewportPanel& other) = delete;
		SceneViewportPanel& operator=(SceneViewportPanel&& other)      = delete;

		PanelType GetPanelType() const override { return PanelType::SceneViewportPanel; }

		/**
		 * @brief Called every frame to update the panel.
		 * @param dt The time since the last frame.
		 */
		void OnUpdate(Timestep dt) final override;

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() override;

		/**
		 * Checks if a scene is currently loaded.
		 *
		 * @return true if a scene is loaded, false otherwise.
		 */
		bool IsSceneLoaded() const { return m_ActiveScene != nullptr; }

		/**
		 * @brief Gets the current scene.
		 *
		 * @return Scene* The current scene.
		 */
		Scene* GetCurrentScene() { return m_ActiveScene; }

		/**
		 * @brief Sets the current scene.
		 *
		 * @param scene The scene to set as the current scene.
		 */
		void SetCurrentScene(Scene* scene) { m_ActiveScene = scene; }

		/**
		 * @brief Renders the gizmo toolbar.
		 *
		 * @return f32 Returns calculated aspect ratio of the scene viewport.
		 */
		f32 GetViewportAspectRatio() const { return m_EditorCamera->GetAspectRatio(); }

	private:
		Scene* m_ActiveScene = nullptr; /** @brief The active scene (the one that is visible). */
		Scene* m_SceneCopy   = nullptr;

		EditorCamera* m_EditorCamera = nullptr; /** @brief The current editor camera. */
		Framebuffer* m_Framebuffer   = nullptr; /** @brief The current framebuffer. */

		bool m_IsViewportFocused = false; /** @brief Is the viewport focused. */
		bool m_IsGizmoBarHovered = false;
		bool m_IsToolbarHovered  = false;

		glm::vec2 m_ViewportSize      = glm::vec2(50.f); /** @brief The viewport size. */
		glm::vec2 m_ViewportBoundsMin = glm::vec2(0.f);  /** @brief The minimum bounds of the viewport. */
		glm::vec2 m_ViewportBoundsMax = glm::vec2(0.f);  /** @brief The maximum bounds of the viewport. */

		int m_GizmoType = 7; // ImGuizmo::TRANSLATE
		int m_GizmoMode = -1;

		glm::vec2 m_LockedMousePosition = glm::vec2(0.0f);

		f32 m_MouseSensitivity     = 0.06f;
		f32 m_MaxMoveSpeed         = 10.0f;
		f32 m_TranslationDampening = 0.6f;
		f32 m_RotationDampening    = 0.3f;

		glm::vec3 m_TranslationVelocity  = glm::vec3(0.0f);
		glm::vec2 m_RotationVelocity     = glm::vec2(0.0f);
		glm::mat4 m_CubeViewMatrix       = glm::mat4(1.0f);
		glm::vec2 m_GizmoToolbarPosition = glm::vec2(1.0f);
		glm::vec2 m_ToolbarPosition      = glm::vec2(1.0f);

		bool m_UsingEditorCamera = false;

		GUI::Popups::FontSourceImportDialog fontImportDialog;

	private:
		/**
		 * Handles the event when a mouse button is pressed.
		 *
		 * @param code The code representing the mouse button that was pressed.
		 * @return True if the event was handled successfully, false otherwise.
		 */
		bool OnMouseButtonPressed(MouseCode code);

		/**
		 * Handles the event when a key is pressed.
		 *
		 * @param code The code representing the key that was pressed.
		 * @return True if the event was handled successfully, false otherwise.
		 */
		bool OnKeyPressed(KeyCode code);

		/**
		 * @brief Renders the scene toolbar.
		 *
		 * @param startCursorPos Initial cursor position.
		 */
		void RenderSceneToolbar(ImVec2 startCursorPos);

		/**
		 * @brief Renders the gizmo toolbar.
		 *
		 * @param startCursorPos Initial cursor position.
		 */
		void RenderGizmoToolbar(ImVec2 startCursorPos);
	};

} // namespace SW
