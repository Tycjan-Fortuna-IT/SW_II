#pragma once

#include <Engine.hpp>

#include <imgui_internal.h>

namespace SW {

	class Panel;

	class TestLayer final : public Layer {
	public:
		explicit TestLayer(const std::string& name)
			: Layer(name) {}

		Scene m_Scene;

		Entity m_CameraEntity;
		Entity m_Square1;
		Entity m_Square2;

		std::vector<Panel*> m_Panels;

		std::shared_ptr<Shader> m_Shader = nullptr;
		std::shared_ptr<Texture2D> boxTexture = nullptr;
		Texture2D* faceTexture = nullptr;

		Texture2D* m_IconTexture = nullptr;
		Texture2D* m_CloseIconTexture = nullptr;
		Texture2D* m_MaximizeIconTexture = nullptr;
		Texture2D* m_MinimizeIconTexture = nullptr;
		Texture2D* m_RestoreIconTexture = nullptr;

		Matrix4<f32> testMatrix = { 1.0f };
		Matrix4<f32> viewProjection = { 1.0f };

		SceneCamera* m_SceneCamera;

		Framebuffer* framebuffer = nullptr;

		Vector2<f32> m_ViewportSize;

		bool m_WindowMaximized = true;
		bool m_IsViewportFocused = false;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep dt) override;
		void OnRender() override;

		bool OnKeyPressed(KeyCode code);

		f32 DrawTitleBar();
		void DrawMenuBar();
	};

}
