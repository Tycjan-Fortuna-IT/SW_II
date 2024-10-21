/**
 * @file Vector2.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-01-20
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	/**
	 * @brief Vector2 is a structure representing mathematical vector in 2D space.
	 *
	 * @tparam T The type of the vector's components. This should be an arithmetic type.
	 */
	template <typename T>
	    requires std::is_arithmetic_v<T>
	struct Vector2 final
	{
		/**
		 * @brief Default constructor.
		 * @note Initializes the vector with zeros.
		 */
		Vector2() = default;

		/**
		 * @brief Constructs a new Vector2 object with both components (X and Y) set to the same value.
		 *
		 * @param x The value to set both components to.
		 */
		Vector2(T x)
		{
			data[0] = static_cast<T>(x);
			data[1] = static_cast<T>(x);
		}

		/**
		 * @brief Constructs a new Vector2 object with the specified components.
		 *
		 * @param x The x-coordinate of the vector.
		 * @param y The y-coordinate of the vector.
		 */
		Vector2(T x, T y)
		{
			data[0] = static_cast<T>(x);
			data[1] = static_cast<T>(y);
		}

		/**
		 * @brief Constructs a new Vector2 object from an initializer list.
		 *
		 * @param values An initializer list containing the vector's components. The behavior depends on the number of
		 * elements:
		 * - No elements: The vector will be filled with zeros.
		 * - One element: Both components will be set to this value.
		 * - Two elements: The components will be set to these values, in order.
		 */
		Vector2(std::initializer_list<T> values)
		{
			if (values.size() > 2)
			{
				SYSTEM_WARN("Too many arguments passed for the Vector2D constructor! Expected 0, 1 or 2, received: {}",
				            values.size());
			}

			auto it = values.begin();
			data[0] = it != values.end() ? *it++ : 0;
			data[1] = it != values.end() ? *it : data[0];
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
		 * @brief Destructor.
		 */
		~Vector2() = default;

		/**
		 * @brief The components of the vector, stored in an array.
		 * @note The components can be accessed using the names: (x, y), (r, g), (s, t), or (u, v).
		 */
		union {
			T data[2] = {static_cast<T>(0)};

			struct
			{
				union {
					T x, r, s, u;
				};
				union {
					T y, g, t, v;
				};
			};
		};

		/**
		 * @brief Adds two Vector2 objects.
		 *
		 * @param other The other Vector2 to add.
		 * @return Vector2<T> The result of the addition.
		 */
		Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(x + other.x, y + other.y); }

		/**
		 * @brief Adds a scalar value to this vector.
		 *
		 * @param other The scalar value to add.
		 * @return Vector2<T> The result of the addition.
		 */
		Vector2<T> operator+(const T& other) const { return Vector2<T>(x + other, y + other); }

		/**
		 * @brief Subtracts two Vector2 objects.
		 *
		 * @param other The other Vector2 to subtract.
		 * @return Vector2<T> The result of the subtraction.
		 */
		Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(x - other.x, y - other.y); }

		/**
		 * @brief Subtracts a scalar value from this vector.
		 *
		 * @param other The scalar value to subtract.
		 * @return Vector2<T> The result of the subtraction.
		 */
		Vector2<T> operator-(const T& other) const { return Vector2<T>(x - other, y - other); }

		/**
		 * @brief Multiplies two Vector2 objects.
		 *
		 * @param other The other Vector2 to multiply by.
		 * @return Vector2<T> The result of the multiplication.
		 */
		Vector2<T> operator*(const Vector2<T>& other) const { return Vector2<T>(x * other.x, y * other.y); }

		/**
		 * @brief Multiplies this vector by a scalar value.
		 *
		 * @param other The scalar value to multiply by.
		 * @return Vector2<T> The result of the multiplication.
		 */
		Vector2<T> operator*(const T& other) const { return Vector2<T>(x * other, y * other); }

		/**
		 * @brief Divides two Vector2 objects.
		 *
		 * @param other The other Vector2 to divide by.
		 * @return Vector2<T> The result of the division.
		 * @note If the other vector's x or y component is zero, an error message is logged and the division is not
		 * performed.
		 */
		Vector2<T> operator/(const Vector2<T>& other) const
		{
			if (other.x == static_cast<T>(0) || other.y == static_cast<T>(0))
			{
				SYSTEM_WARN("Cannot divide component of Vector2<T> by zero!");
				return *this;
			}

			return Vector2<T>(x / other.x, y / other.y);
		}

		/**
		 * @brief Divides this vector by a scalar value.
		 *
		 * @param other The scalar value to divide by.
		 * @return Vector2<T> The result of the division.
		 */
		Vector2<T> operator/(const T& other) const
		{
			if (other == static_cast<T>(0))
			{
				SYSTEM_WARN("Cannot divide component of Vector2<T> by zero!");
				return *this;
			}

			return Vector2<T>(x / other, y / other);
		}

		/**
		 * @brief Adds another Vector2 to this vector.
		 *
		 * @param other The other Vector2 to add.
		 * @return Vector2<T>& A reference to this vector after the addition.
		 */
		Vector2<T>& operator+=(const Vector2<T>& other)
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		/**
		 * @brief Adds a scalar value to this vector.
		 *
		 * @param other The scalar value to add.
		 * @return Vector2<T>& A reference to this vector after the addition.
		 */
		Vector2<T>& operator+=(const T& other)
		{
			x += other;
			y += other;

			return *this;
		}

		/**
		 * @brief Subtracts another Vector2 from this vector.
		 *
		 * @param other The other Vector2 to subtract.
		 * @return Vector2<T>& A reference to this vector after the subtraction.
		 */
		Vector2<T>& operator-=(const Vector2<T>& other)
		{
			x -= other.x;
			y -= other.y;

			return *this;
		}

		/**
		 * @brief Subtracts a scalar value from this vector.
		 *
		 * @param other The scalar value to subtract.
		 * @return Vector2<T>& A reference to this vector after the subtraction.
		 */
		Vector2<T>& operator-=(const T& other)
		{
			x -= other;
			y -= other;

			return *this;
		}

		/**
		 * @brief Multiplies this vector by another Vector2.
		 *
		 * @param other The other Vector2 to multiply by.
		 * @return Vector2<T>& A reference to this vector after the multiplication.
		 */
		Vector2<T>& operator*=(const Vector2<T>& other)
		{
			x *= other.x;
			y *= other.y;

			return *this;
		}

		/**
		 * @brief Multiplies this vector by a scalar value.
		 *
		 * @param other The scalar value to multiply by.
		 * @return Vector2<T>& A reference to this vector after the multiplication.
		 */
		Vector2<T>& operator*=(const T& other)
		{
			x *= other;
			y *= other;

			return *this;
		}

		/**
		 * @brief Divides this vector by another Vector2.
		 * @note If the other vector's x or y component is zero, an error message is logged and the division is not
		 * performed.
		 *
		 * @param other The other Vector2 to divide by.
		 * @return Vector2<T>& A reference to this vector after the division.
		 */
		Vector2<T>& operator/=(const Vector2<T>& other)
		{
			if (other.x == static_cast<T>(0) || other.y == static_cast<T>(0))
			{
				SYSTEM_WARN("Cannot divide component of Vector2<T> by zero!");
				return *this;
			}

			x /= other.x;
			y /= other.y;

			return *this;
		}

		/**
		 * @brief Divides this vector by a scalar value.
		 *
		 * @param other The scalar value to divide by.
		 * @return Vector2<T>& A reference to this vector after the division.
		 */
		Vector2<T>& operator/=(const T& other)
		{
			if (other == static_cast<T>(0))
			{
				SYSTEM_WARN("Cannot divide component of Vector2<T> by zero!");
				return *this;
			}

			x /= other;
			y /= other;

			return *this;
		}

		/**
		 * @brief Checks if the current vector is equal to another vector.
		 *
		 * @param other The vector to compare against.
		 * @return True if the vectors are equal, false otherwise.
		 */
		bool operator==(const Vector2<T>& other) const { return x == other.x && y == other.y; }

		/**
		 * @brief Checks if the current vector is not equal to another vector.
		 *
		 * @param other The vector to compare against.
		 * @return True if the vectors are not equal, false otherwise.
		 */
		bool operator!=(const Vector2<T>& other) const { return !(x == other.x && y == other.y); }

		/**
		 * @brief Returns a string representation of the vector.
		 * @note The format: { x, y }, e.g. { 1.0, 2.0 }.
		 *
		 * @return std::string A string representation of the vector.
		 */
		std::string ToString() const
		{
			const std::string str = "{ " + std::to_string(x) + ", " + std::to_string(y) + " }";

			return str;
		}

		/**
		 * @brief Returns a pointer to the first component of the vector (to the beginning of the internal data array).
		 *
		 * @return const T* A pointer to the first component of the vector.
		 */
		T* ValuePtr() { return data; }

		/**
		 * @brief Returns a const pointer to the first component of the vector (to the beginning of the internal data
		 * array).
		 *
		 * @return const T* A pointer to the first component of the vector.
		 */
		const T* ValuePtrConst() const { return data; }

		/**
		 * @brief Calculates the length (magnitude) of the vector.
		 *
		 * @return T The length of the vector.
		 */
		T Length() const { return static_cast<T>(sqrt(x * x + y * y)); }

		/**
		 * @brief Normalizes the vector, making its length 1 while preserving its direction.
		 */
		void Normalize()
		{
			const T length = Length();

			if (length == static_cast<T>(0))
			{
				SYSTEM_WARN("Cant normalize Vector2<T> with length of 0!");
				return;
			}

			x /= length;
			y /= length;
		}

		/**
		 * @brief Returns a normalized version of the vector.
		 * @note This does not modify the original vector.
		 *
		 * @return Vector2<T> A normalized version of the vector.
		 */
		Vector2<T> Normalized()
		{
			const T length = Length();

			Vector2<T> result = *this;

			if (length == static_cast<T>(0))
			{
				SYSTEM_WARN("Cant normalize Vector2<T> with length of 0!");
				return result;
			}

			result.x /= length;
			result.y /= length;

			return result;
		}

		/**
		 * @brief Returns the dot product of two vectors.
		 *
		 * @param other Vector to calculate the dot product with.
		 * @return T Dot product of two vectors.
		 */
		T Dot(const Vector2<T>& other) const { return static_cast<T>(x * other.x + y * other.y); }
	};

} // namespace SW
