using System.Reflection;
using System;
using System.Collections.Generic;

namespace SW
{
	public static class ReflectionHelper
	{
		internal static BindingFlags InstanceAll = BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.FlattenHierarchy;

		private static Dictionary<string, Assembly> loadedAssemblies = new Dictionary<string, Assembly>();

		/// <summary>
		/// 	Retrieves the specified assembly by name. If the assembly has already been loaded, it is returned from the cache.
		/// 	If the assembly has not been loaded, it is loaded and added to the cache.
		/// </summary>
		/// <param name="assemblyName">The name of the assembly to retrieve.</param>
		/// <returns>The specified assembly.</returns>
		public static Assembly GetAssembly(string assemblyName)
		{
			if (!loadedAssemblies.ContainsKey(assemblyName)) {
				Assembly assembly = Assembly.Load(assemblyName);

				loadedAssemblies[assemblyName] = assembly;

				return assembly;
			}

			return loadedAssemblies[assemblyName];
		}

		/// <summary>
		///		Checks if a field in a specified class has a specific attribute.
		/// </summary>
		/// <param name="assembly">The name of the assembly containing the class.</param>
		/// <param name="className">The name of the class.</param>
		/// <param name="fieldName">The name of the field.</param>
		/// <param name="attrName">The name of the attribute.</param>
		/// <returns>True if the field has the specified attribute, otherwise false.</returns>
		public static bool HasFieldAttribute(string assembly, string className, string fieldName, string attrName)
		{
			Assembly asm = Assembly.Load(assembly);

			Type? type = asm.GetType(className);

			if (type != null) {
				FieldInfo? fieldInfo = type.GetField(fieldName, InstanceAll);

				if (fieldInfo != null) {
					foreach (var attr in fieldInfo.CustomAttributes) {
						if (string.Equals(attr.AttributeType.Name, attrName)) return true;
					}
				} else {
					Console.WriteLine(fieldName + " field not found");
				}
			} else {
				Console.WriteLine(className + " type not found");
			}

			return false;
		}
	}
}
