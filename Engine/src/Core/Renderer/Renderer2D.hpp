/**
 * @file Renderer2D.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-13
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Math/Vector2.hpp"
#include "Core/Math/Vector3.hpp"
#include "Core/Math/Vector4.hpp"
#include "Core/Math/Matrix4.hpp"
#include "Core/OpenGL/OrthographicCamera.hpp"

namespace SW {

	class Shader;
	class VertexArray;

	struct Renderer2DStatistics {
		u32 DrawCalls = 0;
		u32 QuadCount = 0;

		u32 GetTotalVertexCount() const { return QuadCount * 4; }
		u32 GetTotalIndexCount() const { return QuadCount * 6; }
	};

	class Renderer2D final
	{
	public:
		static void Init(const std::shared_ptr<Shader>& shader);
		static void Shutdown();

		static const Renderer2DStatistics& GetStats();
		static void ResetStats();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();
		static void FlushAndReset();

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, u32 indexCount);

		static void DrawQuad(const Matrix4<f32>& transform, const Vector4<f32>& color);
	};

}
