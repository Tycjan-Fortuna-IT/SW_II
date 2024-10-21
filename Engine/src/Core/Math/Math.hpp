/**
 * @file Math.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.3
 * @date 2024-03-12
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

// #include "Core/Math/Matrix4.hpp"

#undef near
#undef far

namespace SW
{

	namespace Math
	{

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
		/*static Vector3<f32> ToRadians(const Vector3<f32> degrees)
		{
		    return { ToRadians(degrees.x), ToRadians(degrees.y), ToRadians(degrees.z) };
		}*/

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
		/*static Vector3<f32> ToDegrees(const Vector3<f32> radians)
		{
		    return { ToDegrees(radians.x), ToDegrees(radians.y), ToDegrees(radians.z) };
		}*/

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
		/*template <typename T>
		    requires std::is_arithmetic_v<T>
		static Matrix4<T> OrthogonalProjection(T left, T right, T bottom, T top, T near, T far)
		{
		    Matrix4<T> result = { static_cast<T>(1) };

		    result[0] = static_cast<T>(2) / (right - left);
		    result[5] = static_cast<T>(2) / (top - bottom);
		    result[10] = -static_cast<T>(2) / (far - near);
		    result[12] = -(right + left) / (right - left);
		    result[13] = -(top + bottom) / (top - bottom);
		    result[14] = -(far + near) / (far - near);

		    return result;
		}*/

		/**
		 * @brief Performs inverse of a matrix.
		 *
		 * @param matrix Matrix to be inverted.
		 * @return Matrix4<T> Inverted matrix.
		 */
		// template <typename T>
		//	requires std::is_arithmetic_v<T>
		// static Matrix4<T> Inverse(const Matrix4<T>& matrix)
		//{
		//	Matrix4<T> result;

		//	const T det_2x2_1 = matrix[10] * matrix[15] - matrix[11] * matrix[14];
		//	const T det_2x2_2 = matrix[9] * matrix[15] - matrix[11] * matrix[13];
		//	const T det_2x2_3 = matrix[9] * matrix[14] - matrix[10] * matrix[13];
		//	const T det_2x2_4 = matrix[8] * matrix[15] - matrix[11] * matrix[12];
		//	const T det_2x2_5 = matrix[8] * matrix[14] - matrix[10] * matrix[12];
		//	const T det_2x2_6 = matrix[8] * matrix[13] - matrix[9] * matrix[12];
		//	const T det_2x2_7 = matrix[6] * matrix[15] - matrix[7] * matrix[14];
		//	const T det_2x2_8 = matrix[5] * matrix[15] - matrix[7] * matrix[13];
		//	const T det_2x2_9 = matrix[5] * matrix[14] - matrix[6] * matrix[13];
		//	const T det_2x2_10 = matrix[6] * matrix[11] - matrix[7] * matrix[10];
		//	const T det_2x2_11 = matrix[4] * matrix[15] - matrix[7] * matrix[12];
		//	const T det_2x2_12 = matrix[5] * matrix[10] - matrix[6] * matrix[9];
		//	const T det_2x2_13 = matrix[4] * matrix[15] - matrix[7] * matrix[12];
		//	const T det_2x2_14 = matrix[4] * matrix[14] - matrix[6] * matrix[12];
		//	const T det_2x2_15 = matrix[4] * matrix[11] - matrix[7] * matrix[8];
		//	const T det_2x2_16 = matrix[4] * matrix[10] - matrix[6] * matrix[8];
		//	const T det_2x2_17 = matrix[4] * matrix[13] - matrix[5] * matrix[12];
		//	const T det_2x2_18 = matrix[4] * matrix[9] - matrix[5] * matrix[8];

		//	const T mul_1 = matrix[5] * det_2x2_1;
		//	const T mul_2 = matrix[6] * det_2x2_2;
		//	const T mul_3 = matrix[7] * det_2x2_3;
		//	const T mul_4 = matrix[4] * det_2x2_1;
		//	const T mul_5 = matrix[6] * det_2x2_4;
		//	const T mul_6 = matrix[7] * det_2x2_5;
		//	const T mul_7 = matrix[4] * det_2x2_2;
		//	const T mul_8 = matrix[5] * det_2x2_4;
		//	const T mul_9 = matrix[7] * det_2x2_6;
		//	const T mul_10 = matrix[4] * det_2x2_3;
		//	const T mul_11 = matrix[5] * det_2x2_5;
		//	const T mul_12 = matrix[6] * det_2x2_6;

		//	// determinant
		//	T d1 = matrix[0] * (mul_1 - mul_2 + mul_3);
		//	T d2 = matrix[1] * (mul_4 - mul_5 + mul_6);
		//	T d3 = matrix[2] * (mul_7 - mul_8 + mul_9);
		//	T d4 = matrix[3] * (mul_10 - mul_11 + mul_12);

		//	T det = d1 - d2 + d3 - d4;

		//	if (det == static_cast<T>(0)) {
		//		SYSTEM_ERROR("Matrix4::Inverse() - Matrix is not invertible. {}", matrix.ToString());
		//		return result;
		//	}

		//	T idet = static_cast<T>(1) / det;

		//	result[0] = idet * (mul_1 - mul_2 + mul_3);

		//	result[1] = -idet * (
		//		matrix[1] * det_2x2_1 -
		//		matrix[2] * det_2x2_2 +
		//		matrix[3] * det_2x2_3
		//	);

		//	result[2] = idet * (
		//		matrix[1] * det_2x2_7 -
		//		matrix[2] * det_2x2_8 +
		//		matrix[3] * det_2x2_9
		//	);

		//	result[3] = -idet * (
		//		matrix[1] * det_2x2_10 -
		//		matrix[2] * det_2x2_11 +
		//		matrix[3] * det_2x2_12
		//	);

		//	result[4] = -idet * (mul_4 - mul_5 + mul_6);

		//	result[5] = idet * (
		//		matrix[0] * det_2x2_1 -
		//		matrix[2] * det_2x2_4 +
		//		matrix[3] * det_2x2_5
		//	);

		//	result[6] = -idet * (
		//		matrix[0] * det_2x2_7 -
		//		matrix[2] * det_2x2_13 +
		//		matrix[3] * det_2x2_14
		//	);

		//	result[7] = idet * (
		//		matrix[0] * det_2x2_10 -
		//		matrix[2] * det_2x2_15 +
		//		matrix[3] * det_2x2_16
		//	);

		//	result[8] = idet * (mul_7 - mul_8 + mul_9);

		//	result[9] = -idet * (
		//		matrix[0] * det_2x2_2 -
		//		matrix[1] * det_2x2_4 +
		//		matrix[3] * det_2x2_6
		//	);

		//	result[10] = idet * (
		//		matrix[0] * det_2x2_8 -
		//		matrix[1] * det_2x2_13 +
		//		matrix[3] * det_2x2_17
		//	);

		//	result[11] = -idet * (
		//		matrix[0] * det_2x2_11 -
		//		matrix[1] * det_2x2_15 +
		//		matrix[3] * det_2x2_18
		//	);

		//	result[12] = -idet * (mul_10 - mul_11 + mul_12);

		//	result[13] = idet * (
		//		matrix[0] * det_2x2_3 -
		//		matrix[1] * det_2x2_5 +
		//		matrix[2] * det_2x2_6
		//	);

		//	result[14] = -idet * (
		//		matrix[0] * det_2x2_9 -
		//		matrix[1] * det_2x2_14 +
		//		matrix[2] * det_2x2_17
		//	);

		//	result[15] = idet * (
		//		matrix[0] * det_2x2_12 -
		//		matrix[1] * det_2x2_16 +
		//		matrix[2] * det_2x2_18
		//	);

		//	return result;
		//}

		/**
		 * @brief Performs scale of a matrix.
		 *
		 * @param matrix Matrix to be scaled.
		 * @param vector Vector used to scale the matrix.
		 * @return Matrix4<T> scaled matrix.
		 */
		/*template <typename T>
		    requires std::is_arithmetic_v<T>
		static Matrix4<T> Scale(const Matrix4<T>& matrix, const Vector3<f32> vector)
		{
		    Matrix4<T> result = matrix;

		    result[0] = vector.x;
		    result[5] = vector.y;
		    result[10] = vector.z;

		    return result;
		}*/

		/*template <typename T>
		    requires std::is_arithmetic_v<T>
		static Matrix4<T> Translate(const Matrix4<T>& matrix, const Vector3<f32> vector)
		{
		    Matrix4<T> result = matrix;

		    result[3] = vector.x;
		    result[7] = vector.y;
		    result[11] = vector.z;

		    return result;
		}*/

		/*template <typename T>
		    requires std::is_arithmetic_v<T>
		static Matrix4<T> RotateZ(const Matrix4<T>& matrix, f32 radians)
		{
		    Matrix4<T> result = matrix;

		    const f32 c = std::cos(radians);
		    const f32 s = std::sin(radians);

		    result[0] = c;
		    result[1] = s;
		    result[4] = -s;
		    result[5] = c;

		    return result;
		}*/

		/**
		 * @brief Decomposes a transformation matrix into its translation, rotation and scale components.
		 *
		 * @param transform The transformation matrix to decompose.
		 * @param translation Output parameter for the translation component.
		 * @param rotation Output parameter for the rotation component.
		 * @param scale Output parameter for the scale component.
		 * @return True if the decomposition was successful, false otherwise.
		 */
		static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation,
		                               glm::vec3& scale)
		{
			// From glm::decompose in matrix_decompose.inl
			using namespace glm;
			using T = f32;

			mat4 LocalMatrix(transform);

			// Normalize the matrix.
			if (epsilonEqual(LocalMatrix[3][3], 0.0f, epsilon<T>()))
				return false;

			// First, isolate perspective.  This is the messiest.
			if (epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			    epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			    epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
			{
				// Clear the perspective partition
				LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
				LocalMatrix[3][3]                                         = static_cast<T>(1);
			}

			// Next take care of translation (easy).
			translation    = vec3(LocalMatrix[3]);
			LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

			vec3 Row[3];

			// Now get scale and shear.
			for (length_t i = 0; i < 3; ++i)
				for (length_t j = 0; j < 3; ++j) Row[i][j] = LocalMatrix[i][j];

			// Compute X scale factor and normalize first row.
			scale.x = length(Row[0]);
			Row[0]  = detail::scale(Row[0], static_cast<T>(1));
			scale.y = length(Row[1]);
			Row[1]  = detail::scale(Row[1], static_cast<T>(1));
			scale.z = length(Row[2]);
			Row[2]  = detail::scale(Row[2], static_cast<T>(1));

			rotation.y = asin(-Row[0][2]);
			if (cos(rotation.y) != 0.0f)
			{
				rotation.x = atan2(Row[1][2], Row[2][2]);
				rotation.z = atan2(Row[0][1], Row[0][0]);
			}
			else
			{
				rotation.x = atan2(-Row[2][0], Row[1][1]);
				rotation.z = 0;
			}

			return true;
		}

		/**
		 * @brief Decomposes a transformation matrix into its translation.
		 *
		 * @param transform The transformation matrix to decompose.
		 * @param translation Output parameter for the translation component.
		 * @return True if the decomposition was successful, false otherwise.
		 */
		static bool DecomposeTransformForTranslation(const glm::mat4& transform, glm::vec3& translation)
		{
			// From glm::decompose in matrix_decompose.inl
			using namespace glm;
			using T = f32;

			mat4 LocalMatrix(transform);

			// Normalize the matrix.
			if (epsilonEqual(LocalMatrix[3][3], 0.0f, epsilon<T>()))
				return false;

			// First, isolate perspective.  This is the messiest.
			if (epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			    epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			    epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
			{
				// Clear the perspective partition
				LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
				LocalMatrix[3][3]                                         = static_cast<T>(1);
			}

			// Next take care of translation (easy).
			translation = vec3(LocalMatrix[3]);

			return true;
		}

		/**
		 * @brief Decomposes a transformation matrix into its translation and rotation components.
		 *
		 * @param transform The transformation matrix to decompose.
		 * @param translation Output parameter for the translation component.
		 * @param rotation Output parameter for the rotation component.
		 * @return True if the decomposition was successful, false otherwise.
		 */
		static bool DecomposeTransformForTranslationAndRotation(const glm::mat4& transform, glm::vec3& translation,
		                                                        glm::vec3& rotation)
		{
			// From glm::decompose in matrix_decompose.inl
			using namespace glm;
			using T = f32;

			mat4 LocalMatrix(transform);

			// Normalize the matrix.
			if (epsilonEqual(LocalMatrix[3][3], 0.0f, epsilon<T>()))
				return false;

			// First, isolate perspective.  This is the messiest.
			if (epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			    epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			    epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
			{
				// Clear the perspective partition
				LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
				LocalMatrix[3][3]                                         = static_cast<T>(1);
			}

			// Next take care of translation (easy).
			translation    = vec3(LocalMatrix[3]);
			LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

			vec3 Row[3];

			// Now get scale and shear.
			for (length_t i = 0; i < 3; ++i)
				for (length_t j = 0; j < 3; ++j) Row[i][j] = LocalMatrix[i][j];

			rotation.y = asin(-Row[0][2]);
			if (cos(rotation.y) != 0.0f)
			{
				rotation.x = atan2(Row[1][2], Row[2][2]);
				rotation.z = atan2(Row[0][1], Row[0][0]);
			}
			else
			{
				rotation.x = atan2(-Row[2][0], Row[1][1]);
				rotation.z = 0;
			}

			return true;
		}

		/**
		 * Smoothly interpolates between the current value and the target value over time using the SmoothDamp
		 * algorithm.
		 *
		 * @tparam T The type of the values being interpolated.
		 * @param current The current value.
		 * @param target The target value.
		 * @param currentVelocity The current velocity of the interpolation.
		 * @param smoothTime The time taken to reach the target value.
		 * @param maxSpeed The maximum speed at which the value can change.
		 * @param deltaTime The time elapsed since the last update.
		 * @return The interpolated value.
		 */
		template <typename T>
		static T SmoothDamp(const T& current, const T& target, T& currentVelocity, f32 smoothTime, f32 maxSpeed,
		                    f32 deltaTime)
		{
			// Based on Game Programming Gems 4 Chapter 1.10
			smoothTime      = glm::max(0.0001F, smoothTime);
			const f32 omega = 2.0f / smoothTime;

			const f32 x   = omega * deltaTime;
			const f32 exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

			T change           = current - target;
			const T originalTo = target;

			// Clamp maximum speed
			const f32 maxChange = maxSpeed * smoothTime;

			const f32 maxChangeSq = maxChange * maxChange;
			const f32 sqDist      = glm::length2(change);
			if (sqDist > maxChangeSq)
			{
				const f32 mag = glm::sqrt(sqDist);
				change        = change / mag * maxChange;
			}

			const T newTarget = current - change;
			const T temp      = (currentVelocity + omega * change) * deltaTime;

			currentVelocity = (currentVelocity - omega * temp) * exp;

			T output = newTarget + (change + temp) * exp;

			// Prevent overshooting
			const T origMinusCurrent = originalTo - current;
			const T outMinusOrig     = output - originalTo;

			if (glm::compAdd(origMinusCurrent * outMinusOrig) > 0.0f)
			{
				output          = originalTo;
				currentVelocity = (output - originalTo) / deltaTime;
			}
			return output;
		}
	} // namespace Math

} // namespace SW
