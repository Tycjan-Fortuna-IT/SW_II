/**
 * @file Renderer2D.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.2.1
 * @date 2024-02-03
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
	class EditorCamera;

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
		 * 
		 * @param spriteShader The shader to be used for rendering sprites.
		 * @param lineShader The shader to be used for rendering lines.
		 * @param circleShader The shader to be used for rendering circles and rings.
		 */
		static void Initialize(Shader* spriteShader, Shader* lineShader, Shader* circleShader);

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
		static void BeginScene(EditorCamera* camera);

		/**
		 * @brief Begins a new rendering scene with the specified camera and transformation matrix.
		 * @param camera The camera to be used for rendering the scene.
		 * @param transform The transformation matrix of the scene.
		 */
		static void BeginScene(const SceneCamera& camera, const glm::mat4& transform);

		/**
		 * @brief Ends the current rendering scene.
		 */
		static void EndScene();

		/**
		 * @brief Starts a new batch of rendering commands.
		 */
		static void StartBatch();

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
		 * @param entityID The ID of the entity.
		 */
		static void DrawQuad(const glm::mat4& transform, const SpriteComponent& sprite, int entityID = -1);

		/**
		 * @brief Draws a line with the specified start and end points and color.
		 * @param p0 The start point of the line.
		 * @param p1 The end point of the line.
		 * @param color The color of the line.
		 */
		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID = -1);

		/**
		 * @brief Draws a rectangle with the specified position, size, and color.
		 * @param position The position of the rectangle.
		 * @param size The size of the rectangle.
		 * @param color The color of the rectangle.
		 */
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1);

		static void DrawCircle(const glm::mat4& transform, const CircleComponent& cc, int entityID = -1);

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, f32 thickness = 1.0f, f32 fade = 0.005f, int entityID = -1);

		/**
		 * @brief Draws a rectangle with the specified transformation matrix and color.
		 * @param transform The transformation matrix of the rectangle.
		 * @param color The color of the rectangle.
		 */
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color);
	};

}
