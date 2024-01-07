/**
 * @file Vector3.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.2
 * @date 2024-01-06
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Debug/LogSystem.hpp"

namespace SW {

	/**
	 * @brief Vector3 is a structure representing mathematical vector in 3D space.
	 *
	 * @param T Type of the vector.
	 */
    template <typename T>
		requires std::is_arithmetic_v<T>
    struct Vector3 final
    {
		/**
		 * @brief Construct a new Vector3 object.
		 * @note Vector is initialized with zeros.
		 */
		Vector3() = default;

		/**
		 * @brief Construct a new Vector 3 object
		 * 
		 * @param x X, Y, Z coordinate of the vector.
		 */
        Vector3(T x) {
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(x);
            data[2] = static_cast<T>(x);
        }

		/**
		 * @brief Construct a new Vector 3 object
		 * 
		 * @param x X coordinate of the vector.
		 * @param y Y coordinate of the vector.
		 * @param z Z coordinate of the vector.
		 */
        Vector3(T x, T y, T z) {
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(y);
            data[2] = static_cast<T>(z);
        }

		/**
		 * @brief Construct a new Vector3 object from a braced initializer list.
		 *
		 * @param values Initializer list containing the data. The behavior depends on the number of elements:
		 * - No elements: The vector will be filled with zeros.
		 * - One element: All fields will be filled with this value.
		 * - Two or three elements: They will be assigned in sequence, with the last value filling the remaining fields.
		 */
		Vector3(std::initializer_list<T> values) {
			ASSERT(values.size() <= 3, "Too many arguments passed for the Vector3 constructor!");

			auto it = values.begin();
			data[0] = it != values.end() ? *it++ : 0;
			data[1] = it != values.end() ? *it++ : data[0];
			data[2] = it != values.end() ? *it : data[1];
		}

		/** @brief Copy constructor. */
        Vector3(const Vector3& other) = default;
		/** @brief Move constructor. */
        Vector3(Vector3&& other) = default;
		/** @brief Copy assignment operator. */
        Vector3& operator=(const Vector3& other) = default;
		/** @brief Move assignment operator. */
        Vector3& operator=(Vector3&& other) = default;

		/**
		 * @brief Destroy the Vector 3 object
		 */
        ~Vector3() = default;

		/**
		 * @brief The data of the vector stored in an array.
		 * @note Data can be accessed by using pairs of names (x, y, z), (r, g, b), (s, t, p), (u, v, w).
		 */
        union {
            T data[3] = { static_cast<T>(0) };

            struct {
                union { T x, r, s, u; };
                union { T y, g, t, v; };
                union { T z, b, p, w; };
            };
        };

		/**
		 * @brief Returns a string representation of the vector.
		 * @note In format: { x, y, z }, e.g. { 1.0, 2.0, 3.0 }.
		 * 
		 * @return std::string String representation of the vector.
		 */
        std::string ToString() const {
            const std::string str = "{ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + " }";

            return str;
        }

		/**
		 * @brief Returns a pointer to the beginning of the data of the vector.
		 * 
		 * @return const T* Pointer to the beginning of the data of the vector.
		 */
        const T* ValuePtr() const {
            return data;
        }

		/**
		 * @brief Calculates the length of the vector.
		 * 
		 * @return T Length of the vector. 
		 */
        T Length() const {
            return static_cast<T>(sqrt(x * x + y * y + z * z));
        }

		/**
		 * @brief Normalizes the vector.
		 */
        void Normalize() {
            const T length = Length();

            x /= length;
            y /= length;
            z /= length;
        }
    };

}
