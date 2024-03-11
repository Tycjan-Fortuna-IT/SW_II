namespace SW
{
	public abstract class Component
	{
		public Entity Entity { get; set; } = default!;
	}

	public class TransformComponent : Component
	{
		public Vector3 Position {
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
	}
}
