#include "Renderer2D.hpp"

#include "Asset/AssetManager.hpp"
#include "Asset/Font.hpp"
#include "Asset/Sprite.hpp"
#include "Core/Editor/EditorCamera.hpp"
#include "Core/OpenGL/IndexBuffer.hpp"
#include "Core/OpenGL/Shader.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Core/OpenGL/VertexArray.hpp"
#include "Core/OpenGL/VertexBuffer.hpp"
#include "GUI/Editor/EditorResources.hpp"
#include "RendererAPI.hpp"

namespace SW
{

	struct QuadVertex final
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;

		f32 TexIndex;
		f32 TilingFactor;

		int EntityID;
	};

	struct LineVertex final
	{
		glm::vec3 Position;
		glm::vec4 Color;

		int EntityID;
	};

	struct CircleVertex final
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		f32 Thickness;
		f32 Fade;

		int EntityID;
	};

	struct TextVertex final
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;

		f32 TexIndex;

		int EntityID;
	};

	struct Renderer2DData final
	{
		Renderer2DData() = default;

		static constexpr u32 MaxQuads        = 1000;
		static constexpr u32 MaxVertices     = MaxQuads * 4;
		static constexpr u32 MaxIndices      = MaxQuads * 6;
		static constexpr u32 MaxTextureSlots = 32;

		VertexArray* QuadVertexArray = nullptr;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;

		VertexArray* LineVertexArray = nullptr;
		std::shared_ptr<VertexBuffer> LineVertexBuffer;

		VertexArray* CircleVertexArray = nullptr;
		std::shared_ptr<VertexBuffer> CircleVertexBuffer;

		VertexArray* TextVertexArray = nullptr;
		std::shared_ptr<VertexBuffer> TextVertexBuffer;

		u32 QuadIndexCount               = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr  = nullptr;

		u32 LineVertexCount              = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr  = nullptr;

		u32 CircleIndexCount                 = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr  = nullptr;

		u32 TextIndexCount               = 0;
		TextVertex* TextVertexBufferBase = nullptr;
		TextVertex* TextVertexBufferPtr  = nullptr;

		std::array<Texture2D*, MaxTextureSlots> TextureSlots;
		std::array<Texture2D*, MaxTextureSlots> FontTextureSlots;

		u32 TextureSlotIndex     = 1; // 0 = white texture
		u32 FontTextureSlotIndex = 0;

		f32 LineWidth = 2.0f;

		glm::vec4 QuadVertexPositions[4] = {};

		Shader* SpriteShader = nullptr;
		Shader* LineShader   = nullptr;
		Shader* CircleShader = nullptr;
		Shader* TextShader   = nullptr;

		Renderer2DStatistics Stats;
	};

	static Renderer2DData s_Data;

	Texture2D* Renderer2D::WhiteTexture = nullptr;
	Texture2D* Renderer2D::BlackTexture = nullptr;

	void Renderer2D::Initialize()
	{
		WhiteTexture         = new Texture2D(1, 1);
		u32 whiteTextureData = 0xffffffff;
		WhiteTexture->SetData(&whiteTextureData, sizeof(u32));

		BlackTexture         = new Texture2D(1, 1);
		u32 blackTextureData = 0x000000ff;
		BlackTexture->SetData(&blackTextureData, sizeof(u32));

		s_Data.SpriteShader =
		    new Shader("assets/shaders/Builtin.2D.Sprite.vert.glsl", "assets/shaders/Builtin.2D.Sprite.frag.glsl");
		s_Data.LineShader =
		    new Shader("assets/shaders/Builtin.2D.Line.vert.glsl", "assets/shaders/Builtin.2D.Line.frag.glsl");
		s_Data.CircleShader =
		    new Shader("assets/shaders/Builtin.2D.Circle.vert.glsl", "assets/shaders/Builtin.2D.Circle.frag.glsl");
		s_Data.TextShader =
		    new Shader("assets/shaders/Builtin.2D.Text.vert.glsl", "assets/shaders/Builtin.2D.Text.frag.glsl");

		s_Data.QuadVertexArray = new VertexArray();
		s_Data.QuadVertexBuffer =
		    std::make_shared<VertexBuffer>(static_cast<u32>(s_Data.MaxVertices * sizeof(QuadVertex)));

		s_Data.QuadVertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"},
		                                    {ShaderDataType::Float4, "a_Color"},
		                                    {ShaderDataType::Float2, "a_TexCoord"},
		                                    {ShaderDataType::Float, "a_TexIndex"},
		                                    {ShaderDataType::Float, "a_TilingFactor"},
		                                    {ShaderDataType::Int, "a_EntityID"}});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		u32* quadIndices = new u32[s_Data.MaxIndices];

		for (u32 i = 0, offset = 0; i < s_Data.MaxIndices; i += 6, offset += 4)
		{
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

		// Lines
		{
			s_Data.LineVertexArray = new VertexArray();
			s_Data.LineVertexBuffer =
			    std::make_shared<VertexBuffer>(static_cast<u32>(s_Data.MaxVertices * sizeof(LineVertex)));
			s_Data.LineVertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"},
			                                    {ShaderDataType::Float4, "a_Color"},
			                                    {ShaderDataType::Int, "a_EntityID"}});
			s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
			s_Data.LineVertexBufferBase = new LineVertex[Renderer2DData::MaxVertices];
		}

		// Circles
		{
			s_Data.CircleVertexArray = new VertexArray();
			s_Data.CircleVertexBuffer =
			    std::make_shared<VertexBuffer>(static_cast<u32>(s_Data.MaxVertices * sizeof(CircleVertex)));
			s_Data.CircleVertexBuffer->SetLayout({{ShaderDataType::Float3, "a_WorldPosition"},
			                                      {ShaderDataType::Float3, "a_LocalPosition"},
			                                      {ShaderDataType::Float4, "a_Color"},
			                                      {ShaderDataType::Float, "a_Thickness"},
			                                      {ShaderDataType::Float, "a_Fade"},
			                                      {ShaderDataType::Int, "a_EntityID"}});
			s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
			s_Data.CircleVertexArray->SetIndexBuffer(quadIndexBuffer); // Use quad IB
			s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];
		}

		// Text
		{
			s_Data.TextVertexArray = new VertexArray();
			s_Data.TextVertexBuffer =
			    std::make_shared<VertexBuffer>(static_cast<u32>(s_Data.MaxVertices * sizeof(TextVertex)));
			s_Data.TextVertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"},
			                                    {ShaderDataType::Float4, "a_Color"},
			                                    {ShaderDataType::Float2, "a_TexCoord"},
			                                    {ShaderDataType::Float, "a_TexIndex"},
			                                    {ShaderDataType::Int, "a_EntityID"}});
			s_Data.TextVertexArray->AddVertexBuffer(s_Data.TextVertexBuffer);
			s_Data.TextVertexArray->SetIndexBuffer(quadIndexBuffer);
			s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];
		}

		int samplers[s_Data.MaxTextureSlots];
		for (int i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_Data.SpriteShader->Bind();
		s_Data.SpriteShader->UploadUniformIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextShader->Bind();
		s_Data.TextShader->UploadUniformIntArray("u_FontAtlasTextures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = WhiteTexture;

		s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
		s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
	}

	void Renderer2D::Shutdown()
	{
		delete WhiteTexture;
		delete BlackTexture;

		delete[] s_Data.QuadVertexBufferBase;
		delete s_Data.QuadVertexArray;

		delete s_Data.SpriteShader;
		delete s_Data.LineShader;
		delete s_Data.CircleShader;
		delete s_Data.TextShader;
	}

	const Renderer2DStatistics& Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Renderer2DStatistics));
	}

	void Renderer2D::BeginScene(EditorCamera* camera)
	{
		const glm::mat4 viewProjection = camera->GetViewProjectionMatrix();

		s_Data.SpriteShader->Bind();
		s_Data.SpriteShader->UploadUniformMat4("u_ViewProjection", viewProjection);

		s_Data.LineShader->Bind();
		s_Data.LineShader->UploadUniformMat4("u_ViewProjection", viewProjection);

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->UploadUniformMat4("u_ViewProjection", viewProjection);

		s_Data.TextShader->Bind();
		s_Data.TextShader->UploadUniformMat4("u_ViewProjection", viewProjection);

		StartBatch();
	}

	void Renderer2D::BeginScene(const SceneCamera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProjection = camera.GetProjectionMatrix() * glm::inverse(transform);

		s_Data.SpriteShader->Bind();
		s_Data.SpriteShader->UploadUniformMat4("u_ViewProjection", viewProjection);

		s_Data.LineShader->Bind();
		s_Data.LineShader->UploadUniformMat4("u_ViewProjection", viewProjection);

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->UploadUniformMat4("u_ViewProjection", viewProjection);

		s_Data.TextShader->Bind();
		s_Data.TextShader->UploadUniformMat4("u_ViewProjection", viewProjection);

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount      = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.LineVertexCount     = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.CircleIndexCount      = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.TextIndexCount      = 0;
		s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;

		s_Data.TextureSlotIndex     = 1;
		s_Data.FontTextureSlotIndex = 0;
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount)
		{
			const u32 dataSize = static_cast<u32>(reinterpret_cast<u8*>(s_Data.QuadVertexBufferPtr) -
			                                      reinterpret_cast<u8*>(s_Data.QuadVertexBufferBase));

			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			for (u32 i = 0; i < s_Data.TextureSlotIndex; i++) s_Data.TextureSlots[i]->Bind(i);

			s_Data.SpriteShader->Bind();
			RendererAPI::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.LineVertexCount)
		{
			u32 dataSize = (u32)((u8*)s_Data.LineVertexBufferPtr - (u8*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Bind();
			RendererAPI::SetLineWidth(s_Data.LineWidth);
			RendererAPI::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.CircleIndexCount)
		{
			u32 dataSize = (u32)((u8*)s_Data.CircleVertexBufferPtr - (u8*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			s_Data.CircleShader->Bind();
			RendererAPI::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.TextIndexCount)
		{
			u32 dataSize = (u32)((u8*)s_Data.TextVertexBufferPtr - (u8*)s_Data.TextVertexBufferBase);
			s_Data.TextVertexBuffer->SetData(s_Data.TextVertexBufferBase, dataSize);

			for (u32 i = 0; i < s_Data.FontTextureSlotIndex; i++) s_Data.FontTextureSlots[i]->Bind(i);

			s_Data.TextShader->Bind();
			RendererAPI::DrawIndexed(s_Data.TextVertexArray, s_Data.TextIndexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const SpriteComponent& sprite, int entityID)
	{
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		f32 textureIndex = 0.f; // White Texture

		glm::vec2 texCoords[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		if (sprite.Handle)
		{
			Sprite** spriteAsset = AssetManager::GetAssetRaw<Sprite>(sprite.Handle);

			Texture2D* texture = nullptr;
			if (spriteAsset)
				texture = (*spriteAsset)->GetTexture();
			else
				texture = EditorResources::MissingAssetIcon;

			for (u32 i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i] == *texture)
				{
					textureIndex = static_cast<f32>(i);
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				textureIndex                                 = (f32)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}

			texCoords[0] = (*spriteAsset)->TexCordUpLeft;
			texCoords[1] = (*spriteAsset)->TexCordUpRight;
			texCoords[2] = (*spriteAsset)->TexCordRightDown;
			texCoords[3] = (*spriteAsset)->TexCordLeftDown;
		}

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position     = glm::vec3(transform * s_Data.QuadVertexPositions[i]);
			s_Data.QuadVertexBufferPtr->Color        = sprite.Color;
			s_Data.QuadVertexBufferPtr->TexCoord     = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex     = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = sprite.TilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID     = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, AnimatedSpriteComponent& asc, f32 time, int entityID /*= -1*/)
	{
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		f32 textureIndex = 0.f; // White Texture

		glm::vec2 texCoords[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		u64 framesCount = (*asc.CurrentAnimation)->Sprites.size();

		asc.CurrentFrame = (int)(time * (*asc.CurrentAnimation)->Speed) % framesCount;

		if (asc.CurrentFrame >= framesCount)
		{
			asc.CurrentFrame = 0;
		}

		Sprite** sprite    = (*asc.CurrentAnimation)->Sprites[asc.CurrentFrame];
		Texture2D* texture = (*sprite)->GetTexture();

		for (u32 i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = static_cast<f32>(i);
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex                                 = (f32)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		texCoords[0] =
		    (*asc.CurrentAnimation)->ReverseAlongX
		        ? ((*asc.CurrentAnimation)->ReverseAlongY ? (*sprite)->TexCordRightDown : (*sprite)->TexCordUpRight)
		        : ((*asc.CurrentAnimation)->ReverseAlongY ? (*sprite)->TexCordLeftDown : (*sprite)->TexCordUpLeft);

		texCoords[1] =
		    (*asc.CurrentAnimation)->ReverseAlongX
		        ? ((*asc.CurrentAnimation)->ReverseAlongY ? (*sprite)->TexCordLeftDown : (*sprite)->TexCordUpLeft)
		        : ((*asc.CurrentAnimation)->ReverseAlongY ? (*sprite)->TexCordRightDown : (*sprite)->TexCordUpRight);

		texCoords[2] =
		    (*asc.CurrentAnimation)->ReverseAlongX
		        ? ((*asc.CurrentAnimation)->ReverseAlongY ? (*sprite)->TexCordUpLeft : (*sprite)->TexCordLeftDown)
		        : ((*asc.CurrentAnimation)->ReverseAlongY ? (*sprite)->TexCordUpRight : (*sprite)->TexCordRightDown);

		texCoords[3] =
		    (*asc.CurrentAnimation)->ReverseAlongX
		        ? ((*asc.CurrentAnimation)->ReverseAlongY ? (*sprite)->TexCordUpRight : (*sprite)->TexCordRightDown)
		        : ((*asc.CurrentAnimation)->ReverseAlongY ? (*sprite)->TexCordUpLeft : (*sprite)->TexCordLeftDown);

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position     = glm::vec3(transform * s_Data.QuadVertexPositions[i]);
			s_Data.QuadVertexBufferPtr->Color        = glm::vec4(1.f);
			s_Data.QuadVertexBufferPtr->TexCoord     = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex     = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.f;
			s_Data.QuadVertexBufferPtr->EntityID     = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawMissingTextureQuad(const glm::mat4& transform, int entityID /*= -1*/)
	{
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		f32 textureIndex = 0.f; // White Texture

		glm::vec2 texCoords[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

		Texture2D* texture = EditorResources::MissingAssetIcon;

		for (u32 i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = static_cast<f32>(i);
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex                                 = (f32)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position     = glm::vec3(transform * s_Data.QuadVertexPositions[i]);
			s_Data.QuadVertexBufferPtr->Color        = glm::vec4(1.f);
			s_Data.QuadVertexBufferPtr->TexCoord     = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex     = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1;
			s_Data.QuadVertexBufferPtr->EntityID     = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color    = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color    = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color)
	{
		glm::vec3 lineVertices[4];
		for (u64 i = 0; i < 4; i++) lineVertices[i] = glm::vec3(transform * s_Data.QuadVertexPositions[i]);

		DrawLine(lineVertices[0], lineVertices[1], color);
		DrawLine(lineVertices[1], lineVertices[2], color);
		DrawLine(lineVertices[2], lineVertices[3], color);
		DrawLine(lineVertices[3], lineVertices[0], color);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const CircleComponent& cc, int entityID /*= -1*/)
	{
		DrawCircle(transform, cc.Color, cc.Thickness, cc.Fade, entityID);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, f32 thickness /*= 1.0f*/,
	                            f32 fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		for (u64 i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color         = color;
			s_Data.CircleVertexBufferPtr->Thickness     = thickness;
			s_Data.CircleVertexBufferPtr->Fade          = fade;
			s_Data.CircleVertexBufferPtr->EntityID      = entityID;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawString(const glm::mat4& transform, const TextComponent& text, int entityID)
	{
		Font** fontAsset = AssetManager::GetAssetRaw<Font>(text.Handle);

		if (!fontAsset)
		{
			DrawMissingTextureQuad(transform, entityID);

			return;
		}

		DrawString(text.TextString, fontAsset, transform, text.Color, text.Kerning, text.LineSpacing, entityID);
	}

	void Renderer2D::DrawString(const std::string& string, Font** font, const glm::mat4& transform,
	                            const glm::vec4& color, f32 kerning /*= 0.0f*/, f32 lineSpacing /*= 0.0f*/,
	                            int entityID /*= -1*/)
	{
		f32 textureIndex = 0.f;

		Texture2D* atlasTexture                      = (*font)->GetAtlasTexture();
		const msdf_atlas::FontGeometry& fontGeometry = (*font)->GetMSDFData().FontGeometry;
		const msdfgen::FontMetrics& fontMetrics      = fontGeometry.getMetrics();

		for (u32 i = 1; i < s_Data.FontTextureSlotIndex; i++)
		{
			if (*s_Data.FontTextureSlots[i] == *atlasTexture)
			{
				textureIndex = static_cast<f32>(i);
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex                                         = (f32)s_Data.FontTextureSlotIndex;
			s_Data.FontTextureSlots[s_Data.FontTextureSlotIndex] = atlasTexture;
			s_Data.FontTextureSlotIndex++;
		}

		f64 x       = 0.0;
		f64 fsScale = 1.0 / (fontMetrics.ascenderY - fontMetrics.descenderY);
		f64 y       = 0.0;

		const f32 spaceGlyphAdvance = (f32)fontGeometry.getGlyph(' ')->getAdvance();

		for (size_t i = 0; i < string.size(); i++)
		{
			msdf_atlas::unicode_t character = string[i];
			if (character == '\r')
				continue;

			if (character == '\n')
			{
				x = 0;
				y -= fsScale * fontMetrics.lineHeight + lineSpacing;
				continue;
			}

			if (character == ' ')
			{
				f32 advance = spaceGlyphAdvance;

				if (i < string.size() - 1)
				{
					char nextCharacter = string[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = (f32)dAdvance;
				}

				x += fsScale * advance + kerning;

				continue;
			}

			if (character == '\t')
			{
				x += 4.0f * (fsScale * spaceGlyphAdvance + kerning);

				continue;
			}

			const msdf_atlas::GlyphGeometry* glyph = fontGeometry.getGlyph(character);

			if (!glyph)
				glyph = fontGeometry.getGlyph('?');

			if (!glyph)
				return;

			f64 al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((f32)al, (f32)ab);
			glm::vec2 texCoordMax((f32)ar, (f32)at);

			f64 pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((f32)pl, (f32)pb);
			glm::vec2 quadMax((f32)pr, (f32)pt);

			quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			f32 texelWidth  = 1.0f / (f32)atlasTexture->GetWidth();
			f32 texelHeight = 1.0f / (f32)atlasTexture->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color    = color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMin;
			s_Data.TextVertexBufferPtr->TexIndex = textureIndex;
			s_Data.TextVertexBufferPtr->EntityID = entityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color    = color;
			s_Data.TextVertexBufferPtr->TexCoord = {texCoordMin.x, texCoordMax.y};
			s_Data.TextVertexBufferPtr->TexIndex = textureIndex;
			s_Data.TextVertexBufferPtr->EntityID = entityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color    = color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMax;
			s_Data.TextVertexBufferPtr->TexIndex = textureIndex;
			s_Data.TextVertexBufferPtr->EntityID = entityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color    = color;
			s_Data.TextVertexBufferPtr->TexCoord = {texCoordMax.x, texCoordMin.y};
			s_Data.TextVertexBufferPtr->TexIndex = textureIndex;
			s_Data.TextVertexBufferPtr->EntityID = entityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextIndexCount += 6;
			s_Data.Stats.QuadCount++;

			if (i < string.size() - 1)
			{
				f64 advance                         = glyph->getAdvance();
				msdf_atlas::unicode_t nextCharacter = string[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);

				x += fsScale * advance + kerning;
			}
		}
	}

} // namespace SW
