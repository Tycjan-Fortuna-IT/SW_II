using System;
using System.Collections.Generic;

namespace SW
{
	public class Entity : IEquatable<Entity>
	{
		private readonly ulong ID = 0;

		private Dictionary<Type, Component> m_ComponentCache = new Dictionary<Type, Component>();

		internal Entity(ulong id)
		{
			ID = id;
		}

		protected Entity() { ID = 0; }

		/// <summary>
		///		Called when the entity is created.
		/// </summary>
		protected virtual void OnCreate() { }

		/// <summary>
		///		Called when the entity is updated.
		/// </summary>
		/// <param name="ts">The time since the last update.</param>
		protected virtual void OnUpdate(float ts) { }

		/// <summary>
		///		Called when the entity is updated after all other entities have been updated.
		/// </summary>
		/// <param name="ts">The time since the last update.</param>
		protected virtual void OnLateUpdate(float ts) { }

		/// <summary>
		///		Called when the entity is destroyed.
		/// </summary>
		protected virtual void OnDestroy() { }

		/// <summary>
		///		Checks if the entity has a component of the specified type.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		/// <returns>True if the entity has the component, false otherwise.</returns>
		public bool HasComponent<T>()
			where T : Component
		{
			unsafe { return InternalCalls.Entity_HasComponent(ID, typeof(T)); }
		}

		//public T? GetComponent<T>()
		//	where T : Component
		//{

		//}

		public bool Equals(Entity other)
		{
			if (ReferenceEquals(this, other))
				return true;

			return ID == other.ID;
		}
	}
}
