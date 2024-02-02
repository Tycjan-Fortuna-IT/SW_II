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
			switch (element.Type) {
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						 GL_FALSE,
						layout.GetStride(),
						(const void*)element.Offset);
					index++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribIPointer(index,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						layout.GetStride(),
						(const void*)element.Offset);
					index++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++) {
						glEnableVertexAttribArray(index);
						glVertexAttribPointer(index,
							count,
							ShaderDataTypeToOpenGLBaseType(element.Type),
							 GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(index, 1);
						index++;
					}
					break;
				}
				default:
					ASSERT(false, "Unknown ShaderDataType!");
			}
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
