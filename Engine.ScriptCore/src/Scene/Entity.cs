using System;

namespace SW
{
	public class Entity : IEquatable<Entity>
	{
		internal Entity(ulong id)
		{
			ID = id;
		}

		protected Entity() { ID = 0; }

		private readonly ulong ID = 0;

		protected virtual void OnCreate() { }

		protected virtual void OnUpdate(float ts) { }
		
		protected virtual void OnLateUpdate(float ts) { }
				
		protected virtual void OnDestroy() { }

		public bool Equals(Entity other)
		{
			if (ReferenceEquals(this, other))
				return true;

			return ID == other.ID;
		}
	}
}
