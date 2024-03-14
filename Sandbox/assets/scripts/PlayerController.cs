using SW;

namespace Sandbox
{
	internal class PlayerController : Entity
	{
		[SerializeField]
		private float Speed = 12.0f;

		private TransformComponent Transform = default!;
		private RigidBody2DComponent Body = default!;

		protected override void OnCreate()
		{
			Transform = GetComponent<TransformComponent>()!;
			Body = GetComponent<RigidBody2DComponent>()!;
		}

		protected override void OnUpdate(float ts)
		{
			if (Input.IsKeyDown(KeyCode.A)) {
				Body.ApplyForce(new Vector2(-Speed, 0.0f));
			}

			if (Input.IsKeyDown(KeyCode.D)) {
				Body.ApplyForce(new Vector2(Speed, 0.0f));
			}

			if (Input.IsKeyPressed(KeyCode.Space)) {
				Body.ApplyForce(new Vector2(0.0f, 250.0f));
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
