/**
 * @file Vector4.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	/**
	 * @brief Vector4 is a structure representing mathematical vector in 4D space.
	 *
	 * @param T Type of the vector.
	 */
    template <typename T>
    struct Vector4 final
    {
		/**
		 * @brief Construct a new Vector4 object.
		 * @note Vector is initialized with zeros.
		 */
		Vector4() = default;

		/**
		 * @brief Construct a new Vector 4 object
		 * 
		 * @param x X, Y, Z, W coordinate of the vector.
		 */
        Vector4(T x) {
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(x);
            data[2] = static_cast<T>(x);
            data[3] = static_cast<T>(x);
        }

		/**
		 * @brief Construct a new Vector 4 object
		 * 
		 * @param x X coordinate of the vector.
		 * @param y Y coordinate of the vector.
		 * @param z Z coordinate of the vector.
		 * @param w W coordinate of the vector.
		 */
        Vector4(T x, T y, T z, T w) {
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(y);
            data[2] = static_cast<T>(z);
            data[3] = static_cast<T>(w);
        }

		/** @brief Copy constructor. */
        Vector4(const Vector4& other) = default;
		/** @brief Move constructor. */
        Vector4(Vector4&& other) = default;
		/** @brief Copy assignment operator. */
        Vector4& operator=(const Vector4& other) = default;
		/** @brief Move assignment operator. */
        Vector4& operator=(Vector4&& other) = default;

		/**
		 * @brief Destroy the Vector 4 object
		 */
        ~Vector4() = default;

		/**
		 * @brief The data of the vector stored in an array.
		 * @note Data can be accessed by using pairs of names (x, y, z, w), (r, g, b, a), (s, t, p, q), (u, v, w, t).
		 */
        union {
            T data[4] = { static_cast<T>(0) };

            struct {
                union { T x, r; };
                union { T y, g; };
                union { T z, b; };
                union { T w, a; };

            };
        };

		/**
		 * @brief Returns a string representation of the vector.
		 * @note In format: { x, y, z, w }, e.g. { 1.0, 2.0, 3.0, 4.0 }.
		 * 
		 * @return std::string String representation of the vector.
		 */
        std::string ToString() const {
            const std::string str = "{ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + " }";

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
			return static_cast<T>(sqrt(x * x + y * y + z * z + w * w));
		}

		/**
		 * @brief Normalizes the vector.
		 */
		void Normalize() {
			const T length = Length();

			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}
    };

}
