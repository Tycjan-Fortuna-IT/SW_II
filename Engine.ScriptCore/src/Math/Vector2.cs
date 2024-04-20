using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace SW
{
	/// <summary>
	///		Represents a 2D vector with X and Y components.
	/// </summary>
	[DebuggerDisplay("[{x}, {y}]")]
	[StructLayout(LayoutKind.Sequential)]
	public struct Vector2 : IEquatable<Vector2>
	{
		/// <summary>
		///		The X component of the vector.
		/// </summary>
		public float X = 0.0f;

		/// <summary>
		///		The Y component of the vector.
		/// </summary>
		public float Y = 0.0f;

		/// <summary>
		///		Initializes a new instance of the <see cref="Vector2"/> class with the specified value for both X and Y components.
		/// </summary>
		/// <param name="value">The value to set for both X and Y components.</param>
		public Vector2(float value) => X = Y = value;

		/// <summary>
		///		Initializes a new instance of the <see cref="Vector2"/> class with the specified values for X and Y components.
		/// </summary>
		/// <param name="x">The value for the X component.</param>
		/// <param name="y">The value for the Y component.</param>
		public Vector2(float x, float y)
		{
			X = x;
			Y = y;
		}

		/// <summary>
		///		Gets a vector with both X and Y components set to zero.
		/// </summary>
		public static readonly Vector2 ZeroVector = new Vector2(0f, 0f);

		/// <summary>
		///		Gets a vector with both X and Y components set to one.
		/// </summary>
		public static readonly Vector2 OneVector = new Vector2(1f, 1f);

		/// <summary>
		///		Calculates the length of the vector.
		/// </summary>
		/// <returns>The length of the vector.</returns>
		public float Length() => Mathf.Sqrt(X * X + Y * Y);

		/// <summary>
		///		Determines whether this vector is equal to another vector.
		/// </summary>
		/// <param name="other">The vector to compare with this vector.</param>
		/// <returns><c>true</c> if the vectors are equal; otherwise, <c>false</c>.</returns>
		public bool Equals(Vector2 other) => X == other.X && Y == other.Y;

		public static Vector2 operator+(in Vector2 left, in Vector2 right) => new Vector2(left.X + right.X, left.Y + right.Y);
		public static Vector2 operator-(in Vector2 left, in Vector2 right) => new Vector2(left.X - right.X, left.Y - right.Y);
		public static Vector2 operator*(in Vector2 left, in float scalar) => new Vector2(left.X * scalar, left.Y * scalar);
		public static Vector2 operator/(in Vector2 left, in float scalar) => new Vector2(left.X / scalar, left.Y / scalar);
	}
}
