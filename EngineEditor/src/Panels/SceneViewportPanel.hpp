/**
 * @file SceneViewportPanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-01-30
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <Engine.hpp>

#include "GUI/Panel.hpp"

namespace SW {

	/**
	 * @brief The SceneViewportPanel class represents a panel that displays the current scene viewport.
	 */
	class SceneViewportPanel final : public Panel
	{
	public:
		/**
		 * @brief Constructs a SceneViewportPanel object with an optional name.
		 * 
		 * @param name The name of the panel. Default is "Stats".
		 */
		explicit SceneViewportPanel(const char* name = "Scene Viewport");

		/**
		 * @brief Default destructor for the SceneViewportPanel class.
		 */
		~SceneViewportPanel() override;

		/** @brief Copy constructor (deleted). */
		SceneViewportPanel(const SceneViewportPanel& other) = delete;
		/** @brief Move constructor (deleted). */
		SceneViewportPanel(SceneViewportPanel&& other) = delete;
		/** @brief Copy assignment operator (deleted). */
		SceneViewportPanel& operator=(const SceneViewportPanel& other) = delete;
		/** @brief Move assignment operator (deleted). */
		SceneViewportPanel& operator=(SceneViewportPanel&& other) = delete;

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
		 * @brief Gets the current scene.
		 * 
		 * @return Scene* The current scene.
		 */
		Scene* GetCurrentScene() { return m_Scene; }

		/**
		 * @brief Renders the scene toolbar.
		 */
		void RenderSceneToolbar();

		/**
		 * @brief Renders the gizmo toolbar.
		 */
		void RenderGizmoToolbar();

		/**
		 * @brief Renders the gizmo toolbar.
		 * 
		 * @return f32 Returns calculated aspect ratio of the scene viewport.
		 */
		f32 GetViewportAspectRatio() { return m_SceneCamera->GetAspectRatio(); }

	private:
		Scene* m_Scene = nullptr;				/** @brief The current scene. */
		SceneCamera* m_SceneCamera = nullptr;	/** @brief The current scene camera. */
		Framebuffer* m_Framebuffer = nullptr;	/** @brief The current framebuffer. */
		bool m_IsViewportFocused = false;		/** @brief Is the viewport focused. */
		Vector2<f32> m_ViewportSize = { 50.f };	/** @brief The viewport size. */
	};

}
