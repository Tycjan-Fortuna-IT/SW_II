using System;
using System.Security.Cryptography;

namespace SW
{
	/// <summary>
	///		Random number generator based on the implementation from: 
	///		https://codereview.stackexchange.com/questions/273390/c-prng-class-based-on-xoshiro256
	/// </summary>
	public static class Random
	{
		private static UInt64[] state = default!;  /// The current state of the generator, from which new bytes are generated.

		private const double INCR_DOUBLE = 1.0 / (1UL << 53);
		private const float INCR_FLOAT = 1f / (1U << 24);

		/// <summary>
		///		Seed the instance with a cryptographically-secure seed from the operating system for hard-to-guess sequences of pseudo-random numbers.
		/// </summary>
		static Random()
		{
			// Use the cryptography class to generate a good, random seed (ultimately from the OS's implementation of hardware seeding).
			RandomNumberGenerator rand = RandomNumberGenerator.Create();

			// Pull the next 64 bits (8 bytes) into an array.
			byte[] randBytes = new byte[8];
			rand.GetBytes(randBytes, 0, 8);

			// Convert the 64 bits into a single 64-bit unsigned int and return it.
			UInt64 seed = BitConverter.ToUInt64(randBytes, 0);

			// Initialize state using the generated seed. 
			xorshift256_init(seed);
		}

		/// <summary>
		///		Helper function for xorshift256_init, mixes up seed values to get a pseudo-random starting state.
		/// </summary>
		/// <param name="partialstate">The state portion to splitmix.</param>
		/// <returns>The splitmixed state portion.</returns>
		private static UInt64 splitmix64(UInt64 partialstate)
		{
			partialstate = partialstate + 0x9E3779B97f4A7C15;
			partialstate = (partialstate ^ (partialstate >> 30)) * 0xBF58476D1CE4E5B9;
			partialstate = (partialstate ^ (partialstate >> 27)) * 0x94D049BB133111EB;

			return partialstate ^ (partialstate >> 31);
		}

		/// <summary>
		///		Initialize the generator's state with the given seed value.
		/// </summary>
		/// <param name="seed">A 64-bit seed with which to seed this instance of the class.</param>
		private static void xorshift256_init(UInt64 seed)
		{
			UInt64[] result = new UInt64[4];

			result[0] = splitmix64(seed);
			result[1] = splitmix64(result[0]);
			result[2] = splitmix64(result[2]);
			result[3] = splitmix64(result[3]);

			state = result;
		}

		/// <summary>
		///		Helper function for xoshiro256ss, rotates some bits around.
		/// </summary>
		/// <param name="x">The state portion being modified.</param>
		/// <param name="k">The number of bits to shift around.</param>
		/// <returns>The modified state portion.</returns>
		private static UInt64 rotl64(UInt64 x, int k)
		{
			return (x << k) | (x >> (64 - k));
		}

		/// <summary>
		///		Generates the next 64-bit integer in the sequence, then updates the state to be ready for another call.
		///		(Star Star), xoshiro256**
		/// </summary>
		/// <returns>The next 64-bit integer in the sequence.</returns>
		private static UInt64 xoshiro256ss()
		{
			UInt64 result = rotl64(state[1] * 5, 7) * 9;
			UInt64 t = state[1] << 17;

			state[2] ^= state[0];
			state[3] ^= state[1];
			state[1] ^= state[2];
			state[0] ^= state[3];

			state[2] ^= t;
			state[3] = rotl64(state[3], 45);

			return result;
		}

		/// <summary>
		///		Get the next value in the sequence as an array of 8 bytes.
		/// </summary>
		/// <returns>The next 8 bytes in the sequence.</returns>
		public static byte[] Bytes()
		{
			UInt64 nextInt64 = xoshiro256ss();

			// Get and return the bytes of the 64-bit value (there will be 8 bytes).
			return BitConverter.GetBytes(nextInt64);
		}

		/// <summary>
		///		Get the next value in the sequence as a 64-bit unsigned integer.
		/// </summary>
		/// <returns>The next 64-bit unsigned integer in the sequence.</returns>
		public static UInt64 UInt64()
		{
			return xoshiro256ss();
		}

		/// <summary>
		///		Get the next value in the sequence as a 32-bit unsigned integer.
		/// </summary>
		/// <returns>The next 32-bit unsigned integer in the sequence.</returns>
		public static uint UInt()
		{
			// Get the next 8 bytes in the sequence.
			byte[] randBytes = Bytes();

			// Use the first 4 bytes to generate and return a 32-bit value.
			return BitConverter.ToUInt32(randBytes, 0);
		}

        /// <summary>
        /// 	Generates a pseudo-random integer.
        /// </summary>
        /// <returns>
        /// 	A 32-bit signed integer that is greater than or equal to 0 and less than Int32.MaxValue.
        /// 	Note: The range of return values includes 0 but not Int32.MaxValue.
        /// </returns>
        public static int Int()
        {
            return (int)UInt();
        }

        /// <summary>
        ///		Generates a pseudo-random float.
        /// </summary>
        /// <returns>
        /// 	A single-precision floating point number that is greater than or equal to 0 and less than 1.
        /// 	Note: The range of return values includes 0 but not 1.
        /// </returns>
        public static float Float()
        {
            return (UInt64() >> 40) * INCR_FLOAT;
        }

        /// <summary>
        /// 	Generates a pseudo-random double.
        /// </summary>
        /// <returns>
		/// 	A double-precision floating point number that is greater than or equal to 0 and less than 1.
		/// 	Note: The range of return values includes 0 but not 1.
        /// </returns>
        public static double Double()
        {
            return (UInt64() >> 11) * INCR_DOUBLE;
        }

        /// <summary>
        ///		Generates a pseudo-random integer within a specified range.
        /// </summary>
        /// <param name="min">The inclusive lower bound of the random number returned.</param>
        /// <param name="max">The inclusive upper bound of the random number returned. max must be greater than or equal to min.</param>
        /// <returns>
        ///		A 32-bit signed integer greater than or equal to min and less than or equal to max; that is, the range of return values includes both min and max.
        /// </returns>
        public static int Range(int min, int max)
        {
            return min + (int)(UInt() % ((max - min) + 1));
        }

        /// <summary>
        ///		Generates a pseudo-random float within a specified range.
        /// </summary>
        /// <param name="min">The inclusive lower bound of the random number returned.</param>
        /// <param name="max">The inclusive upper bound of the random number returned. max must be greater than or equal to min.</param>
        /// <returns>
        ///		A single-precision floating point number greater than or equal to min and less than or equal to max; that is, the range of return values includes both min and max.
        /// </returns>
        public static float RangeF(float min, float max)
        {
            return min + (Float() * ((max - min) + float.Epsilon));
        }
	}
}

