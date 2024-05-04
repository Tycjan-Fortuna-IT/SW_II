using System;
using System.Collections.Generic;

namespace SW
{
	[EditorAssignable]
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
		/// 	Destroys the entity.
		/// </summary>
		public void Destroy() => Scene.DestroyEntity(this);

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
		///		Called when the entity is destroyed when the scene is being unloaded.
		/// </summary>
		protected virtual void OnCleanup() { }

		public ulong GetID() { return ID; }

		/// <summary>
		/// 	Event triggered when a 2D collision with another entity begins.
		/// </summary>
		/// <remarks>
		/// 	The event handler receives the entity that this entity started colliding with.
		/// </remarks>
		public event Action<Entity>? OnCollision2DBegin;

		/// <summary>
		/// Event triggered when a 2D collision with another entity ends.
		/// </summary>
		/// <remarks>
		/// 	The event handler receives the entity that this entity stopped colliding with.
		/// </remarks>
		public event Action<Entity>? OnCollision2DEnd;

		private void OnCollision2DBeginInternal(ulong id) => OnCollision2DBegin?.Invoke(new Entity(id));
		
		private void OnCollision2DEndInternal(ulong id) => OnCollision2DEnd?.Invoke(new Entity(id));

		/// <summary>
		///		Checks if this entity is a script entity of type T
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns>True if this entity is a script entity of type T</returns>
		public bool Is<T>() where T : Entity
		{
			ScriptComponent? sc = GetComponent<ScriptComponent>();

			return sc?.Instance.Get() is T;
		}

		/// <summary>
		///		Returns the script instance as type T if this entity is of the given type, otherwise null
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		public T? As<T>() where T : Entity
		{
			ScriptComponent? sc = GetComponent<ScriptComponent>();
			
			return sc?.Instance.Get() as T;
		}

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

		public T? GetComponent<T>()
			where T : Component, new()
		{
			Type type = typeof(T);

			if (!HasComponent<T>()) {
				m_ComponentCache.Remove(type);

				return null;
			}

			if (!m_ComponentCache.ContainsKey(type)) {
				T comp = new T { Entity = this };

				m_ComponentCache[type] = comp;

				return comp;
			}

			return m_ComponentCache[type] as T;
		}

		public T? AddComponent<T>()
			where T : Component, new()
		{
			if (HasComponent<T>())
				return GetComponent<T>();

			unsafe {
				InternalCalls.Entity_AddComponent(ID, typeof(T));
			}

			T comp = new T { Entity = this };

			m_ComponentCache[typeof(T)] = comp;

			return comp;
		}

		public void RemoveComponent<T>()
			where T : Component
		{
			Type type = typeof(T);

			m_ComponentCache.Remove(type);
			
			if (!HasComponent<T>()) {
				return;
			}

			unsafe {
				InternalCalls.Entity_RemoveComponent(ID, typeof(T));
			}
		}

		public bool Equals(Entity? other)
		{
			if (other == null)
				return false;

			if (ReferenceEquals(this, other))
				return true;

			return ID == other.ID;
		}
	}
}
