﻿namespace SW
{
	/// <summary>
	///		Prefab is a type of asset that represents a prefab file.
	///		Basically a prefab is a template for creating new objects.
	/// </summary>
	[EditorAssignable]
	public class Prefab : Asset<Prefab>
	{
		public static bool operator ==(Prefab prefabA, Prefab prefabB) => prefabA is null ? prefabB is null : prefabA.Equals(prefabB);
		public static bool operator !=(Prefab prefabA, Prefab prefabB) => !(prefabA == prefabB);

		public override bool Equals(object? obj)
		{
			if (obj is Prefab prefab) {
				return prefab == this;
			}

			return false;
		}

		public override int GetHashCode()
		{
			return base.GetHashCode();
		}
	}
}
