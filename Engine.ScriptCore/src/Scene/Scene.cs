using System.Collections.Generic;

namespace SW
{
	public static class Scene
	{
		private static Dictionary<ulong, Entity> s_EntityCache = new Dictionary<ulong, Entity>();

		public static Entity GetEntityByID(ulong id)
		{
			return new Entity(id);
		}

		public static Entity? TryGetEntityByID(ulong id)
		{
			return null;
		}

		public static Entity GetEntityByTag(string tag)
		{
			return new Entity(1);
		}

		public static Entity? TryGetEntityByTag(string tag)
		{
			return null;
		}
	}
}
