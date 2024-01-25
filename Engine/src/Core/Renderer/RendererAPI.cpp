#include "RendererAPI.hpp"

#include <glad/glad.h>

#include "Core/OpenGL/VertexArray.hpp"
#include "Core/OpenGL/IndexBuffer.hpp"

namespace SW {

	void RendererAPI::Initialize()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void RendererAPI::Shutdown()
	{

	}

	void RendererAPI::DrawIndexed(VertexArray* vertexArray, u32 indexCount)
	{
		u32 count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

		if (indexCount == 0)
			return;

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void RendererAPI::Clear()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}
