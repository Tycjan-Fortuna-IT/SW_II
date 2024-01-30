/**
 * @file Renderer2D.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.
 * @date 2024-01-25
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Math/Vector2.hpp"
#include "Core/Math/Vector3.hpp"
#include "Core/Math/Vector4.hpp"
#include "Core/Math/Matrix4.hpp"
#include "Core/Scene/SceneCamera.hpp"
#include "Core/ECS/Components.hpp"

namespace SW {

	class Shader;
	class VertexArray;

	/**
	 * @brief The Renderer2DStatistics struct contains statistics of the Renderer2D.
	 */
	struct Renderer2DStatistics {
		u32 DrawCalls = 0;		/**< The number of draw calls. */
		u32 QuadCount = 0;		/**< The number of quads. */

		u32 GetTotalVertexCount() const { return QuadCount * 4; }	/**< The total number of vertices. */
		u32 GetTotalIndexCount() const { return QuadCount * 6; }	/**< The total number of indices. */
	};

	/**
	 * @brief The Renderer2D class is responsible for rendering 2D graphics using a specified shader and vertex array.
	 */
	class Renderer2D final
	{
	public:
		/**
		 * @brief Initializes the Renderer2D with the specified shader.
		 * @param shader The shader to be used for rendering.
		 */
		static void Initialize(const std::shared_ptr<Shader>& shader);

		/**
		 * @brief Shuts down the Renderer2D and releases any allocated resources.
		 */
		static void Shutdown();

		/**
		 * @brief Retrieves the statistics of the Renderer2D.
		 * @return The statistics of the Renderer2D.
		 */
		static const Renderer2DStatistics& GetStats();

		/**
		 * @brief Resets the statistics of the Renderer2D.
		 */
		static void ResetStats();

		/**
		 * @brief Begins a new rendering scene with the specified camera.
		 * @param camera The camera to be used for rendering the scene.
		 */
		static void BeginScene(const SceneCamera& camera);

		/**
		 * @brief Begins a new rendering scene with the specified camera and transformation matrix.
		 * @param camera The camera to be used for rendering the scene.
		 * @param transform The transformation matrix of the scene.
		 */
		static void BeginScene(const SceneCamera& camera, const Matrix4<f32>& transform);

		/**
		 * @brief Ends the current rendering scene.
		 */
		static void EndScene();

		/**
		 * @brief Flushes any pending rendering commands to the GPU.
		 */
		static void Flush();

		/**
		 * @brief Flushes any pending rendering commands to the GPU and resets the Renderer2D state.
		 */
		static void FlushAndReset();

		/**
		 * @brief Draws a quad with the specified transformation matrix and sprite component.
		 * @param transform The transformation matrix of the quad.
		 * @param sprite The sprite component of the quad.
		 */
		static void DrawQuad(const Matrix4<f32>& transform, const SpriteComponent& sprite);
	};

}
