/**
 * @file Renderer2D.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.2.3
 * @date 2024-04-12
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/ECS/Components.hpp"
#include "Core/Scene/SceneCamera.hpp"

namespace SW
{

	class Shader;
	class VertexArray;
	class EditorCamera;
	class Font;

	/**
	 * @brief The Renderer2DStatistics struct contains statistics of the Renderer2D.
	 */
	struct Renderer2DStatistics
	{
		u32 DrawCalls = 0; /**< The number of draw calls. */
		u32 QuadCount = 0; /**< The number of quads. */

		u32 GetTotalVertexCount() const { return QuadCount * 4; } /**< The total number of vertices. */
		u32 GetTotalIndexCount() const { return QuadCount * 6; }  /**< The total number of indices. */
	};

	/**
	 * @brief The Renderer2D class is responsible for rendering 2D graphics using a specified shader and vertex array.
	 */
	class Renderer2D final
	{
	public:
		static Texture2D* WhiteTexture;
		static Texture2D* BlackTexture;

	public:
		/**
		 * @brief Initializes the Renderer2D with the specified shader.
		 */
		static void Initialize();

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
		 * @brief Draws a quad with the specified transformation matrix and animated sprite component.
		 * @param transform The transformation matrix of the quad.
		 * @param asc The animated sprite component of the quad.
		 * @param time The time elapsed since the last frame. Cumulative time is used to determine the current frame.
		 * @param entityID The ID of the entity.
		 */
		static void DrawQuad(const glm::mat4& transform, AnimatedSpriteComponent& asc, f32 time, int entityID = -1);

		/**
		 * @brief Draws a quad with the placeholder missing texture and the specified transformation matrix.
		 * @param transform The transformation matrix of the quad.
		 * @param entityID The ID of the entity.
		 */
		static void DrawMissingTextureQuad(const glm::mat4& transform, int entityID = -1);

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
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
		                     int entityID = -1);

		/**
		 * Draws a circle using the specified transformation matrix and circle component.
		 *
		 * @param transform The transformation matrix to apply to the circle.
		 * @param cc The CircleComponent object that contains the circle's properties.
		 * @param entityID (optional) The ID of the entity associated with the circle. Default value is -1.
		 */
		static void DrawCircle(const glm::mat4& transform, const CircleComponent& cc, int entityID = -1);

		/**
		 * @brief Draws a circle shape on the screen.
		 *
		 * @param transform The transformation matrix to apply to the circle.
		 * @param color The color of the circle.
		 * @param thickness The thickness of the circle's outline (default is 1.0f).
		 * @param fade The rate at which the circle's color fades towards transparency (default is 0.005f).
		 * @param entityID The ID of the entity associated with the circle (default is -1).
		 */
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, f32 thickness = 1.0f,
		                       f32 fade = 0.005f, int entityID = -1);

		/**
		 * @brief Draws a rectangle with the specified transformation matrix and color.
		 * @param transform The transformation matrix of the rectangle.
		 * @param color The color of the rectangle.
		 */
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color);

		/**
		 * Draws a string using the specified transformation matrix and text component.
		 *
		 * @param transform The transformation matrix to apply to the string.
		 * @param text The text component containing the string to draw.
		 * @param entityID The ID of the entity associated with the string (optional, default is -1).
		 */
		static void DrawString(const glm::mat4& transform, const TextComponent& text, int entityID = -1);

		static void DrawString(const std::string& string, Font** font, const glm::mat4& transform,
		                       const glm::vec4& color, f32 kerning = 0.0f, f32 lineSpacing = 0.0f, int entityID = -1);
	};

} // namespace SW
