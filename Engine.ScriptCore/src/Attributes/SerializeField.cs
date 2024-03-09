using System;

namespace SW
{
	/// <summary>
	///		Tells engine to serialize the field and show inside the SW editor.
	/// </summary>
	[AttributeUsage(AttributeTargets.Field)]
	public class SerializeFieldAttribute : Attribute
	{
	
	}
}
