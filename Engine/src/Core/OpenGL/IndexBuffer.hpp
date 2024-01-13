#pragma once

namespace SW {

	/**
	 * @brief Represents an index buffer used for rendering.
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

