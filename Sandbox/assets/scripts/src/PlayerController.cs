using SW;

namespace Sandbox
{
	internal class PlayerController : Entity
	{
		[SerializeField]
		private float Velocity = 0.02f;

		private TransformComponent Transform;

		protected override void OnCreate()
		{
			Transform = GetComponent<TransformComponent>();
		}

		protected override void OnUpdate(float ts)
		{
			if (Input.IsKeyDown(KeyCode.A)) {
				Transform.Position += new Vector3(-Velocity, 0.0f, 0.0f);
			}

			if (Input.IsKeyDown(KeyCode.D)) {
				Transform.Position += new Vector3(Velocity, 0.0f, 0.0f);
			}

			if (Input.IsKeyDown(KeyCode.W)) {
				Transform.Position += new Vector3(0.0f, Velocity, 0.0f);
			}

			if (Input.IsKeyDown(KeyCode.S)) {
				Transform.Position += new Vector3(0.0f, -Velocity, 0.0f);
			}
		}

		protected override void OnLateUpdate(float ts)
		{

		}

		protected override void OnDestroy()
		{

		}
	}
}
