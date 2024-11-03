#include "SceneCamera.hpp"

namespace SW
{

	SceneCamera::SceneCamera(f32 aspectRatio) : m_AspectRatio(aspectRatio)
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(f32 verticalFov, f32 nearClip, f32 farClip)
	{
		PROFILE_SCOPE();

		m_ProjectionType = ProjectionType::Perspective;

		m_PerspectiveFOV  = verticalFov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar  = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(f32 size, f32 nearClip, f32 farClip)
	{
		PROFILE_SCOPE();

		m_ProjectionType = ProjectionType::Orthographic;

		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar  = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(u32 width, u32 height)
	{
		ASSERT(width > 0 && height > 0, "Width and height must be positive!");

		m_AspectRatio = (f32)width / (f32)height;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		PROFILE_SCOPE();

		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			const f32 orthoLeft   = -m_OrthographicSize * m_AspectRatio * 0.5f;
			const f32 orthoRight  = m_OrthographicSize * m_AspectRatio * 0.5f;
			const f32 orthoBottom = -m_OrthographicSize * 0.5f;
			const f32 orthoTop    = m_OrthographicSize * 0.5f;

			m_Projection =
			    glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
	}

} // namespace SW
