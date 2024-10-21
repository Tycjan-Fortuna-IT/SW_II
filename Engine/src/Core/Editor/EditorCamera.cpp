#include "EditorCamera.hpp"

namespace SW
{

	EditorCamera::EditorCamera(f32 fov, f32 aspectRatio, f32 nearClip, f32 farClip)
	    : Camera(glm::perspective(fov, aspectRatio, nearClip, farClip)), m_Fov(fov), m_AspectRatio(aspectRatio),
	      m_NearClip(nearClip), m_FarClip(farClip)
	{
	}

	void EditorCamera::OnUpdate(Timestep timestep)
	{
		const f32 cosYaw   = glm::cos(m_Yaw);
		const f32 sinYaw   = glm::sin(m_Yaw);
		const f32 cosPitch = glm::cos(m_Pitch);
		const f32 sinPitch = glm::sin(m_Pitch);

		m_Forward.x = cosYaw * cosPitch;
		m_Forward.y = sinPitch;
		m_Forward.z = sinYaw * cosPitch;

		m_Forward = glm::normalize(m_Forward);
		m_Right   = glm::normalize(glm::cross(m_Forward, {0, 1, 0}));
		m_Up      = glm::normalize(glm::cross(m_Right, m_Forward));

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}

	void EditorCamera::SetViewportSize(f32 width, f32 height)
	{
		m_AspectRatio = width / height;
		m_Projection  = glm::perspective(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
	}

} // namespace SW
