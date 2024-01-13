#include "VertexArray.hpp"

#include <glad/glad.h>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace SW {

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_Handle);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_Handle);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_Handle);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_Handle);
		vertexBuffer->Bind();

		ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		u32 index = 0;
		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);

			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
				GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.Offset));

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_Handle);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
