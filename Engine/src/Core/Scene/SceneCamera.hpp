/**
 * @file SceneCamera.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-13
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Renderer/Camera.hpp"
#include "Core/Timestep.hpp"

namespace SW {

	class SceneCamera final : public Camera
	{
	public:
		SceneCamera(f32 aspectRatio);
		~SceneCamera() override = default;

		void SetViewportSize(u32 width, u32 height);

		void OnUpdate(Timestep dt);

		void OnViewportResize(f32 width, f32 height);
		void OnMouseScrolled(f32 xOffset, f32 yOffset);

		f32 GetOrthographicSize() const { return m_OrthographicSize; }
		f32 GetOrthographicNearClip() const { return m_OrthographicNear; }
		f32 GetOrthographicFarClip() const { return m_OrthographicFar; }

		void SetOrthographicSize(f32 size) { m_OrthographicSize = size; RecalculateProjection(); }
		void SetOrthographicNearClip(f32 nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		void SetOrthographicFarClip(f32 farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		const Matrix4<f32>& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const Matrix4<f32>& GetViewMatrix() const { return m_ViewMatrix; }
		const Matrix4<f32>& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		f32 m_OrthographicSize = 10.0f;
		f32 m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		f32 m_AspectRatio = 0.0f;
		f32 m_ZoomLevel = 1.0f;

		Vector3<f32> m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		f32 m_CameraRotation = 0.0f;

		f32 m_CameraMoveSpeed = 20.0f;
		f32 m_CameraRotationSpeed = 30.0f;

		Matrix4<f32> m_ProjectionMatrix;
		Matrix4<f32> m_ViewMatrix;
		Matrix4<f32> m_ViewProjectionMatrix;

		void RecalculateViewMatrix();
		void RecalculateProjection();
	};

}
