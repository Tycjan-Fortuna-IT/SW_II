#pragma once

#include <Engine.hpp>

#include "Core/OpenGL/Shader.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Core/Utils/Random.hpp"
#include "Core/Math/Matrix4.hpp"
#include "Core/Math/Vector2.hpp"
#include "Core/OpenGL/Framebuffer.hpp"
#include "Core/OpenGL/OrthographicCameraController.hpp"
#include "Core/Scene/Scene.hpp"


namespace SW {

	class TestLayer final : public Layer {
	public:
		explicit TestLayer(const std::string& name)
			: Layer(name) {}

		Scene m_Scene;

		Entity m_CameraEntity;
		Entity m_Square1;
		Entity m_Square2;

		std::shared_ptr<Shader> m_Shader = nullptr;
		Texture2D* boxTexture = nullptr;
		Texture2D* faceTexture = nullptr;

		Texture2D* m_IconTexture = nullptr;
		Texture2D* m_CloseIconTexture = nullptr;
		Texture2D* m_MaximizeIconTexture = nullptr;
		Texture2D* m_MinimizeIconTexture = nullptr;
		Texture2D* m_RestoreIconTexture = nullptr;

		Matrix4<f32> testMatrix = { 1.0f };
		Matrix4<f32> viewProjection = { 1.0f };

		OrthographicCameraController* m_CameraController;

		Framebuffer* framebuffer = nullptr;

		Vector2<f32> m_ViewportSize;

		bool m_WindowMaximized = true;
		bool m_IsViewportFocused = false;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(float dt) override;
		void OnRender() override;

		f32 DrawTitleBar();
		void DrawMenuBar();
	};

}
