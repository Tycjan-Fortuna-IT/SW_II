#include "VertexBuffer.hpp"

namespace SW {

	VertexBuffer::VertexBuffer(u32 size)
	{
		glCreateBuffers(1, &m_Handle);

		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);

		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::VertexBuffer(float* vertices, u32 size)
	{
		glCreateBuffers(1, &m_Handle);

		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);

		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_Handle);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
	}

	void VertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

}
