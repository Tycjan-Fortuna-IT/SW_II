using System;

namespace SW
{
	/// <summary>
	///		Tells engine that the value will be assigned through the editor.
	/// </summary>
	[AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = true)]
	internal class EditorAssignableAttribute : Attribute
	{ 

	}
}
