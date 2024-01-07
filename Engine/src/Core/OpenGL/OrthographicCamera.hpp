/**
 * @file OrthographicCamera.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Math/Matrix4.hpp"

namespace SW {

	class OrthographicCamera final
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const Matrix4<f32>& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const Matrix4<f32>& GetViewMatrix() const { return m_ViewMatrix; }
		const Matrix4<f32>& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		Matrix4<f32> m_ProjectionMatrix;
		Matrix4<f32> m_ViewMatrix;
		Matrix4<f32> m_ViewProjectionMatrix;

		void RecalculateViewMatrix();
	};

}
