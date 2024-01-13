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

namespace SW {

	class SceneCamera final : public Camera
	{
	public:
		SceneCamera();
		~SceneCamera() override = default;

		void SetViewportSize(u32 width, u32 height);

		void RecalculateProjection();

		f32 GetOrthographicSize() const { return m_OrthographicSize; }
		f32 GetOrthographicNearClip() const { return m_OrthographicNear; }
		f32 GetOrthographicFarClip() const { return m_OrthographicFar; }

		void SetOrthographicSize(f32 size) { m_OrthographicSize = size; RecalculateProjection(); }
		void SetOrthographicNearClip(f32 nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		void SetOrthographicFarClip(f32 farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

	private:
		f32 m_OrthographicSize = 10.0f;
		f32 m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		f32 m_AspectRatio = 0.0f;
	};

}
