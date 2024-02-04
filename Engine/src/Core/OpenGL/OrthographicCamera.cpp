#include "OrthographicCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Core/Math/Math.hpp"

namespace SW {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.f, 1.f);

		m_ViewMatrix = glm::mat4(1.0f);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(f32 left, f32 right, f32 bottom, f32 top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.f, 1.f);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
