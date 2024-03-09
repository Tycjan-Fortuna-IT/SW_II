using System.Reflection;
using System.Runtime.InteropServices;
using System;

namespace SW
{
	internal class ReflectionHelper
	{
		internal static BindingFlags InstanceAll = BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.FlattenHierarchy;

	}
}
