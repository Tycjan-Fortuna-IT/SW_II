/**
 * @file VertexArray.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-13
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	class IndexBuffer;
	class VertexBuffer;

	/**
	 * @brief Represents a vertex array object in OpenGL.
	 * 
	 * The VertexArray class encapsulates the functionality of a vertex array object (VAO) in OpenGL.
	 * It provides methods to bind and unbind the VAO, add vertex buffers, and set an index buffer.
	 * The VAO stores the configuration of vertex attributes and their associated vertex buffers.
	 */
	class VertexArray final
	{
	public:
		/**
		 * @brief Constructs a new VertexArray object.
		 */
		VertexArray();

		/**
		 * @brief Destroys the VertexArray object.
		 */
		~VertexArray();

		/**
		 * @brief Binds the VertexArray object.
		 */
		void Bind() const;

		/**
		 * @brief Unbinds the VertexArray object.
		 */
		void Unbind() const;

		/**
		 * @brief Adds a vertex buffer to the VertexArray.
		 * 
		 * @param vertexBuffer The vertex buffer to add.
		 */
		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);

		/**
		 * @brief Sets the index buffer of the VertexArray.
		 * 
		 * @param indexBuffer The index buffer to set.
		 */
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		/**
		 * @brief Retrieves the vertex buffers associated with the VertexArray.
		 * 
		 * @return A vector of shared pointers to the vertex buffers.
		 */
		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }

		/**
		 * @brief Retrieves the index buffer associated with the VertexArray.
		 * 
		 * @return A shared pointer to the index buffer.
		 */
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		u32 m_Handle; // The handle of the vertex array object.

		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers; // The vertex buffers associated with the VAO.

		std::shared_ptr<IndexBuffer> m_IndexBuffer; // The index buffer associated with the VAO.
	};

}
