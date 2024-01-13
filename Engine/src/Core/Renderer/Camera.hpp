#pragma once

#include "Core/Math/Matrix4.hpp"

namespace SW {

	class Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;

		const Matrix4<f32>& GetProjection() const { return m_Projection; }

	protected:
		Matrix4<f32> m_Projection = { 1.0f };
	};

}
