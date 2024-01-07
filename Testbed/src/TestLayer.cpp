#include "TestLayer.hpp"

#include <glad/glad.h>

#include "imgui.h"
#include "Core/KeyCode.hpp"
#include "Core/Debug/LogSystem.hpp"
#include "Core/ECS/Components.hpp"
#include "Core/ECS/EntityRegistry.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Math/Math.hpp"
#include "Core/Math/Vector3.hpp"

namespace SW {

	void TestLayer::OnAttach() {
		APP_TRACE("TestLayer::OnAttach()");

		const Vector3<f32> vec = { 1.2f, 1.4f, 1.3f };
		const Matrix4<f32> mat = { 2.4f };

		shader = new Shader("assets/shaders/Initial.vert.glsl", "assets/shaders/Initial.frag.glsl");
		boxTexture = new Texture2D("assets/textures/container_512x512.jpg");
		faceTexture = new Texture2D("assets/textures/awesomeface_512x512.png");

		FramebufferSpecification spec = { 1280, 720 };
		framebuffer = new Framebuffer(spec);

		// matrix to test the inverse
		Matrix4<f32> inverseTest = { 2.f };
		inverseTest[3] = 3.f;
		inverseTest[4] = 4.f;
		inverseTest[8] = 4.f;
		inverseTest[12] = 4.f;
		Matrix4<f32> result = Math::Inverse<f32>(inverseTest);

		APP_DEBUG("{}", result.ToString());

		//m_CameraController = 
		//viewProjection

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

		Vector3<f32>* t = new Vector3<f32>({ 1.f, 1.f, 1.f });

		EventSystem::Register(EventCode::EVENT_CODE_KEY_PRESSED, nullptr, [this](Event event, void* sender, void* listener) -> bool {
			KeyCode code = (KeyCode)event.Payload.u16[0];
			APP_DEBUG("Key pressed: {}", (int)code);

			if (code == KeyCode::A) {
				Vector3<f32> offset = { -0.1f, 0.0f, 0.0f };

				testMatrix.Translate(offset);
			} else if (code == KeyCode::D) {
				Vector3<f32> offset = { 0.1f, 0.0f, 0.0f };

				testMatrix.Translate(offset);
			} else if (code == KeyCode::W) {
				Vector3<f32> offset = { 0.0f, 0.1f, 0.0f };

				testMatrix.Translate(offset);
			} else if (code == KeyCode::S) {
				Vector3<f32> offset = { 0.0f, -0.1f, 0.0f };

				testMatrix.Translate(offset);
			} else {
				return false;
			}

			APP_DEBUG("{}", testMatrix.ToString());

			shader->UploadUniformMat4("u_Transform", testMatrix);

			return true;
		});
	}

	void TestLayer::OnDetach() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		delete shader;
		delete boxTexture;
		delete faceTexture;

		EntityRegistry registry;

		const Vector3<f32> vec1 = { 1.2f, 1.4f, 1.3f };
		const Vector3<f32> vec2 = { 2.2f, 2.4f, 2.3f };
		const Vector3<f32> vec3 = { 3.2f, 3.4f, 3.3f };

		Entity entity = registry.CreateEntity();
		entity.AddComponent<TransformComponent>(vec1, vec2, vec3);

		TransformComponent transform = entity.GetComponent<TransformComponent>();
	}

	void TestLayer::OnUpdate(float dt) {
		FramebufferSpecification spec = framebuffer->GetSpecification();

		if (
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // if it's a valid size viewport
			(spec.Width != (u32)m_ViewportSize.x || spec.Height != (u32)m_ViewportSize.y) // if it changed
		) {
			framebuffer->Resize((u32)m_ViewportSize.x, (u32)m_ViewportSize.y);
			shader->UploadUniformMat4("u_ViewProjection", viewProjection);
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

	void TestLayer::OnRender() {
		//bool open = true;
		//ImGui::ShowDemoWindow(&open);

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGui::Begin("Panel");

		ImGui::End();

		ImGui::Begin("Viewport");

		const ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
		m_ViewportSize.x = currentViewportSize.x;
		m_ViewportSize.y = currentViewportSize.y;

		u32 textureID = framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, currentViewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
	}

}
