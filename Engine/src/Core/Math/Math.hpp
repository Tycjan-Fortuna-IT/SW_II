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
        constexpr f32 ToRadians(f32 degrees)
		{
            return degrees * 0.017453292519943295f;
        }

		/**
		 * @brief Converts radians to degrees.
		 * 
		 * @param radians Radians to convert.
		 * @return constexpr f32 
		 */
        constexpr f32 ToDegrees(f32 radians)
		{
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
		inline Matrix4<T> OrthogonalProjection(T left, T right, T bottom, T top, T near, T far)
		{
			Matrix4<T> result = { static_cast<T>(1) };

			result[0] = static_cast<T>(2) / (right - left);
			result[5] = static_cast<T>(2) / (top - bottom);
			result[10] = -static_cast<T>(2) / (far - near);
			result[12] = -(right + left) / (right - left);
			result[13] = -(top + bottom) / (top - bottom);
			result[14] = -(far + near) / (far - near);

			return result;
		}

		/**
		 * @brief Performs inverse of a matrix.
		 * 
		 * @param matrix Matrix to be inverted.
		 * @return Matrix4<T> Inverted matrix. 
		 */
		template <typename T>
			requires std::is_arithmetic_v<T>
		inline Matrix4<T> Inverse(const Matrix4<T>& matrix)
		{
			Matrix4<T> result;

			const T det_2x2_1 = matrix[10] * matrix[15] - matrix[11] * matrix[14];
			const T det_2x2_2 = matrix[9] * matrix[15] - matrix[11] * matrix[13];
			const T det_2x2_3 = matrix[9] * matrix[14] - matrix[10] * matrix[13];
			const T det_2x2_4 = matrix[8] * matrix[15] - matrix[11] * matrix[12];
			const T det_2x2_5 = matrix[8] * matrix[14] - matrix[10] * matrix[12];
			const T det_2x2_6 = matrix[8] * matrix[13] - matrix[9] * matrix[12];
			const T det_2x2_7 = matrix[6] * matrix[15] - matrix[7] * matrix[14];
			const T det_2x2_8 = matrix[5] * matrix[15] - matrix[7] * matrix[13];
			const T det_2x2_9 = matrix[5] * matrix[14] - matrix[6] * matrix[13];
			const T det_2x2_10 = matrix[6] * matrix[11] - matrix[7] * matrix[10];
			const T det_2x2_11 = matrix[4] * matrix[15] - matrix[7] * matrix[12];
			const T det_2x2_12 = matrix[5] * matrix[10] - matrix[6] * matrix[9];
			const T det_2x2_13 = matrix[4] * matrix[15] - matrix[7] * matrix[12];
			const T det_2x2_14 = matrix[4] * matrix[14] - matrix[6] * matrix[12];
			const T det_2x2_15 = matrix[4] * matrix[11] - matrix[7] * matrix[8];
			const T det_2x2_16 = matrix[4] * matrix[10] - matrix[6] * matrix[8];
			const T det_2x2_17 = matrix[4] * matrix[13] - matrix[5] * matrix[12];
			const T det_2x2_18 = matrix[4] * matrix[9] - matrix[5] * matrix[8];

			const T mul_1 = matrix[5] * det_2x2_1;
			const T mul_2 = matrix[6] * det_2x2_2;
			const T mul_3 = matrix[7] * det_2x2_3;
			const T mul_4 = matrix[4] * det_2x2_1;
			const T mul_5 = matrix[6] * det_2x2_4;
			const T mul_6 = matrix[7] * det_2x2_5;
			const T mul_7 = matrix[4] * det_2x2_2;
			const T mul_8 = matrix[5] * det_2x2_4;
			const T mul_9 = matrix[7] * det_2x2_6;
			const T mul_10 = matrix[4] * det_2x2_3;
			const T mul_11 = matrix[5] * det_2x2_5;
			const T mul_12 = matrix[6] * det_2x2_6;

			// determinant
			T d1 = matrix[0] * (mul_1 - mul_2 + mul_3);
			T d2 = matrix[1] * (mul_4 - mul_5 + mul_6);
			T d3 = matrix[2] * (mul_7 - mul_8 + mul_9);
			T d4 = matrix[3] * (mul_10 - mul_11 + mul_12);

			T det = d1 - d2 + d3 - d4;

			if (det == static_cast<T>(0)) {
				SW_ERROR("Matrix4::Inverse() - Matrix is not invertible. {}", matrix.ToString());
				return result;
			}

			T idet = static_cast<T>(1) / det;

			result[0] = idet * (mul_1 - mul_2 + mul_3);

			result[1] = -idet * (
				matrix[1] * det_2x2_1 -
				matrix[2] * det_2x2_2 +
				matrix[3] * det_2x2_3
			);

			result[2] = idet * (
				matrix[1] * det_2x2_7 -
				matrix[2] * det_2x2_8 +
				matrix[3] * det_2x2_9
			);

			result[3] = -idet * (
				matrix[1] * det_2x2_10 -
				matrix[2] * det_2x2_11 +
				matrix[3] * det_2x2_12
			);

			result[4] = -idet * (mul_4 - mul_5 + mul_6);

			result[5] = idet * (
				matrix[0] * det_2x2_1 -
				matrix[2] * det_2x2_4 +
				matrix[3] * det_2x2_5
			);

			result[6] = -idet * (
				matrix[0] * det_2x2_7 -
				matrix[2] * det_2x2_13 +
				matrix[3] * det_2x2_14
			);

			result[7] = idet * (
				matrix[0] * det_2x2_10 -
				matrix[2] * det_2x2_15 +
				matrix[3] * det_2x2_16
			);

			result[8] = idet * (mul_7 - mul_8 + mul_9);

			result[9] = -idet * (
				matrix[0] * det_2x2_2 -
				matrix[1] * det_2x2_4 +
				matrix[3] * det_2x2_6
			);

			result[10] = idet * (
				matrix[0] * det_2x2_8 -
				matrix[1] * det_2x2_13 +
				matrix[3] * det_2x2_17
			);

			result[11] = -idet * (
				matrix[0] * det_2x2_11 -
				matrix[1] * det_2x2_15 +
				matrix[3] * det_2x2_18
			);

			result[12] = -idet * (mul_10 - mul_11 + mul_12);

			result[13] = idet * (
				matrix[0] * det_2x2_3 -
				matrix[1] * det_2x2_5 +
				matrix[2] * det_2x2_6
			);

			result[14] = -idet * (
				matrix[0] * det_2x2_9 -
				matrix[1] * det_2x2_14 +
				matrix[2] * det_2x2_17
			);

			result[15] = idet * (
				matrix[0] * det_2x2_12 -
				matrix[1] * det_2x2_16 +
				matrix[2] * det_2x2_18
			);

			return result;
		}

		/**
		 * @brief Performs scale of a matrix.
		 *
		 * @param matrix Matrix to be scaled.
		 * @param vector Vector used to scale the matrix.
		 * @return Matrix4<T> scaled matrix.
		 */
		template <typename T>
			requires std::is_arithmetic_v<T>
		inline Matrix4<T> Scale(const Matrix4<T>& matrix, const Vector3<f32> vector)
		{
			Matrix4<T> result = matrix;

			result[0] = vector.x;
			result[5] = vector.y;
			result[10] = vector.z;

			return result;
		}
    }

}
