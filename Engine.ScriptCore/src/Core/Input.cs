namespace SW
{
	public class Input
	{
		/// <summary>
		///		Checks if the specified key is pressed. Only once per frame.
		/// </summary>
		/// <param name="keycode">The key to check.</param>
		/// <returns>True if the key is pressed, false otherwise.</returns>
		public static bool IsKeyPressed(KeyCode keycode)
		{
			unsafe {
				return InternalCalls.Input_IsKeyPressed(keycode);
			}
		}

		/// <summary>
		/// 	Checks if the specified key is held.
		/// </summary>
		/// <param name="keycode">The key to check.</param>
		/// <returns>True if the key is held, false otherwise.</returns>
		public static bool IsKeyHeld(KeyCode keycode)
		{
			unsafe {
				return InternalCalls.Input_IsKeyHeld(keycode);
			}
		}

		/// <summary>
		/// 	Checks if the specified key is down. Meaning it's being held or pressed.
		/// </summary>
		/// <param name="keycode">The key to check.</param>
		/// <returns>True if the key is down, false otherwise.</returns>
		public static bool IsKeyDown(KeyCode keycode)
		{
			unsafe {
				return InternalCalls.Input_IsKeyDown(keycode);
			}
		}

		/// <summary>
		/// 	Checks if the specified key is released. Only once per frame.
		/// </summary>
		/// <param name="keycode">The key to check.</param>
		/// <returns>True if the key is released, false otherwise.</returns>
		public static bool IsKeyReleased(KeyCode keycode)
		{
			unsafe {
				return InternalCalls.Input_IsKeyReleased(keycode);
			}
		}

		/// <summary>
		/// 	Checks if the specified mouse button is pressed. Only once per frame.
		/// </summary>
		/// <param name="button">The mouse button to check.</param>
		/// <returns>True if the mouse button is pressed, false otherwise.</returns>
		public static bool IsMouseButtonPressed(MouseCode button)
		{
			unsafe {
				return InternalCalls.Input_IsMouseButtonPressed(button);
			}
		}

		/// <summary>
		/// 	Checks if the specified mouse button is held.
		/// </summary>
		/// <param name="button">The mouse button to check.</param>
		/// <returns>True if the mouse button is held, false otherwise.</returns>
		public static bool IsMouseButtonHeld(MouseCode button)
		{
			unsafe {
				return InternalCalls.Input_IsMouseButtonHeld(button);
			}
		}

		/// <summary>
		/// 	Checks if the specified mouse button is down. Meaning it's being held or pressed.
		/// </summary>
		/// <param name="button">The mouse button to check.</param>
		/// <returns>True if the mouse button is down, false otherwise.</returns>
		public static bool IsMouseButtonDown(MouseCode button)
		{
			unsafe {
				return InternalCalls.Input_IsMouseButtonDown(button);
			}
		}

		/// <summary>
		/// 	Checks if the specified mouse button is released. Only once per frame.
		/// </summary>
		/// <param name="button">The mouse button to check.</param>
		/// <returns>True if the mouse button is released, false otherwise.</returns>
		public static bool IsMouseButtonReleased(MouseCode button)
		{
			unsafe {
				return InternalCalls.Input_IsMouseButtonReleased(button);
			}
		}

		/// <summary>
		/// 	Returns the mouse position in window space.
		/// </summary>
		/// <returns>The mouse position in window space.</returns>
		/// <remarks>
		/// 	Window space is the space where the origin is at the top-left corner of the window.
		/// 		- X increases from left to right.
		/// 	    - Y increases from top to bottom.
		/// </remarks>
		/// <seealso cref="GetViewportMousePosition"/>
		/// <returns>The mouse position in window space.</returns>
		public static Vector2 GetWindowMousePosition()
		{
			unsafe {
				Vector2 position;

				InternalCalls.Input_GetWindowMousePosition(&position);

				return position;
			}
		}

		/// <summary>
		/// 	Returns the mouse position in viewport space.
		/// </summary>
		/// <remarks>
		/// 	Viewport space is the space where the origin is at the top-left corner of the viewport.
		/// 		- X increases from left to right.
		/// 		- Y increases from top to bottom.
		/// </remarks>
		/// <seealso cref="GetWindowMousePosition"/>
		/// <returns>The mouse position in viewport space.</returns>
		public static Vector2 GetViewportMousePosition()
		{
			unsafe {
				Vector2 position;

				InternalCalls.Input_GetViewportMousePosition(&position);

				return position;
			}
		}
	}
}
