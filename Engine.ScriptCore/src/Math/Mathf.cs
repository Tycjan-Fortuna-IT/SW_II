namespace SW
{
	public static class Mathf
	{
		/// <summary>
		/// 	Returns the smaller of two floats.
		/// </summary>
		/// <param name="a">The first float.</param>
		/// <param name="b">The second float.</param>
		/// <returns>The smaller of the two floats.</returns>
		public static float Min(float a, float b) => a < b ? a : b;

		/// <summary>
		/// 	Returns the larger of two floats.
		/// </summary>
		/// <param name="a">The first float.</param>
		/// <param name="b">The second float.</param>
		/// <returns>The larger of the two floats.</returns>
		public static float Max(float a, float b) => a > b ? a : b;

		/// <summary>
		/// 	Clamps a value between a minimum and maximum value.
		/// </summary>
		/// <param name="value">The value to clamp.</param>
		/// <param name="min">The minimum value.</param>
		/// <param name="max">The maximum value.</param>
		/// <returns>The clamped value.</returns>
		public static float Clamp(float value, float min, float max) => Min(Max(value, min), max);

		/// <summary>
		/// 	Returns the absolute value of a float.
		/// </summary>
		/// <param name="value">The float value.</param>
		/// <returns>The absolute value of the float.</returns>
		public static float Abs(float value) => value < 0 ? -value : value;

		/// <summary>
		/// 	Returns the sign of an float.
		/// </summary>
		/// <param name="value">The float value.</param>
		/// <returns>The sign of the float (-1 if negative, 1 if positive).</returns>
		public static float Sign(float value) => value < 0 ? -1 : 1; 

		/// <summary>
		/// 	Returns the result of raising an float to a power.
		/// </summary>
		/// <param name="value">The base value.</param>
		/// <param name="power">The exponent value.</param>
		/// <returns>The result of raising the base value to the power.</returns>
		public static float Pow(float value, float power) => (float)System.Math.Pow(value, power);			

		/// <summary>
		/// 	Returns the square root of an float.
		/// </summary>
		/// <param name="value">The float value.</param>
		/// <returns>The square root of the float.</returns>
		public static float Sqrt(float value) => (float)System.Math.Sqrt(value);		

		/// <summary>
		/// 	Rounds a decimal value to the nearest float.
		/// </summary>
		/// <param name="value">The decimal value.</param>
		/// <returns>The rounded float value.</returns>
		public static float Round(float value) => (float)System.Math.Round(value);

		/// <summary>
		/// 	Returns the smallest float greater than or equal to a decimal value.
		/// </summary>
		/// <param name="value">The decimal value.</param>
		/// <returns>The smallest float greater than or equal to the decimal value.</returns>
		public static float Ceil(float value) => (float)System.Math.Ceiling(value);

		/// <summary>
		/// 	Returns the largest float less than or equal to a decimal value.
		/// </summary>
		/// <param name="value">The decimal value.</param>
		/// <returns>The largest float less than or equal to the decimal value.</returns>
		public static float Floor(float value) => (float)System.Math.Floor(value);
	}
}
