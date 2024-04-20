using SW;

namespace BumCatcher
{
	internal class Rotor : Entity
	{
		private TransformComponent Transform = default!;

		protected override void OnCreate()
		{
			Transform = GetComponent<TransformComponent>()!;
		}

		protected override void OnUpdate(float ts)
		{
			Vector3 rot = Transform.Rotation;

			Transform.Rotation = new Vector3(rot.X, rot.Y, rot.Z + ts / 4);
		}
	}
}
