#pragma once

namespace SW {

	/**
	 * @brief Represents an index buffer object in OpenGL.
	 * 
	 * The IndexBuffer class encapsulates the functionality of an index buffer object (IBO) in OpenGL.
	 * It provides methods to construct the buffer with a specified size or with specified indices and size,
	 * bind and unbind the IBO, and destroy the IBO.
	 * The IBO stores the indices that OpenGL uses to decide which vertices to draw, which can reduce the amount of data that needs to be sent to the GPU.
	 */
	class IndexBuffer final
	{
	public:
		/**
		 * @brief Constructs an IndexBuffer object.
		 * @param indices Pointer to the array of indices.
		 * @param count Number of indices in the array.
		 */
		IndexBuffer(u32* indices, u32 count);

		/**
		 * @brief Destroys the IndexBuffer object.
		 */
		~IndexBuffer();

		/**
		 * @brief Binds the index buffer for rendering.
		 */
		void Bind() const;

		/**
		 * @brief Unbinds the index buffer.
		 */
		void Unbind() const;

		/**
		 * @brief Gets the number of indices in the buffer.
		 * @return The number of indices.
		 */
		u32 GetCount() const { return m_Count; }

	private:
		u32 m_Handle; /** @brief The handle of the index buffer. */
		u32 m_Count;  /** @brief The number of indices in the buffer. */
	};

}

