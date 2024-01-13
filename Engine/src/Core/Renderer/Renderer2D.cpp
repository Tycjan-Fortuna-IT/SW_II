#include "Renderer2D.hpp"

#include "Core/OpenGL/Texture2D.hpp"
#include "Core/OpenGL/VertexArray.hpp"
#include "Core/OpenGL/VertexBuffer.hpp"
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

		Texture2D TextureSlots[MaxTextureSlots];
		u32 TextureSlotIndex = 1; // 0 = white texture

		Vector4<f32> QuadVertexPositions[4] = {};
	};

	void Renderer2D::Init()
	{

	}

	void Renderer2D::Shutdown()
	{

	}

	void Renderer2D::DrawQuad(const Matrix4<f32>& transform, const Vector4<f32>& color)
	{

	}

}
