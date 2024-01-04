/**
 * @file Vector2.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	/**
	 * @brief Vector2 is a structure representing mathematical vector in 2D space.
	 * 
	 * @param T Type of the vector.
	 */
	template <typename T>
	struct Vector2 final
	{
		/**
		 * @brief Construct a new Vector2 object.
		 * @note Vector is initialized with zeros.
		 */
		Vector2() = default;

		/**
		 * @brief Construct a new Vector2 object.
		 * 
		 * @param x X, Y coordinate of the vector.
		 */
		Vector2(T x) {
			data[0] = static_cast<T>(x);
			data[1] = static_cast<T>(x);
		}

		/**
		 * @brief Construct a new Vector2 object.
		 * 
		 * @param x X coordinate of the vector.
		 * @param y Y coordinate of the vector.
		 * @param z Z coordinate of the vector.
		 */
		Vector2(T x, T y, T z) {
			data[0] = static_cast<T>(x);
			data[1] = static_cast<T>(y);
		}

		/** @brief Copy constructor. */
		Vector2(const Vector2& other) = default;
		/** @brief Move constructor. */
		Vector2(Vector2&& other) = default;
		/** @brief Copy assignment operator. */
		Vector2& operator=(const Vector2& other) = default;
		/** @brief Move assignment operator. */
		Vector2& operator=(Vector2&& other) = default;

		/**
		 * @brief Destroy the Vector 2 object
		 */
		~Vector2() = default;

		/**
		 * @brief The data of the vector stored in an array.
		 * @note Data can be accessed by using pairs of names (x, y), (r, g), (s, t), (u, v).
		 */
		union {
			T data[2] = { static_cast<T>(0) };

			struct {
				union { T x, r, s, u; };
				union { T y, g, t, v; };
			};
		};

		/**
		 * @brief Returns a string representation of the vector.
		 * @note In format: { x, y }, e.g. { 1.0, 2.0 }.
		 * 
		 * @return std::string String representation of the vector.
		 */
		std::string ToString() const {
			const std::string str = "{ " + std::to_string(x) + ", " + std::to_string(y) + " }";

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
			return static_cast<T>(sqrt(x * x + y * y));
		}

		/**
		 * @brief Normalizes the vector.
		 */
		void Normalize() {
			const T length = Length();

			x /= length;
			y /= length;
		}
	};

}
