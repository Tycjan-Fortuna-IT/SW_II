/**
 * @file Math.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.2
 * @date 2024-01-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Matrix4.hpp"

#undef near
#undef far

namespace SW {

    namespace Math {

		/**
		 * @brief Converts degrees to radians.
		 * 
		 * @param degrees Degrees to convert. 
		 * @return constexpr f32 
		 */
        constexpr f32 ToRadians(f32 degrees) {
            return degrees * 0.017453292519943295f;
        }

		/**
		 * @brief Converts radians to degrees.
		 * 
		 * @param radians Radians to convert.
		 * @return constexpr f32 
		 */
        constexpr f32 ToDegrees(f32 radians) {
            return radians * 57.29577951308232f;
        }

		/**
		 * @brief Approximation of PI constant.
		 */
        constexpr f32 PI = 3.14159265358979323846264338327950288f;

		/**
		 * @brief Constructs a right-handed orthographic projection matrix.
		 * 
		 * @param left The left edge of the view frustum. 
		 * @param right The right edge of the view frustum.
		 * @param bottom The bottom edge of the view frustum.
		 * @param top The top edge of the view frustum.
		 * @param near The near edge of the view frustum.
		 * @param far The far edge of the view frustum.
		 * @return Matrix4<T>
		 */
		template <typename T>
		inline Matrix4<T> OrthogonalProjection(T left, T right, T bottom, T top, T near, T far) {
			Matrix4<T> result = { static_cast<T>(1) };

			result[0][0] = static_cast<T>(2) / (right - left);
			result[1][1] = static_cast<T>(2) / (top - bottom);
			result[2][2] = -static_cast<T>(2) / (far - near);
			result[3][0] = -(right + left) / (right - left);
			result[3][1] = -(top + bottom) / (top - bottom);
			result[3][2] = -(far + near) / (far - near);

			return result;
		}

		template <typename T>
		inline Matrix4<T> Inverse(const Matrix4<T>& matrix) {
			Matrix4<T> result;

			return result;
		}
    }

}
