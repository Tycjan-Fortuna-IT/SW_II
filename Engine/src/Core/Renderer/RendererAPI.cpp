#include "RendererAPI.hpp"

#include <glad/glad.h>

#include "Core/OpenGL/VertexArray.hpp"
#include "Core/OpenGL/IndexBuffer.hpp"

namespace SW {

	void OpenGLMessageCallback(
		u32 source, UNUSED u32 type, UNUSED u32 id, u32 severity, UNUSED i32 length, const char* message, UNUSED const void* userParam
	) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:         SYSTEM_ERROR(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       SYSTEM_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          SYSTEM_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: SYSTEM_TRACE(message); return;
			default:
				ASSERT(false, "Unknown severity level!");
		}
	}

	void RendererAPI::Initialize()
	{
#ifdef SW_DEBUG_BUILD
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void RendererAPI::Shutdown()
	{

	}

	void RendererAPI::DrawIndexed(VertexArray* vertexArray, u32 indexCount)
	{
		vertexArray->Bind();

		u32 count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

		if (indexCount == 0)
			return;

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void RendererAPI::DrawLines(VertexArray* vertexArray, u32 vertexCount)
	{
		vertexArray->Bind();

		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void RendererAPI::SetLineWidth(f32 width)
	{
		glLineWidth(width);
	}

	void RendererAPI::Clear()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}
