namespace SW
{
	public static class Scene
	{
		/// <summary>
		/// 	Creates a new entity.
		/// </summary>
		/// <param name="name">The name of the entity. (optional)</param>
		/// <returns>The newly created entity.</returns>
		public static Entity CreateEntity(string name = "Empty")
		{
			unsafe {
				return new Entity(InternalCalls.Scene_CreateEntity(name));
			}
		}

		/// <summary>
		/// 	Destroys the specified entity.
		/// </summary>
		/// <param name="entity">The entity to destroy.</param>
		public static void DestroyEntity(Entity entity)
		{
			unsafe {
				InternalCalls.Scene_DestroyEntity(entity.GetID());
			}
		}

		/// <summary>
		/// 	Retrieves the entity with the specified ID.
		/// </summary>
		/// <param name="id">The ID of the entity.</param>
		/// <returns>The entity with the specified ID.</returns>
		/// <exception cref="System.Exception">Thrown when the entity with the specified ID does not exist.</exception>
		/// <remarks>Entity is expected to exist (runtime error if it doesn't)!</remarks>
		public static Entity GetEntityByID(ulong id)
		{
			Entity? entity = TryGetEntityByID(id);

			if (entity == null) {
				Log.Error("Entity with ID: {0} not found", id);

				throw new System.Exception($"Entity with ID: {id} not found");
			} else {
				return entity;
			}
		}

		/// <summary>
		/// 	Tries to retrieve the entity with the specified ID.
		/// </summary>
		/// <param name="id">The ID of the entity.</param>
		/// <returns>The entity with the specified ID, or null if not found.</returns>
		/// <remarks>Entity is not expected to exist (null returned if it doesn't) - caller must check!</remarks>
		public static Entity? TryGetEntityByID(ulong id)
		{
			unsafe {
				ulong internalId = InternalCalls.Scene_TryGetEntityByID(id);

				return internalId == 0 ? null : new Entity(internalId);
			}
		}

		/// <summary>
		/// 	Retrieves the entity with the specified tag.
		/// </summary>
		/// <param name="tag">The tag of the entity.</param>
		/// <returns>The entity with the specified tag.</returns>
		/// <exception cref="System.Exception">Thrown when the entity with the specified tag does not exist.</exception>
		/// <remarks>Entity is expected to exist (runtime error if it doesn't)! If multiple entities have the same tag - the first found is returned.</remarks>
		public static Entity GetEntityByTag(string tag)
		{
			Entity? entity = TryGetEntityByTag(tag);

			if (entity == null) {
				Log.Error("Entity with tag: {0} not found", tag);

				throw new System.Exception($"Entity with tag: {tag} not found");
			} else {
				return entity;
			}
		}

		/// <summary>
		/// 	Tries to retrieve the entity with the specified tag.
		/// </summary>
		/// <param name="tag">The tag of the entity.</param>
		/// <returns>The entity with the specified tag, or null if not found.</returns>
		/// <remarks>Entity is not expected to exist (null returned if it doesn't) - caller must check! If multiple entities have the same tag - the first found is returned.</remarks>
		public static Entity? TryGetEntityByTag(string tag)
		{
			unsafe {
				ulong internalId = InternalCalls.Scene_TryGetEntityByTag(tag);

				return internalId == 0 ? null : new Entity(internalId);
			}
		}

		/// <summary>
		///		Instantiates a prefab in the scene.
		/// </summary>
		/// <param name="prefab">The prefab to instantiate.</param>
		/// <returns>An Entity representing the instantiated prefab, or null if instantiation failed.</returns>
		public static Entity? InstantiatePrefab(Prefab prefab)
		{
			unsafe {
				ulong entityID = InternalCalls.Scene_InstantiatePrefab(prefab.Handle);

				return entityID == 0 ? null : new Entity(entityID);
			}
		}

		/// <summary>
		///		Instantiates a prefab in the scene at a specific position.
		/// </summary>
		/// <param name="prefab">The prefab to instantiate.</param>
		/// <param name="position">The position to instantiate the prefab at.</param>
		/// <returns>An Entity representing the instantiated prefab, or null if instantiation failed.</returns>
		public static Entity? InstantiatePrefab(Prefab prefab, Vector3 position)
		{
			unsafe {
				ulong entityID = InternalCalls.Scene_InstantiatePrefabWithPosition(prefab.Handle, &position);

				return entityID == 0 ? null : new Entity(entityID);
			}
		}

		/// <summary>
		///		Instantiates a prefab in the scene at a specific position and rotation.
		/// </summary>
		/// <param name="prefab">The prefab to instantiate.</param>
		/// <param name="position">The position to instantiate the prefab at.</param>
		/// <param name="rotation">The rotation to instantiate the prefab with.</param>
		/// <returns>An Entity representing the instantiated prefab, or null if instantiation failed.</returns>
		public static Entity? InstantiatePrefab(Prefab prefab, Vector3 position, Vector3 rotation)
		{
			unsafe {
				ulong entityID = InternalCalls.Scene_InstantiatePrefabWithPositionRotation(prefab.Handle, &position, &rotation);

				return entityID == 0 ? null : new Entity(entityID);
			}
		}

		/// <summary>
		///		Instantiates a prefab in the scene at a specific position, rotation, and scale.
		/// </summary>
		/// <param name="prefab">The prefab to instantiate.</param>
		/// <param name="position">The position to instantiate the prefab at.</param>
		/// <param name="rotation">The rotation to instantiate the prefab with.</param>
		/// <param name="scale">The scale to instantiate the prefab with.</param>
		/// <returns>An Entity representing the instantiated prefab, or null if instantiation failed.</returns>
		public static Entity? InstantiatePrefab(Prefab prefab, Vector3 position, Vector3 rotation, Vector3 scale)
		{
			unsafe {
				ulong entityID = InternalCalls.Scene_InstantiatePrefabWithPositionRotationScale(prefab.Handle, &position, &rotation, &scale);

				return entityID == 0 ? null : new Entity(entityID);
			}
		}

		/// <summary>
		///		Instantiates a prefab in the scene with a specific transform.
		/// </summary>
		/// <param name="prefab">The prefab to instantiate.</param>
		/// <param name="transform">The transform to instantiate the prefab with.</param>
		/// <returns>An Entity representing the instantiated prefab, or null if instantiation failed.</returns>
		public static Entity? InstantiatePrefab(Prefab prefab, Transform transform)
		{
			unsafe {
				ulong entityID = InternalCalls.Scene_InstantiatePrefabWithPositionRotationScale(prefab.Handle, &transform.Position, &transform.Rotation, &transform.Scale);

				return entityID == 0 ? null : new Entity(entityID);
			}
		}
	}
}
