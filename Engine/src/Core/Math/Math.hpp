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
    		requires std::is_arithmetic_v<T>
		inline Matrix4<T> OrthogonalProjection(T left, T right, T bottom, T top, T near, T far) {
			Matrix4<T> result = { static_cast<T>(1) };

			result[0] = static_cast<T>(2) / (right - left);
			result[5] = static_cast<T>(2) / (top - bottom);
			result[10] = -static_cast<T>(2) / (far - near);
			result[12] = -(right + left) / (right - left);
			result[13] = -(top + bottom) / (top - bottom);
			result[14] = -(far + near) / (far - near);

			return result;
		}

		template <typename T>
			requires std::is_arithmetic_v<T>
		inline Matrix4<T> Inverse(const Matrix4<T>& matrix) {
			Matrix4<T> result;

			// determinant
			T d1 = matrix[0] * (
				   matrix[5] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) -
				   matrix[6] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) +
				   matrix[7] * (matrix[9] * matrix[14] - matrix[10] * matrix[13])
			);

			T d2 = matrix[1] * (
				   matrix[4] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) -
				   matrix[6] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) +
				   matrix[7] * (matrix[8] * matrix[14] - matrix[10] * matrix[12])
			);

			T d3 = matrix[2] * (
				   matrix[4] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) -
				   matrix[5] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) +
				   matrix[7] * (matrix[8] * matrix[13] - matrix[9] * matrix[12])
			);

			T d4 = matrix[3] * (
				   matrix[4] * (matrix[9] * matrix[14] - matrix[10] * matrix[13]) -
				   matrix[5] * (matrix[8] * matrix[14] - matrix[10] * matrix[12]) +
				   matrix[6] * (matrix[8] * matrix[13] - matrix[9] * matrix[12])
			);

			T det = d1 - d2 + d3 - d4;

			if (det == static_cast<T>(0)) {
				SW_ERROR("Matrix4::Inverse() - Matrix is not invertible. {}", matrix.ToString());
				return result;
			}

			T idet = static_cast<T>(1) / det;

			result[0] = idet * (
					matrix[5] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) -
					matrix[6] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) +
					matrix[7] * (matrix[9] * matrix[14] - matrix[10] * matrix[13])
			);

			result[1] = -idet * (
					matrix[1] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) -
					matrix[2] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) +
					matrix[3] * (matrix[9] * matrix[14] - matrix[10] * matrix[13])
			);

			result[2] = idet * (
					matrix[1] * (matrix[6] * matrix[15] - matrix[7] * matrix[14]) -
					matrix[2] * (matrix[5] * matrix[15] - matrix[7] * matrix[13]) +
					matrix[3] * (matrix[5] * matrix[14] - matrix[6] * matrix[13])
			);

			result[3] = -idet * (
					matrix[1] * (matrix[6] * matrix[11] - matrix[7] * matrix[10]) -
					matrix[2] * (matrix[5] * matrix[11] - matrix[7] * matrix[9]) +
					matrix[3] * (matrix[5] * matrix[10] - matrix[6] * matrix[9])
			);

			result[4] = -idet * (
					matrix[4] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) -
					matrix[6] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) +
					matrix[7] * (matrix[8] * matrix[14] - matrix[10] * matrix[12])
			);

			result[5] = idet * (
					matrix[0] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) -
					matrix[2] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) +
					matrix[3] * (matrix[8] * matrix[14] - matrix[10] * matrix[12])
			);

			result[6] = -idet * (
					matrix[0] * (matrix[6] * matrix[15] - matrix[7] * matrix[14]) -
					matrix[2] * (matrix[4] * matrix[15] - matrix[7] * matrix[12]) +
					matrix[3] * (matrix[4] * matrix[14] - matrix[6] * matrix[12])
			);

			result[7] = idet * (
					matrix[0] * (matrix[6] * matrix[11] - matrix[7] * matrix[10]) -
					matrix[2] * (matrix[4] * matrix[11] - matrix[7] * matrix[8]) +
					matrix[3] * (matrix[4] * matrix[10] - matrix[6] * matrix[8])
			);

			result[8] = idet * (
					matrix[4] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) -
					matrix[5] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) +
					matrix[7] * (matrix[8] * matrix[13] - matrix[9] * matrix[12])
			);

			result[9] = -idet * (
					matrix[0] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) -
					matrix[1] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) +
					matrix[3] * (matrix[8] * matrix[13] - matrix[9] * matrix[12])
			);

			result[10] = idet * (
					matrix[0] * (matrix[5] * matrix[15] - matrix[7] * matrix[13]) -
					matrix[1] * (matrix[4] * matrix[15] - matrix[7] * matrix[12]) +
					matrix[3] * (matrix[4] * matrix[13] - matrix[5] * matrix[12])
			);

			result[11] = -idet * (
					matrix[0] * (matrix[5] * matrix[11] - matrix[7] * matrix[9]) -
					matrix[1] * (matrix[4] * matrix[11] - matrix[7] * matrix[8]) +
					matrix[3] * (matrix[4] * matrix[9] - matrix[5] * matrix[8])
			);

			result[12] = -idet * (
					matrix[4] * (matrix[9] * matrix[14] - matrix[10] * matrix[13]) -
					matrix[5] * (matrix[8] * matrix[14] - matrix[10] * matrix[12]) +
					matrix[6] * (matrix[8] * matrix[13] - matrix[9] * matrix[12])
			);

			result[13] = idet * (
					matrix[0] * (matrix[9] * matrix[14] - matrix[10] * matrix[13]) -
					matrix[1] * (matrix[8] * matrix[14] - matrix[10] * matrix[12]) +
					matrix[2] * (matrix[8] * matrix[13] - matrix[9] * matrix[12])
			);

			result[14] = -idet * (
					matrix[0] * (matrix[5] * matrix[14] - matrix[6] * matrix[13]) -
					matrix[1] * (matrix[4] * matrix[14] - matrix[6] * matrix[12]) +
					matrix[2] * (matrix[4] * matrix[13] - matrix[5] * matrix[12])
			);

			result[15] = idet * (
					matrix[0] * (matrix[5] * matrix[10] - matrix[6] * matrix[9]) -
					matrix[1] * (matrix[4] * matrix[10] - matrix[6] * matrix[8]) +
					matrix[2] * (matrix[4] * matrix[9] - matrix[5] * matrix[8])
			);

			return result;
		}
    }

}
