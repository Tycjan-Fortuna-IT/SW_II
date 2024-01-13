#include "SceneCamera.hpp"

#include "Core/Math/Math.hpp"

namespace SW {

    SceneCamera::SceneCamera()
	{
		RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(u32 width, u32 height)
	{
		ASSERT(width > 0 && height > 0, "Width and height must be positive!");

		m_AspectRatio = (float)width / (float)height;

		RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection()
	{
		f32 orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		f32 orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		f32 orthoBottom = -m_OrthographicSize * 0.5f;
		f32 orthoTop = m_OrthographicSize * 0.5f;

		m_Projection = Math::OrthogonalProjection(
			orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar
		);
    }

}
