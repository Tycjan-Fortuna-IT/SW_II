#include "IndexBuffer.hpp"

#include <glad/glad.h>

namespace SW
{

	IndexBuffer::IndexBuffer(u32* indices, u32 count) : m_Count(count)
	{
		glGenBuffers(1, &m_Handle);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_Handle);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} // namespace SW
