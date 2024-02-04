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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

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
		 * Converts the given vector of degrees to vector of radians.
		 *
		 * @param degrees The vector of degrees to be converted.
		 * @return The vector of radians.
		 */
		inline Vector3<f32> ToRadians(const Vector3<f32> degrees)
		{
			return { ToRadians(degrees.x), ToRadians(degrees.y), ToRadians(degrees.z) };
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
		 * Converts the given vector of radians to vector of degrees.
		 *
		 * @param radians The vector of radians to convert.
		 * @return The vector of degrees.
		 */
		inline Vector3<f32> ToDegrees(const Vector3<f32> radians)
		{
			return { ToDegrees(radians.x), ToDegrees(radians.y), ToDegrees(radians.z) };
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

		template <typename T>
			requires std::is_arithmetic_v<T>
		inline Matrix4<T> Translate(const Matrix4<T>& matrix, const Vector3<f32> vector)
		{
			Matrix4<T> result = matrix;

			result[3] = vector.x;
			result[7] = vector.y;
			result[11] = vector.z;

			return result;
		}

		template <typename T>
			requires std::is_arithmetic_v<T>
		inline Matrix4<T> RotateZ(const Matrix4<T>& matrix, f32 radians)
		{
			Matrix4<T> result = matrix;

			const f32 c = std::cos(radians);
			const f32 s = std::sin(radians);

			result[0] = c;
			result[1] = s;
			result[4] = -s;
			result[5] = c;

			return result;
		}

		inline bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
		{
			// From glm::decompose in matrix_decompose.inl
			using namespace glm;
			using T = float;

			mat4 LocalMatrix(transform);

			// Normalize the matrix.
			if (epsilonEqual(LocalMatrix[3][3], 0.0f, epsilon<T>()))
				return false;

			// First, isolate perspective.  This is the messiest.
			if (
				epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>())) {
				// Clear the perspective partition
				LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
				LocalMatrix[3][3] = static_cast<T>(1);
			}

			// Next take care of translation (easy).
			translation = vec3(LocalMatrix[3]);
			LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

			vec3 Row[3];

			// Now get scale and shear.
			for (length_t i = 0; i < 3; ++i)
				for (length_t j = 0; j < 3; ++j)
					Row[i][j] = LocalMatrix[i][j];

			// Compute X scale factor and normalize first row.
			scale.x = length(Row[0]);
			Row[0] = detail::scale(Row[0], static_cast<T>(1));
			scale.y = length(Row[1]);
			Row[1] = detail::scale(Row[1], static_cast<T>(1));
			scale.z = length(Row[2]);
			Row[2] = detail::scale(Row[2], static_cast<T>(1));

			rotation.y = asin(-Row[0][2]);
			if (cos(rotation.y) != 0.0f) {
				rotation.x = atan2(Row[1][2], Row[2][2]);
				rotation.z = atan2(Row[0][1], Row[0][0]);
			}
			else {
				rotation.x = atan2(-Row[2][0], Row[1][1]);
				rotation.z = 0;
			}

			return true;
		}
    }

}
