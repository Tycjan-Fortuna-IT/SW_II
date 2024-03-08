using System;
using System.Runtime.InteropServices;

namespace Scripting.Managed
{
    internal static unsafe class ScriptingHost
    {
		private static delegate*<string, void> s_ExceptionCallback;

		private static delegate*<string, void> s_MessageCallback;

		//[UnmanagedCallersOnly]
		//private static void Initialize(delegate*<string, void> messageCallback, delegate*<string, void> exceptionCallback)
		//{
		//	s_MessageCallback = messageCallback;
		//	s_ExceptionCallback = exceptionCallback;
		//}
		[UnmanagedCallersOnly]
		private static void Initialize()
		{
			Console.WriteLine("Test from C# .dll");
			//s_MessageCallback = messageCallback;
			//s_ExceptionCallback = exceptionCallback;
		}
	}
}
