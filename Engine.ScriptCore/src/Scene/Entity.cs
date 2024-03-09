using System;

namespace SW
{
	public class Entity : IEquatable<Entity>
	{
		protected virtual void OnCreate() { }

		protected virtual void OnUpdate(float ts) { }
		
		protected virtual void OnLateUpdate(float ts) { }
		
		protected virtual void OnPhysicsUpdate(float ts) { }
		
		protected virtual void OnDestroy() { }

		private readonly ulong ID = 0;

		public bool Equals(Entity other)
		{
			if (ReferenceEquals(this, other))
				return true;

			return ID == other.ID;
		}
	}
}
