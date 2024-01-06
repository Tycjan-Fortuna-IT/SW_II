#pragma once

#include <Engine.hpp>

#include "Core/OpenGL/Shader.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Core/Utils/Random.hpp"
#include "Core/Math/Matrix4.hpp"
#include "Core/Math/Vector2.hpp"
#include "Core/OpenGL/Framebuffer.hpp"


namespace SW {

	class TestLayer final : public Layer {
	public:
		explicit TestLayer(const std::string& name)
			: Layer(name) {}

		u32 VBO, VAO, EBO = 0;

		Shader* shader = nullptr;
		Texture2D* boxTexture = nullptr;
		Texture2D* faceTexture = nullptr;

		Matrix4<f32> testMatrix = { 1.0f };

		Framebuffer* framebuffer = nullptr;

		Vector2<f32> m_ViewportSize;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(float dt) override;
		void OnRender() override;
	};

}
