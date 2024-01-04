/**
 * @file Matrix4.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Math/Vector3.hpp"

namespace SW {

	/**
	 * @brief Matrix4 is a structure representing mathematical matrix in 4D space.
	 * @note Matrix is stored in column-major order. T type must be arithmetic meaning it can be used in mathematical operations.
	 *
	 * @param T Type of the matrix.
	 */
    template <typename T>
        requires std::is_arithmetic_v<T>
    struct Matrix4 final
    {
		/**
		 * @brief Construct a new Matrix 4 object
		 * @note It is initialized with zeros.
		 */
		Matrix4() = default;

		/**
		 * @brief Construct a new Matrix 4 object
		 * @note It is an identity matrix.
		 * @param x Initial value of the matrix.
		 */
        Matrix4(T x) {
            data[0] = static_cast<T>(x);
            data[5] = static_cast<T>(x);
            data[10] = static_cast<T>(x);
            data[15] = static_cast<T>(x);
        }

		/** @brief Copy constructor. */
        Matrix4(const Matrix4& other) = default;
		/** @brief Move constructor. */
        Matrix4(Matrix4&& other) = default;
		/** @brief Copy assignment operator. */
        Matrix4& operator=(const Matrix4& other) = default;
		/** @brief Move assignment operator. */
        Matrix4& operator=(Matrix4&& other) = default;

		/**
		 * @brief Destroy the Matrix 4 object
		 * 
		 */
        ~Matrix4() = default;

		/**
		 * @brief The data of the matrix is stored in an array.
		 * @note The matrix is stored in column-major order.
		 */
        T data[16] = { static_cast<T>(0) };

		/**
		 * @brief Returns a string representation of the matrix.
		 * @note In the format of: \n
		 * { data[0], data[1], data[2], data[3] \n
		 *  data[4], data[5], data[6], data[7] \n
		 * data[8], data[9], data[10], data[11] \n
		 * data[12], data[13], data[14], data[15] }
		 * @return std::string 
		 */
        std::string ToString() const {
            const std::string str =
                "\n{ " + std::to_string(data[0]) + ", " + std::to_string(data[1]) + ", " + std::to_string(data[2]) + ", " + std::to_string(data[3]) + "\n" +
                "  " + std::to_string(data[4]) + ", " + std::to_string(data[5]) + ", " + std::to_string(data[6]) + ", " + std::to_string(data[7]) + "\n" +
                "  " + std::to_string(data[8]) + ", " + std::to_string(data[9]) + ", " + std::to_string(data[10]) + ", " + std::to_string(data[11]) + "\n" +
                "  " + std::to_string(data[12]) + ", " + std::to_string(data[13]) + ", " + std::to_string(data[14]) + ", " + std::to_string(data[15]) + " }";

            return str;
        }

		/**
		 * @brief Returns a pointer to the beginning of the data of the matrix.
		 * 
		 * @return const T* Pointer to the beginning of the data of the matrix.
		 */
        const T* ValuePtr() const {
            return data;
        }

		/**
		 * @brief Translates the matrix by the given vector.
		 * 
		 * @param translation Vector by which the matrix will be translated.
		 */
		void Translate(const Vector3<T>& translation) {
	        data[12] += translation.x;
			data[13] += translation.y;
			data[14] += translation.z;
        }

		/**
		 * @brief Rotates the matrix by the given angle around the X axis.
		 * 
		 * @param radians Angle in radians.
		 */
        void RotateX(f32 radians) {
            data[5] = static_cast<T>(cos(radians));
            data[6] = static_cast<T>(sin(radians));
            data[9] = static_cast<T>(-sin(radians));
            data[10] = static_cast<T>(cos(radians));
        }

		/**
		 * @brief Rotates the matrix by the given angle around the Y axis.
		 * 
		 * @param radians Angle in radians.
		 */
        void RotateY(f32 radians) {
            data[0] = static_cast<T>(cos(radians));
            data[2] = static_cast<T>(-sin(radians));
            data[8] = static_cast<T>(sin(radians));
            data[10] = static_cast<T>(cos(radians));
        }

		/**
		 * @brief Rotates the matrix by the given angle around the Z axis.
		 * 
		 * @param radians Angle in radians.
		 */
        void RotateZ(f32 radians) {
            data[0] = static_cast<T>(cos(radians));
            data[1] = static_cast<T>(sin(radians));
            data[4] = static_cast<T>(-sin(radians));
            data[5] = static_cast<T>(cos(radians));
        }
    };

}
