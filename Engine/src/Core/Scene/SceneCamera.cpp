#include "SceneCamera.hpp"

#include "Core/Math/Math.hpp"
#include "Core/KeyCode.hpp"
#include "Core/Utils/Input.hpp"

namespace SW {

	SceneCamera::SceneCamera(f32 aspectRatio)
		: m_AspectRatio(aspectRatio)
	{
		m_ViewMatrix = { 1.0f };

		RecalculateProjection();
		RecalculateViewMatrix();
    }

    void SceneCamera::SetViewportSize(u32 width, u32 height)
	{
		ASSERT(width > 0 && height > 0, "Width and height must be positive!");

		m_AspectRatio = (float)width / (float)height;

		RecalculateProjection();
		RecalculateViewMatrix();
    }

	void SceneCamera::OnUpdate(Timestep dt)
	{
		if (Input::IsKeyPressed(KeyCode::A))
			m_CameraPosition.x -= m_CameraMoveSpeed * dt;
		else if (Input::IsKeyPressed(KeyCode::D))
			m_CameraPosition.x += m_CameraMoveSpeed * dt;

		if (Input::IsKeyPressed(KeyCode::W))
			m_CameraPosition.y += m_CameraMoveSpeed * dt;
		else if (Input::IsKeyPressed(KeyCode::S))
			m_CameraPosition.y -= m_CameraMoveSpeed * dt;

		if (Input::IsKeyPressed(KeyCode::Q))
			m_CameraRotation += m_CameraRotationSpeed * dt;
		else if (Input::IsKeyPressed(KeyCode::E))
			m_CameraRotation -= m_CameraRotationSpeed * dt;

		RecalculateViewMatrix();
	}

	void SceneCamera::OnViewportResize(f32 width, f32 height)
	{
		m_AspectRatio = width / height;

		RecalculateProjection();
		RecalculateViewMatrix();
	}

	void SceneCamera::OnMouseScrolled(f32 xOffset, f32 yOffset)
	{
		m_ZoomLevel -= yOffset * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		RecalculateProjection();
		RecalculateViewMatrix();
	}

	void SceneCamera::RecalculateViewMatrix()
	{
		Matrix4<f32> transform = Math::Translate(Matrix4<f32>::Identity(), m_CameraPosition) *
			Math::RotateZ(Matrix4<f32>::Identity(), Math::ToRadians(m_CameraRotation));

		m_ViewMatrix = Math::Inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

	void SceneCamera::RecalculateProjection()
	{
		f32 orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f * m_ZoomLevel;
		f32 orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f * m_ZoomLevel;
		f32 orthoBottom = -m_OrthographicSize * 0.5f * m_ZoomLevel;
		f32 orthoTop = m_OrthographicSize * 0.5f * m_ZoomLevel;

		m_ProjectionMatrix = Math::OrthogonalProjection(
			orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar
		);
	}

}
