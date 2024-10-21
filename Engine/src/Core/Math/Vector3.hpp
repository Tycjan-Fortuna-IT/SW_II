/**
 * @file Vector3.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.5
 * @date 2024-01-20
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Math/Vector4.hpp"

namespace SW {

	/**
	 * @brief Vector3 is a structure representing mathematical vector in 3D space.
	 *
	 * @tparam T The type of the vector's components. This should be an arithmetic type.
	 */
    template <typename T>
		requires std::is_arithmetic_v<T>
    struct Vector3 final
    {
		/**
		 * @brief Default constructor.
		 * @note Initializes the vector with zeros.
		 */
		Vector3() = default;

		/**
		 * @brief Constructs a new Vector3 object with both components (X, Y and Z) set to the same value.
		 *
		 * @param x The value to set both components to.
		 */
        Vector3(T x)
		{
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(x);
            data[2] = static_cast<T>(x);
        }

		/**
		 * @brief Constructs a new Vector3 object with the specified components.
		 *
		 * @param x The x-coordinate of the vector.
		 * @param y The y-coordinate of the vector.
		 * @param z The z-coordinate of the vector.
		 */
        Vector3(T x, T y, T z)
		{
            data[0] = static_cast<T>(x);
            data[1] = static_cast<T>(y);
            data[2] = static_cast<T>(z);
        }

		/**
		 * @brief Constructs a new Vector3 object from an initializer list.
		 *
		 * @param values An initializer list containing the vector's components. The behavior depends on the number of elements:
		 * - No elements: The vector will be filled with zeros.
		 * - One element: Both components will be set to this value.
		 * - Two or three elements: The components will be set to these values, in order with the last value filling the remaining fields.
		 */
		Vector3(std::initializer_list<T> values)
		{
			if (values.size() > 3) {
				SYSTEM_WARN("Too many arguments passed for the Vector3D constructor! Expected 0, 1, 2 or 3, received: {}", values.size());
			}

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
		 * @brief Destructor.
		 */
        ~Vector3() = default;

		/**
		 * @brief The components of the vector, stored in an array.
		 * @note The components can be accessed using the names: (x, y, z), (r, g, b), (s, t, p), (u, v, w).
		 */
        union
		{
            T data[3] = { static_cast<T>(0) };

            struct
			{
                union { T x, r, s, u; };
                union { T y, g, t, v; };
                union { T z, b, p, w; };
            };
        };

		/**
		 * @brief Adds two Vector3 objects.
		 *
		 * @param other The other Vector3 to add.
		 * @return Vector3<T> The result of the addition.
		 */
		Vector3<T> operator+(const Vector3<T>& other) const
		{
			return Vector3<T>(x + other.x, y + other.y, z + other.z);
		}

		/**
		 * @brief Adds a scalar value to this vector.
		 *
		 * @param other The scalar value to add.
		 * @return Vector3<T> The result of the addition.
		 */
		Vector3<T> operator+(const T& other) const
		{
			return Vector3<T>(x + other, y + other, z + other);
		}

		/**
		 * @brief Subtracts two Vector3 objects.
		 *
		 * @param other The other Vector3 to subtract.
		 * @return Vector3<T> The result of the subtraction.
		 */
		Vector3<T> operator-(const Vector3<T>& other) const
		{
			return Vector3<T>(x - other.x, y - other.y, z - other.z);
		}

		/**
		 * @brief Subtracts a scalar value from this vector.
		 *
		 * @param other The scalar value to subtract.
		 * @return Vector3<T> The result of the subtraction.
		 */
		Vector3<T> operator-(const T& other) const
		{
			return Vector3<T>(x - other, y - other, z - other);
		}

		/**
		 * @brief Multiplies two Vector3 objects.
		 *
		 * @param other The other Vector3 to multiply by.
		 * @return Vector3<T> The result of the multiplication.
		 */
		Vector3<T> operator*(const Vector3<T>& other) const
		{
			return Vector3<T>(x * other.x, y * other.y, z * other.z);
		}

		/**
		 * @brief Multiplies this vector by a scalar value.
		 *
		 * @param other The scalar value to multiply by.
		 * @return Vector3<T> The result of the multiplication.
		 */
		Vector3<T> operator*(const T& other) const
		{
			return Vector3<T>(x * other, y * other, z * other);
		}

		/**
		 * @brief Divides two Vector3 objects.
		 *
		 * @param other The other Vector3 to divide by.
		 * @return Vector3<T> The result of the division.
		 * @note If the other vector's x, y or z component is zero, an error message is logged and the division is not performed.
		 */
		Vector3<T> operator/(const Vector3<T>& other) const
		{
			if (other.x == static_cast<T>(0) || other.y == static_cast<T>(0) || other.z == static_cast<T>(0)) {
				SYSTEM_WARN("Cannot divide component of Vector3<T> by zero!");
				return *this;
			}

			return Vector3<T>(x / other.x, y / other.y, z / other.z);
		}

		/**
		 * @brief Divides this vector by a scalar value.
		 *
		 * @param other The scalar value to divide by.
		 * @return Vector3<T> The result of the division.
		 */
		Vector3<T> operator/(const T& other) const
		{
			if (other == static_cast<T>(0)) {
				SYSTEM_WARN("Cannot divide component of Vector3<T> by zero!");
				return *this;
			}

			return Vector3<T>(x / other, y / other, z / other);
		}

		/**
		 * @brief Adds another Vector3 to this vector.
		 *
		 * @param other The other Vector3 to add.
		 * @return Vector3<T>& A reference to this vector after the addition.
		 */
		Vector3<T>& operator+=(const Vector3<T>& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;

			return *this;
		}

		/**
		 * @brief Adds a scalar value to this vector.
		 *
		 * @param other The scalar value to add.
		 * @return Vector3<T>& A reference to this vector after the addition.
		 */
		Vector3<T>& operator+=(const T& other)
		{
			x += other;
			y += other;
			z += other;

			return *this;
		}

		/**
		 * @brief Subtracts another Vector3 from this vector.
		 *
		 * @param other The other Vector3 to subtract.
		 * @return Vector3<T>& A reference to this vector after the subtraction.
		 */
		Vector3<T>& operator-=(const Vector3<T>& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;

			return *this;
		}

		/**
		 * @brief Subtracts a scalar value from this vector.
		 *
		 * @param other The scalar value to subtract.
		 * @return Vector3<T>& A reference to this vector after the subtraction.
		 */
		Vector3<T>& operator-=(const T& other)
		{
			x -= other;
			y -= other;
			z -= other;

			return *this;
		}

		/**
		 * @brief Multiplies this vector by another Vector3.
		 *
		 * @param other The other Vector3 to multiply by.
		 * @return Vector3<T>& A reference to this vector after the multiplication.
		 */
		Vector3<T>& operator*=(const Vector3<T>& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;

			return *this;
		}

		/**
		 * @brief Multiplies this vector by a scalar value.
		 *
		 * @param other The scalar value to multiply by.
		 * @return Vector3<T>& A reference to this vector after the multiplication.
		 */
		Vector3<T>& operator*=(const T& other)
		{
			x *= other;
			y *= other;
			z *= other;

			return *this;
		}

		/**
		 * @brief Divides this vector by another Vector3.
		 * @note If the other vector's x, y or z component is zero, an error message is logged and the division is not performed.
		 *
		 * @param other The other Vector3 to divide by.
		 * @return Vector3<T>& A reference to this vector after the division.
		 */
		Vector3<T>& operator/=(const Vector3<T>& other)
		{
			if (other.x == static_cast<T>(0) || other.y == static_cast<T>(0) || other.z == static_cast<T>(0)) {
				SYSTEM_WARN("Cannot divide component of Vector3<T> by zero!");
				return *this;
			}

			x /= other.x;
			y /= other.y;
			z /= other.z;

			return *this;
		}

		/**
		 * @brief Divides this vector by a scalar value.
		 *
		 * @param other The scalar value to divide by.
		 * @return Vector3<T>& A reference to this vector after the division.
		 */
		Vector3<T>& operator/=(const T& other)
		{
			if (other == static_cast<T>(0)) {
				SYSTEM_WARN("Cannot divide component of Vector3<T> by zero!");
				return *this;
			}

			x /= other;
			y /= other;
			z /= other;

			return *this;
		}

		/**
		 * @brief Checks if the current vector is equal to another vector.
		 *
		 * @param other The vector to compare against.
		 * @return True if the vectors are equal, false otherwise.
		 */
		bool operator==(const Vector3<T>& other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		/**
		 * @brief Checks if the current vector is not equal to another vector.
		 *
		 * @param other The vector to compare against.
		 * @return True if the vectors are not equal, false otherwise.
		 */
		bool operator!=(const Vector3<T>& other) const
		{
			return !(x == other.x && y == other.y && z == other.z);
		}

		/**
		 * @brief Returns a string representation of the vector.
		 * @note In format: { x, y, z }, e.g. { 1.0, 2.0, 3.0 }.
		 * 
		 * @return std::string A string representation of the vector.
		 */
        std::string ToString() const
		{
            const std::string str = "{ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + " }";

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

		/**
		 * @brief Calculates the length (magnitude) of the vector.
		 *
		 * @return T The length of the vector.
		 */
        T Length() const
		{
            return static_cast<T>(sqrt(x * x + y * y + z * z));
        }

		/**
		 * @brief Normalizes the vector, making its length 1 while preserving its direction.
		 */
        void Normalize()
		{
            const T length = Length();

			if (length == static_cast<T>(0)) {
				SYSTEM_WARN("Cant normalize Vector3<T> with length of 0!");
				return;
			}

            x /= length;
            y /= length;
            z /= length;
        }

		/**
		 * @brief Returns a normalized version of the vector.
		 * @note This does not modify the original vector.
		 *
		 * @return Vector3<T> A normalized version of the vector.
		 */
		Vector3<T> Normalized()
		{
			const T length = Length();

			Vector3<T> result = *this;

			if (length == static_cast<T>(0)) {
				SYSTEM_WARN("Cant normalize Vector2<T> with length of 0!");
				return result;
			}

			result.x /= length;
			result.y /= length;
			result.z /= length;

			return result;
		}

		/**
		 * @brief Returns the dot product of two vectors.
		 * 
		 * @param other Vector to calculate the dot product with.
		 * @return T Dot product of two vectors.
		 */
		T Dot(const Vector3<T>& other) const
		{
			return static_cast<T>(x * other.x + y * other.y + z * other.z);
		}

		/**
		 * @brief Returns the cross product of two vectors.
		 * 
		 * @param other Vector to calculate the cross product with.
		 * @return Vector3<T> Cross product of two vectors.
		 */
		Vector3<T> Cross(const Vector3<T>& other) const
		{
			return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}

		/**
		 * @brief Returns the angle between two vectors.
		 * @note The angle is in radians.
		 * 
		 * @param other Vector to calculate the angle with.
		 * @return T Angle between two vectors.
		 */
		T AngleBetween(const Vector3<T>& other) const
		{
			return static_cast<T>(acos(Dot(other) / (Length() * other.Length())));
		}

		/**
		 * @brief Returns the distance between two vectors.
		 *
		 * @param other Vector to calculate the distance with.
		 * @return T Distance between two vectors.
		 */
		T DistanceBetween(const Vector3<T>& other) const
		{
			return static_cast<T>(sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2)));
		}
			
		/**
		 * @brief Converts the 4D vector to 3D vector.
		 * 
		 * @param other 4D vector to be converted.
		 * @return Vector3<T> Converted 3D vector.
		 */
		static Vector3<T> FromVector4(const Vector4<T>& other)
		{
			return Vector3<T>(other.x, other.y, other.z);
		}
    };

}
