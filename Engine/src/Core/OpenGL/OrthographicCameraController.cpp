#include "OrthographicCameraController.hpp"

#include "Core/KeyCode.hpp"
#include "Core/Utils/Input.hpp"

namespace SW {

	OrthographicCameraController::OrthographicCameraController(f32 aspectRatio)
		: m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel) {}

	void OrthographicCameraController::OnUpdate(Timestep dt)
	{
		if (Input::IsKeyDown(KeyCode::A))
			m_CameraPosition.x -= m_CameraMoveSpeed * dt;
		else if (Input::IsKeyDown(KeyCode::D))
			m_CameraPosition.x += m_CameraMoveSpeed * dt;

		if (Input::IsKeyDown(KeyCode::W))
			m_CameraPosition.y += m_CameraMoveSpeed * dt;
		else if (Input::IsKeyDown(KeyCode::S))
			m_CameraPosition.y -= m_CameraMoveSpeed * dt;

		if (Input::IsKeyDown(KeyCode::Q))
			m_CameraRotation += m_CameraRotationSpeed * dt;
		else if (Input::IsKeyDown(KeyCode::E))
			m_CameraRotation -= m_CameraRotationSpeed * dt;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		m_CameraMoveSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnResize(f32 width, f32 height)
	{
		m_AspectRatio = width / height;

		CalculateView();
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(f32 xOffset, f32 yOffset)
	{
		m_ZoomLevel -= yOffset * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		APP_WARN("Zoom {}", m_ZoomLevel);

		CalculateView();

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(i32 width, i32 height)
	{
		m_AspectRatio = static_cast<f32>(width) / static_cast<f32>(height);

		APP_WARN("width {} height {}", width, height);

		CalculateView();

		return false;
	}

}
