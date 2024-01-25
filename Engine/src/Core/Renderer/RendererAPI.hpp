/**
 * @file RendererAPI.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-25
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	class VertexArray;

	/**
	 * @brief Class responsible for exposing useful OpenGL related methods.
	 */
	class RendererAPI final
	{
	public:
		/**
		 * @briefief Initializes OpenGL.
		 * @warning Must be called before any OpenGL operation is performed.
		 */
		static void Initialize();

		/**
		 * @brief Shuts down OpenGL.
		 * @warning Must be called before application exit.
		 */
		static void Shutdown();

		/**
		 * @brief Draws the specified vertex array using the specified number of indices.
		 * @param vertexArray The vertex array to be drawn.
		 * @param indexCount The number of indices to be drawn.
		 */
		static void DrawIndexed(VertexArray* vertexArray, u32 indexCount);

		static void Clear();
	};

}

