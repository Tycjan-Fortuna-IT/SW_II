namespace SW
{
	public static class Game
	{
		/// <summary>
		/// 	Gets the current width of the game viewport. (window)
		/// </summary>
		/// <returns>The width of the game viewport.</returns>
		public static uint GetVieportWidth()
		{
			unsafe {
				return InternalCalls.Application_GetVieportWidth();
			}
		}

		/// <summary>
		/// 	Gets the current height of the game viewport. (window)
		/// </summary>
		/// <returns>The height of the game viewport.</returns>
		public static uint GetVieportHeight()
		{
			unsafe {
				return InternalCalls.Application_GetVieportHeight();
			}
		}

		/// <summary>
		/// 	Shuts down the game.
		/// </summary>
		public static void ShutDown()
		{
			unsafe {
				InternalCalls.Application_Shutdown();
			}
		}
	}
}
