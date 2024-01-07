#include "OrthographicCamera.hpp"

#include "Core/Math/Math.hpp"

namespace SW {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) {
		m_ProjectionMatrix = Math::OrthogonalProjection<f32>(left, right, bottom, top, -1.f, 1.f);

		m_ViewMatrix = { 1.0f };

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(f32 left, f32 right, f32 bottom, f32 top) {
		m_ProjectionMatrix = Math::OrthogonalProjection<f32>(left, right, bottom, top, -1.f, 1.f);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix() {
		Matrix4<f32> transform = { 1.0f };

		transform.Translate(m_Position);
		transform.RotateZ(Math::ToRadians(m_Rotation));

		m_ViewMatrix = Math::Inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
