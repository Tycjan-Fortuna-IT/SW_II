/**
 * @file VertexBuffer.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-13
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "BufferLayout.hpp"

namespace SW
{

	/**
	 * @brief Represents a vertex buffer object in OpenGL.
	 *
	 * The VertexBuffer class encapsulates the functionality of a vertex buffer object (VBO) in OpenGL.
	 * It provides methods to construct the buffer with a specified size or with specified vertices and size,
	 * bind and unbind the VBO, and destroy the VBO.
	 * The VBO stores the vertex data which will be processed by the Vertex Shader to determine the final positions of
	 * the vertices in the screen.
	 */
	class VertexBuffer final
	{
	public:
		/**
		 * @brief Constructs a vertex buffer object with the specified size.
		 * @param size The size of the buffer in bytes.
		 */
		VertexBuffer(u32 size);

		/**
		 * @brief Constructs a vertex buffer object with the specified vertices and size.
		 * @param vertices The array of vertices.
		 * @param size The size of the buffer in bytes.
		 */
		VertexBuffer(float* vertices, u32 size);

		/**
		 * @brief Destroys the vertex buffer object.
		 */
		~VertexBuffer();

		/**
		 * @brief Binds the vertex buffer object.
		 */
		void Bind() const;

		/**
		 * @brief Unbinds the vertex buffer object.
		 */
		void UnBind() const;

		/**
		 * @brief Sets the layout of the vertex buffer.
		 * @param layout The buffer layout to be set.
		 */
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; };

		/**
		 * @brief Sets the data of the vertex buffer.
		 * @param data The data to be set.
		 * @param size The size of the data in bytes.
		 */
		void SetData(const void* data, uint32_t size);

		/**
		 * @brief Retrieves the layout of the vertex buffer.
		 * @return The buffer layout.
		 */
		const BufferLayout& GetLayout() const { return m_Layout; }

	private:
		u32 m_Handle;          /** @brief The handle of the vertex buffer object. */
		BufferLayout m_Layout; /** @brief The layout of the vertex buffer. */
	};

} // namespace SW
