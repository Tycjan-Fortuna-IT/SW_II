using System.Diagnostics;
using System.Runtime.InteropServices;
using System;

namespace SW
{
	/// <summary>
	/// 	Represents a 3D vector with X, Y and Z components.
	/// </summary>
	[DebuggerDisplay("[{x}, {y}, {z}]")]
	[StructLayout(LayoutKind.Sequential)]
	public struct Vector3 : IEquatable<Vector3>
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
		/// 	Initializes a new instance of the <see cref="Vector3"/> class with all components set to the specified value.
		/// </summary>
		/// <param name="value">The value to set all components to.</param>
		public Vector3(float value) => X = Y = Z = value;

		/// <summary>
		/// 	Initializes a new instance of the <see cref="Vector3"/> class with the specified components.
		/// </summary>
		/// <param name="x">The X component of the vector.</param>
		/// <param name="y">The Y component of the vector.</param>
		/// <param name="z">The Z component of the vector.</param>
		public Vector3(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}

		/// <summary>
		/// 	Gets a vector with all components set to zero.
		/// </summary>
		public static readonly Vector3 ZeroVector = new Vector3(0f, 0f, 0f);

		/// <summary>
		/// 	Gets a vector with all components set to one.
		/// </summary>
		public static readonly Vector3 OneVector = new Vector3(1f, 1f, 1f);

		/// <summary>
		/// C	alculates the length of the vector.
		/// </summary>
		/// <returns>The length of the vector.</returns>
		public float Length() => (float)Math.Sqrt(X * X + Y * Y + Z * Z);

		/// <summary>
		/// 	Determines whether this vector is equal to another vector.
		/// </summary>
		/// <param name="other">The vector to compare with this vector.</param>
		/// <returns><c>true</c> if the vectors are equal; otherwise, <c>false</c>.</returns>
		public bool Equals(Vector3 other) => X == other.X && Y == other.Y && Z == other.Z;

		public static Vector3 operator+(in Vector3 left, in Vector3 right) => new Vector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
	}
}
