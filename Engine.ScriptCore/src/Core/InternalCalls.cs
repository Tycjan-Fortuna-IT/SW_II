using Coral.Managed.Interop;

namespace SW
{
#pragma warning disable CS0649 // Variable is never assigned to

	internal static unsafe class InternalCalls
	{
		#region Application
		
		internal static delegate* unmanaged<uint> Application_GetVieportWidth;
		internal static delegate* unmanaged<uint> Application_GetVieportHeight;
		internal static delegate* unmanaged<void> Application_Shutdown;

		#endregion

		#region Entity

		internal static delegate* unmanaged<ulong, ReflectionType, bool> Entity_HasComponent;

		#endregion
	}

#pragma warning restore CS0649
}
