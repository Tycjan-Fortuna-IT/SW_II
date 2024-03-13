namespace SW
{
	public static class Scene
	{
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
	}
}
