namespace SW
{
	public static class Math
	{
		/// <summary>
		/// 	Returns the smaller of two integers.
		/// </summary>
		/// <param name="a">The first integer.</param>
		/// <param name="b">The second integer.</param>
		/// <returns>The smaller of the two integers.</returns>
		public static int Min(int a, int b) => a < b ? a : b;

		/// <summary>
		/// 	Returns the larger of two integers.
		/// </summary>
		/// <param name="a">The first integer.</param>
		/// <param name="b">The second integer.</param>
		/// <returns>The larger of the two integers.</returns>
		public static int Max(int a, int b) => a > b ? a : b;

		/// <summary>
		/// 	Clamps a value between a minimum and maximum value.
		/// </summary>
		/// <param name="value">The value to clamp.</param>
		/// <param name="min">The minimum value.</param>
		/// <param name="max">The maximum value.</param>
		/// <returns>The clamped value.</returns>
		public static int Clamp(int value, int min, int max) => Min(Max(value, min), max);

		/// <summary>
		/// 	Returns the absolute value of an integer.
		/// </summary>
		/// <param name="value">The integer value.</param>
		/// <returns>The absolute value of the integer.</returns>
		public static int Abs(int value) => value < 0 ? -value : value;

		/// <summary>
		/// 	Returns the sign of an integer.
		/// </summary>
		/// <param name="value">The integer value.</param>
		/// <returns>The sign of the integer (-1 if negative, 1 if positive).</returns>
		public static int Sign(int value) => value < 0 ? -1 : 1;

		/// <summary>
		/// 	Returns the result of raising an integer to a power.
		/// </summary>
		/// <param name="value">The base value.</param>
		/// <param name="power">The exponent value.</param>
		/// <returns>The result of raising the base value to the power.</returns>
		public static int Pow(int value, int power) => (int)System.Math.Pow(value, power);

		/// <summary>
		/// 	Returns the square root of an integer.
		/// </summary>
		/// <param name="value">The integer value.</param>
		/// <returns>The square root of the integer.</returns>
		public static int Sqrt(int value) => (int)System.Math.Sqrt(value);
	}
}
