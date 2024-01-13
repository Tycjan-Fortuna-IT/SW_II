#include "Renderer2D.hpp"

#include "Core/OpenGL/Texture2D.hpp"

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

		Texture2D TextureSlots[MaxTextureSlots];
		u32 TextureSlotIndex = 1; // 0 = white texture
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
