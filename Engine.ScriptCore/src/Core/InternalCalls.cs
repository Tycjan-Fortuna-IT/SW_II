using Coral.Managed.Interop;

namespace SW
{
#pragma warning disable CS0649 // Variable is never assigned to

	internal static unsafe class InternalCalls
	{	
		internal static delegate* unmanaged<uint> Application_GetVieportWidth;
		internal static delegate* unmanaged<uint> Application_GetVieportHeight;
		internal static delegate* unmanaged<void> Application_Shutdown;


		internal static delegate* unmanaged<KeyCode, bool> Input_IsKeyPressed;
		internal static delegate* unmanaged<KeyCode, bool> Input_IsKeyHeld;
		internal static delegate* unmanaged<KeyCode, bool> Input_IsKeyDown;
		internal static delegate* unmanaged<KeyCode, bool> Input_IsKeyReleased;

		internal static delegate* unmanaged<MouseCode, bool> Input_IsMouseButtonPressed;
		internal static delegate* unmanaged<MouseCode, bool> Input_IsMouseButtonHeld;
		internal static delegate* unmanaged<MouseCode, bool> Input_IsMouseButtonDown;
		internal static delegate* unmanaged<MouseCode, bool> Input_IsMouseButtonReleased;

		internal static delegate* unmanaged<Vector2*, void> Input_GetWindowMousePosition;
		internal static delegate* unmanaged<Vector2*, void> Input_GetViewportMousePosition;


		internal static delegate* unmanaged<NativeString, void> Log_TraceMessage;
		internal static delegate* unmanaged<NativeString, void> Log_InfoMessage;
		internal static delegate* unmanaged<NativeString, void> Log_DebugMessage;
		internal static delegate* unmanaged<NativeString, void> Log_WarnMessage;
		internal static delegate* unmanaged<NativeString, void> Log_ErrorMessage;


		internal static delegate* unmanaged<ulong, ReflectionType, bool> Entity_HasComponent;
		internal static delegate* unmanaged<ulong, ReflectionType, void> Entity_AddComponent;
		internal static delegate* unmanaged<ulong, ReflectionType, void> Entity_RemoveComponent;


		internal static delegate* unmanaged<ulong, Vector3*, void> TransformComponent_GetPosition;
		internal static delegate* unmanaged<ulong, Vector3*, void> TransformComponent_SetPosition;
	}

#pragma warning restore CS0649
}
