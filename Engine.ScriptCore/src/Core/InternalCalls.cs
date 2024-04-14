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


		internal static delegate* unmanaged<ulong, bool> AssetHandle_IsValid;
		

		internal static delegate* unmanaged<ulong, ReflectionType, bool> Entity_HasComponent;
		internal static delegate* unmanaged<ulong, ReflectionType, void> Entity_AddComponent;
		internal static delegate* unmanaged<ulong, ReflectionType, void> Entity_RemoveComponent;


		internal static delegate* unmanaged<NativeString, ulong> Scene_CreateEntity;
		internal static delegate* unmanaged<ulong, ulong> Scene_TryGetEntityByID;
		internal static delegate* unmanaged<NativeString, ulong> Scene_TryGetEntityByTag;

		internal static delegate* unmanaged<ulong, ulong> Scene_InstantiatePrefab;
		internal static delegate* unmanaged<ulong, Vector3*, ulong> Scene_InstantiatePrefabWithPosition;
		internal static delegate* unmanaged<ulong, Vector3*, Vector3*, ulong> Scene_InstantiatePrefabWithPositionRotation;
		internal static delegate* unmanaged<ulong, Vector3*, Vector3*, Vector3*, ulong> Scene_InstantiatePrefabWithPositionRotationScale;


		internal static delegate* unmanaged<ulong, NativeString> TagComponent_GetTag;
		internal static delegate* unmanaged<ulong, NativeString, void> TagComponent_SetTag;


		internal static delegate* unmanaged<ulong, Vector3*, void> TransformComponent_GetPosition;
		internal static delegate* unmanaged<ulong, Vector3*, void> TransformComponent_SetPosition;

		internal static delegate* unmanaged<ulong, Vector3*, void> TransformComponent_GetRotation;
		internal static delegate* unmanaged<ulong, Vector3*, void> TransformComponent_SetRotation;

		internal static delegate* unmanaged<ulong, Vector3*, void> TransformComponent_GetScale;
		internal static delegate* unmanaged<ulong, Vector3*, void> TransformComponent_SetScale;


		internal static delegate* unmanaged<ulong, NativeString, void> AnimatedSpriteComponent_Play;
		internal static delegate* unmanaged<ulong, void> AnimatedSpriteComponent_Stop;


		internal static delegate* unmanaged<ulong, NativeString> TextComponent_GetText;
		internal static delegate* unmanaged<ulong, NativeString, void> TextComponent_SetText;

		internal static delegate* unmanaged<ulong, Vector4*, void> TextComponent_GetColor;
		internal static delegate* unmanaged<ulong, Vector4*, void> TextComponent_SetColor;

		internal static delegate* unmanaged<ulong, float> TextComponent_GetKerning;
		internal static delegate* unmanaged<ulong, float, void> TextComponent_SetKerning;

		internal static delegate* unmanaged<ulong, float> TextComponent_GetLineSpacing;
		internal static delegate* unmanaged<ulong, float, void> TextComponent_SetLineSpacing;

		internal static delegate* unmanaged<ulong, Vector2*, void> Rigidbody2DComponent_GetVelocity;
		internal static delegate* unmanaged<ulong, Vector2*, void> Rigidbody2DComponent_SetVelocity;

		internal static delegate* unmanaged<ulong, Vector2*, Vector2*, bool, void> RigidBody2DComponent_ApplyForce;
	}

#pragma warning restore CS0649
}
