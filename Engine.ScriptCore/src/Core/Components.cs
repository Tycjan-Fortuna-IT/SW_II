namespace SW
{
	public abstract class Component
	{
		public Entity Entity { get; set; } = default!;
	}

	/// <summary>
	/// 	Represents a component that contains a tag associated with the entity.
	/// </summary>
	public class TagComponent : Component
	{
		/// <summary>
		/// 	Gets or sets the tag associated with the entity.
		/// </summary>
		public string Tag
		{
			get {
				unsafe {
					return InternalCalls.TagComponent_GetTag(Entity.GetID())!;
				}
			}
			set {
				unsafe {
					InternalCalls.TagComponent_SetTag(Entity.GetID(), value);
				}
			}
		}
	}

	/// <summary>
	/// 	Represents a component that defines the position, rotation, and scale of an entity in 3D space.
	/// </summary>
	public class TransformComponent : Component
	{
		/// <summary>
		/// 	Gets or sets the position of the entity in 3D space.
		/// </summary>
		public Vector3 Position
		{
			get {
				Vector3 result;

				unsafe {
					InternalCalls.TransformComponent_GetPosition(Entity.GetID(), &result);
				}

				return result;
			}
			set {
				unsafe {
					InternalCalls.TransformComponent_SetPosition(Entity.GetID(), &value);
				}
			}
		}

		/// <summary>
		/// 	Gets or sets the rotation of the entity in 3D space.
		/// </summary>
		public Vector3 Rotation
		{
			get {
				Vector3 result;

				unsafe {
					InternalCalls.TransformComponent_GetRotation(Entity.GetID(), &result);
				}

				return result;
			}
			set {
				unsafe {
					InternalCalls.TransformComponent_SetRotation(Entity.GetID(), &value);
				}
			}
		}

		/// <summary>
		/// 	Gets or sets the scale of the entity in 3D space.
		/// </summary>
		public Vector3 Scale
		{
			get {
				Vector3 result;

				unsafe {
					InternalCalls.TransformComponent_GetScale(Entity.GetID(), &result);
				}

				return result;
			}
			set {
				unsafe {
					InternalCalls.TransformComponent_SetScale(Entity.GetID(), &value);
				}
			}
		}
	}

	/// <summary>
	///		Represents a component that allows playing and stopping animations for a sprite.
	/// </summary>
	public class AnimatedSpriteComponent : Component
	{
		/// <summary>
		///		Plays the animation with the specified name.
		/// </summary>
		/// <param name="name">The name of the animation to play.</param>
		public void Play(string name)
		{
			unsafe {
				InternalCalls.AnimatedSpriteComponent_Play(Entity.GetID(), name);
			}
		}

		/// <summary>
		///		Stops the currently playing animation.
		/// </summary>
		public void Stop()
		{
			unsafe {
				InternalCalls.AnimatedSpriteComponent_Stop(Entity.GetID());
			}
		}
	}

	/// <summary>
	/// 	Represents a component that adds 2D rigid body physics to an entity.
	/// </summary>
	public class RigidBody2DComponent : Component
	{
		/// <summary>
		///		Linear velocity in units per second.
		/// </summary>
		public Vector2 Velocity {
			get {
				unsafe {
					Vector2 velocity;

					InternalCalls.Rigidbody2DComponent_GetVelocity(Entity.GetID(), &velocity);

					return velocity;
				}
			}
			set {
				unsafe {
					InternalCalls.Rigidbody2DComponent_SetVelocity(Entity.GetID(), &value);
				}
			}
		}

		/// <summary>
		/// 	Applies a force to the center of the body.
		/// </summary>
		/// <param name="force">The force to apply.</param>
		/// <param name="wake">Determines whether to wake up the body if it is sleeping (default is true).</param>
		public void ApplyForce(Vector2 force, bool wake = true)
		{
			ApplyForce(force, new Vector2(0.0f, 0.0f), wake);
		}

		/// <summary>
		/// 	Applies a force to a specific offset from the center of the body.
		/// </summary>
		/// <param name="force">The force to apply.</param>
		/// <param name="offset">The offset from the center of the body where the force should be applied.</param>
		/// <param name="wake">Determines whether to wake up the body if it is sleeping (default is true).</param>
		public void ApplyForce(Vector2 force, Vector2 offset, bool wake = true)
		{
			unsafe {
				InternalCalls.RigidBody2DComponent_ApplyForce(Entity.GetID(), &force, &offset, wake);
			}
		}
	}
}
