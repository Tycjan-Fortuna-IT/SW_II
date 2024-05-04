using Coral.Managed.Interop;

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
		private string CurrentAnimation = string.Empty;

		/// <summary>
		///		Plays the animation with the specified name.
		/// </summary>
		/// <param name="name">The name of the animation to play.</param>
		public void Play(string name)
		{
			unsafe {
				if (CurrentAnimation == name)
					return;
					
				InternalCalls.AnimatedSpriteComponent_Play(Entity.GetID(), name);

				CurrentAnimation = name;
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
	/// 	Represents a component that displays a text.
	/// </summary>
	public class TextComponent : Component
	{
		/// <summary>
		/// 	Gets or sets the text content of the component.
		/// </summary>
		public string Text
		{
			get {
				unsafe {
					return InternalCalls.TextComponent_GetText(Entity.GetID())!;
				}
			}
			set {
				unsafe {
					InternalCalls.TextComponent_SetText(Entity.GetID(), value);
				}
			}
		}

		/// <summary>
		/// 	Gets or sets the color of the text.
		/// 	E.g. new Vector4(1.0f, 0.0f, 0.0f, 1.0f) is red.
		/// </summary>
		public Vector4 Color
		{
			get {
				Vector4 result;

				unsafe {
					InternalCalls.TextComponent_GetColor(Entity.GetID(), &result);
				}

				return result;
			}
			set {
				unsafe {
					InternalCalls.TextComponent_SetColor(Entity.GetID(), &value);
				}
			}
		}

		/// <summary>
		/// 	Gets or sets the kerning value of the text.
		/// 	Basically, it's the space between characters.
		/// </summary>
		public float Kerning
		{
			get {
				unsafe {
					return InternalCalls.TextComponent_GetKerning(Entity.GetID());
				}
			}
			set {
				unsafe {
					InternalCalls.TextComponent_SetKerning(Entity.GetID(), value);
				}
			}
		}

		/// <summary>
		/// 	Gets or sets the line spacing value of the text.
		/// 	Basically, it's the space between lines.
		/// </summary>
		public float LineSpacing
		{
			get {
				unsafe {
					return InternalCalls.TextComponent_GetLineSpacing(Entity.GetID());
				}
			}
			set {
				unsafe {
					InternalCalls.TextComponent_SetLineSpacing(Entity.GetID(), value);
				}
			}
		}
	}

	/// <summary>
	/// Represents a script component that can be attached to an entity.
	/// </summary>
	public class ScriptComponent : Component
	{
		/// <summary>
		/// Gets the native instance associated with this script component.
		/// </summary>
		public NativeInstance<object> Instance
		{
			get {
				unsafe { return InternalCalls.ScriptComponent_GetInstance(Entity.GetID()); }
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
