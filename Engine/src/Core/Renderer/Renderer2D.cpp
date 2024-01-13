#include "Renderer2D.hpp"

#include "Core/OpenGL/Texture2D.hpp"
#include "Core/OpenGL/VertexArray.hpp"
#include "Core/OpenGL/VertexBuffer.hpp"
#include "Core/OpenGL/IndexBuffer.hpp"
#include "Core/OpenGL/Shader.hpp"

namespace SW {

	struct QuadVertex final
	{
		Vector3<f32> Position;
		Vector4<f32> Color;
		Vector2<f32> TexCoord;

		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData final
	{
		Renderer2DData() = default;

		static constexpr u32 MaxQuads = 10000;
		static constexpr u32 MaxVertices = MaxQuads * 4;
		static constexpr u32 MaxIndices = MaxQuads * 6;
		static constexpr u32 MaxTextureSlots = 32;

		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;

		std::shared_ptr<Shader> TextureShader;
		std::shared_ptr<Texture2D> WhiteTexture;

		u32 QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
		u32 TextureSlotIndex = 1; // 0 = white texture

		Vector4<f32> QuadVertexPositions[4] = {};
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init(const std::shared_ptr<Shader>& shader)
	{
		s_Data.QuadVertexArray = std::make_shared<VertexArray>();
		s_Data.QuadVertexBuffer = std::make_shared<VertexBuffer>(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		u32* quadIndices = new uint32_t[s_Data.MaxIndices];

		for (u32 i = 0, offset = 0; i < s_Data.MaxIndices; i += 6, offset += 4) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
		}

		std::shared_ptr<IndexBuffer> quadIndexBuffer = std::make_shared<IndexBuffer>(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

		delete[] quadIndices;

		s_Data.WhiteTexture = std::make_shared<Texture2D>(1, 1);

		u32 whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int samplers[s_Data.MaxTextureSlots];
		for (int i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}
		
		s_Data.TextureShader = shader;
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::DrawQuad(const Matrix4<f32>& transform, const Vector4<f32>& color)
	{

	}

}
