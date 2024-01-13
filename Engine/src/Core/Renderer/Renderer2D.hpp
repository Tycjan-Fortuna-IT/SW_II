/**
 * @file Renderer2D.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-13
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Math/Vector2.hpp"
#include "Core/Math/Vector3.hpp"
#include "Core/Math/Vector4.hpp"
#include "Core/Math/Matrix4.hpp"

namespace SW {

	class Shader;

	class Renderer2D final
	{
		static void Init(const std::shared_ptr<Shader>& shader);
		static void Shutdown();

		static void DrawQuad(const Matrix4<f32>& transform, const Vector4<f32>& color);
	};

}
