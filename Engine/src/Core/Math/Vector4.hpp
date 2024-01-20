/**
 * @file Vector4.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.5
 * @date 2024-01-20
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Debug/LogSystem.hpp"

namespace SW {

	/**
	 * @brief Vector4 is a structure representing mathematical vector in 4D space.
	 *
	 * @tparam T The type of the vector's components. This should be an arithmetic type.
	 */
    template <typename T>
		requires std::is_arithmetic_v<T>
    struct Vector4 final
    {
		/**
		 * @brief Default constructor.
		 * @note Initializes the vector with zeros.
		 */
		Vector4() = default;

		/**
		 * @brief Constructs a new Vector3 object with both components (X, Y, Z and W) set to the same value.
		 * 
		 * @param x The value to set both components to.
		 */
        Vector4(T x)
		{
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(x);
            data[2] = static_cast<T>(x);
            data[3] = static_cast<T>(x);
        }

		/**
		 * @brief Constructs a new Vector4 object with the specified components.
		 * 
		 * @param x The x-coordinate of the vector.
		 * @param y The y-coordinate of the vector.
		 * @param z The z-coordinate of the vector.
		 * @param w The w-coordinate of the vector.
		 */
        Vector4(T x, T y, T z, T w)
		{
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(y);
            data[2] = static_cast<T>(z);
            data[3] = static_cast<T>(w);
        }

		/**
		 * @brief Constructs a new Vector4 object from an initializer list.
		 *
		 * @param values An initializer list containing the vector's components. The behavior depends on the number of elements:
		 * - No elements: The vector will be filled with zeros.
		 * - One element: Both components will be set to this value.
		 * - Two, three or four elements: The components will be set to these values, in order with the last value filling the remaining fields.
		 */
		Vector4(std::initializer_list<T> values)
		{
			if (values.size() > 4) {
				SW_WARN("Too many arguments passed for the Vector3D constructor! Expected 0, 1, 2, 3 or 4, received: {}", values.size());
			}

			auto it = values.begin();
			data[0] = it != values.end() ? *it++ : 0;
			data[1] = it != values.end() ? *it++ : data[0];
			data[2] = it != values.end() ? *it++ : data[1];
			data[3] = it != values.end() ? *it : data[2];
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
		 * @brief Destructor.
		 */
        ~Vector4() = default;

		/**
		 * @brief The components of the vector, stored in an array.
		 * @note The components can be accessed using the names: (x, y, z, w), (r, g, b, a), (s, t, p, q), (u, v, w, t).
		 */
        union
		{
            T data[4] = { static_cast<T>(0) };

            struct
			{
                union { T x, r; };
                union { T y, g; };
                union { T z, b; };
                union { T w, a; };
            };
        };

		/**
		 * @brief Adds two Vector4 objects.
		 *
		 * @param other The other Vector4 to add.
		 * @return Vector4<T> The result of the addition.
		 */
		Vector4<T> operator+(const Vector4<T>& other) const
		{
			return Vector4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		/**
		 * @brief Adds a scalar value to this vector.
		 *
		 * @param other The scalar value to add.
		 * @return Vector4<T> The result of the addition.
		 */
		Vector4<T> operator+(const T& other) const
		{
			return Vector4<T>(x + other, y + other, z + other, w + other);
		}

		/**
		 * @brief Subtracts two Vector4 objects.
		 *
		 * @param other The other Vector4 to subtract.
		 * @return Vector4<T> The result of the subtraction.
		 */
		Vector4<T> operator-(const Vector4<T>& other) const
		{
			return Vector4<T>(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		/**
		 * @brief Subtracts a scalar value from this vector.
		 *
		 * @param other The scalar value to subtract.
		 * @return Vector4<T> The result of the subtraction.
		 */
		Vector4<T> operator-(const T& other) const
		{
			return Vector4<T>(x - other, y - other, z - other, w - other);
		}

		/**
		 * @brief Multiplies two Vector4 objects.
		 *
		 * @param other The other Vector4 to multiply by.
		 * @return Vector4<T> The result of the multiplication.
		 */
		Vector4<T> operator*(const Vector4<T>& other) const
		{
			return Vector4<T>(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		/**
		 * @brief Multiplies this vector by a scalar value.
		 *
		 * @param other The scalar value to multiply by.
		 * @return Vector4<T> The result of the multiplication.
		 */
		Vector4<T> operator*(const T& other) const
		{
			return Vector4<T>(x * other, y * other, z * other, w * other);
		}

		/**
		 * @brief Divides two Vector4 objects.
		 *
		 * @param other The other Vector4 to divide by.
		 * @return Vector4<T> The result of the division.
		 * @note If the other vector's x, y, z or w component is zero, an error message is logged and the division is not performed.
		 */
		Vector4<T> operator/(const Vector4<T>& other) const
		{
			if (other.x == static_cast<T>(0) || other.y == static_cast<T>(0) || other.z == static_cast<T>(0) || other.w == static_cast<T>(0)) {
				SW_WARN("Cannot divide component of Vector4<T> by zero!");
				return *this;
			}

			return Vector4<T>(x / other.x, y / other.y, z / other.z, w / other.w);
		}

		/**
		 * @brief Divides this vector by a scalar value.
		 *
		 * @param other The scalar value to divide by.
		 * @return Vector4<T> The result of the division.
		 */
		Vector4<T> operator/(const T& other) const
		{
			if (other == static_cast<T>(0)) {
				SW_WARN("Cannot divide component of Vector4<T> by zero!");
				return *this;
			}

			return Vector4<T>(x / other, y / other, z / other, w / other);
		}

		/**
		 * @brief Adds another Vector4 to this vector.
		 *
		 * @param other The other Vector4 to add.
		 * @return Vector4<T>& A reference to this vector after the addition.
		 */
		Vector4<T>& operator+=(const Vector4<T>& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;

			return *this;
		}

		/**
		 * @brief Adds a scalar value to this vector.
		 *
		 * @param other The scalar value to add.
		 * @return Vector4<T>& A reference to this vector after the addition.
		 */
		Vector4<T>& operator+=(const T& other)
		{
			x += other;
			y += other;
			z += other;
			w += other;

			return *this;
		}

		/**
		 * @brief Subtracts another Vector4 from this vector.
		 *
		 * @param other The other Vector4 to subtract.
		 * @return Vector4<T>& A reference to this vector after the subtraction.
		 */
		Vector4<T>& operator-=(const Vector4<T>& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;

			return *this;
		}

		/**
		 * @brief Subtracts a scalar value from this vector.
		 *
		 * @param other The scalar value to subtract.
		 * @return Vector4<T>& A reference to this vector after the subtraction.
		 */
		Vector4<T>& operator-=(const T& other)
		{
			x -= other;
			y -= other;
			z -= other;
			w -= other;

			return *this;
		}

		/**
		 * @brief Multiplies this vector by another Vector4.
		 *
		 * @param other The other Vector4 to multiply by.
		 * @return Vector4<T>& A reference to this vector after the multiplication.
		 */
		Vector4<T>& operator*=(const Vector4<T>& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;

			return *this;
		}

		/**
		 * @brief Multiplies this vector by a scalar value.
		 *
		 * @param other The scalar value to multiply by.
		 * @return Vector4<T>& A reference to this vector after the multiplication.
		 */
		Vector4<T>& operator*=(const T& other)
		{
			x *= other;
			y *= other;
			z *= other;
			w *= other;

			return *this;
		}

		/**
		 * @brief Divides this vector by another Vector4.
		 * @note If the other vector's x, y, z or w component is zero, an error message is logged and the division is not performed.
		 *
		 * @param other The other Vector4 to divide by.
		 * @return Vector4<T>& A reference to this vector after the division.
		 */
		Vector4<T>& operator/=(const Vector4<T>& other)
		{
			if (other.x == static_cast<T>(0) || other.y == static_cast<T>(0) || other.z == static_cast<T>(0) || other.w == static_cast<T>(0)) {
				SW_WARN("Cannot divide component of Vector4<T> by zero!");
				return *this;
			}

			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;

			return *this;
		}

		/**
		 * @brief Divides this vector by a scalar value.
		 *
		 * @param other The scalar value to divide by.
		 * @return Vector4<T>& A reference to this vector after the division.
		 */
		Vector4<T>& operator/=(const T& other)
		{
			if (other == static_cast<T>(0)) {
				SW_WARN("Cannot divide component of Vector4<T> by zero!");
				return *this;
			}

			x /= other;
			y /= other;
			z /= other;
			w /= other;

			return *this;
		}

		/**
		 * @brief Returns a string representation of the vector.
		 * @note In format: { x, y, z, w }, e.g. { 1.0, 2.0, 3.0, 4.0 }.
		 * 
		 * @return std::string String representation of the vector.
		 */
        std::string ToString() const
		{
            const std::string str = "{ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + " }";

            return str;
        }

		/**
		 * @brief Returns a pointer to the first component of the vector (to the beginning of the internal data array).
		 *
		 * @return const T* A pointer to the first component of the vector.
		 */
		T* ValuePtr() { return data; }

		/**
		 * @brief Returns a const pointer to the first component of the vector (to the beginning of the internal data array).
		 * 
		 * @return const T* A pointer to the first component of the vector.
		 */
        const T* ValuePtrConst() const { return data; }
    };

}
