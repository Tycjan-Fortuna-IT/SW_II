using System.Diagnostics;
using System.Runtime.InteropServices;
using System;

namespace SW
{
	/// <summary>
	/// 	Represents a 4D vector with X, Y, Z and W components.
	/// </summary>
	[DebuggerDisplay("[{x}, {y}, {z}, {w}]")]
	[StructLayout(LayoutKind.Sequential)]
	public struct Vector4 : IEquatable<Vector4>
	{
		/// <summary>
		/// 	The X component of the vector.
		/// </summary>
		public float X = 0.0f;

		/// <summary>
		/// 	The Y component of the vector.
		/// </summary>
		public float Y = 0.0f;

		/// <summary>
		/// 	The Z component of the vector.
		/// </summary>
		public float Z = 0.0f;

		/// <summary>
		/// 	The W component of the vector.
		/// </summary>
		public float W = 0.0f;

		/// <summary>
		/// 	Initializes a new instance of the <see cref="Vector4"/> class with all components set to the specified value.
		/// </summary>
		/// <param name="value">The value to set all components to.</param>
		public Vector4(float value) => X = Y = Z = W = value;

		/// <summary>
		/// 	Initializes a new instance of the <see cref="Vector4"/> class with the specified component values.
		/// </summary>
		/// <param name="x">The X component of the vector.</param>
		/// <param name="y">The Y component of the vector.</param>
		/// <param name="z">The Z component of the vector.</param>
		/// <param name="w">The W component of the vector.</param>
		public Vector4(float x, float y, float z, float w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

		/// <summary>
		/// 	Gets a vector with all components set to zero.
		/// </summary>
		public static readonly Vector4 ZeroVector = new Vector4(0f, 0f, 0f, 0f);

		/// <summary>
		/// 	Gets a vector with all components set to one.
		/// </summary>
		public static readonly Vector4 OneVector = new Vector4(1f, 1f, 1f, 1f);

		/// <summary>
		/// 	Calculates the length of the vector.
		/// </summary>
		/// <returns>The length of the vector.</returns>
		public float Length() => Mathf.Sqrt(X * X + Y * Y + Z * Z + W * W);

		/// <summary>
		/// 	Determines whether this vector is equal to another vector.
		/// </summary>
		/// <param name="other">The vector to compare with this vector.</param>
		/// <returns><c>true</c> if the vectors are equal; otherwise, <c>false</c>.</returns>
		public bool Equals(Vector4 other) => X == other.X && Y == other.Y && Z == other.Z && W == other.W;
	}
}
