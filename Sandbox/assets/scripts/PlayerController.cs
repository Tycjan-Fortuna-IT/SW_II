using SW;

namespace Sandbox
{
	internal class PlayerController : Entity
	{
		[SerializeField]
		private float Speed = 12.0f;

		private TransformComponent Transform = default!;
		private RigidBody2DComponent Body = default!;
		private AnimatedSpriteComponent AnimationComponent = default!;

		protected override void OnCreate()
		{
			Transform = GetComponent<TransformComponent>()!;
			Body = GetComponent<RigidBody2DComponent>()!;
			AnimationComponent = GetComponent<AnimatedSpriteComponent>()!;
		}

		protected override void OnUpdate(float ts)
		{
			if (Input.IsKeyPressed(KeyCode.A)) {
				AnimationComponent.Play("Walk_Left");
			}

			if (Input.IsKeyPressed(KeyCode.D)) {
				AnimationComponent.Play("Walk_Right");
			}

			if (Input.IsKeyReleased(KeyCode.A) || Input.IsKeyReleased(KeyCode.D))
				AnimationComponent.Stop();

			if (Input.IsKeyDown(KeyCode.A)) {
				Body.ApplyForce(new Vector2(-Speed, 0.0f));
			}

			if (Input.IsKeyDown(KeyCode.D)) {
				Body.ApplyForce(new Vector2(Speed, 0.0f));
			}

			//if (Input.IsKeyPressed(KeyCode.Space)) {
			//	Body.ApplyForce(new Vector2(0.0f, 250.0f));
			//}

			// // Play walk animation
			// if (Input.IsKeyDown(KeyCode.A) || Input.IsKeyDown(KeyCode.D)) {
			// 	var animComponent = GetComponent<AnimationComponent>()!;
			// 	animComponent.Play("walk_right");
			// } else {
			// 	var animComponent = GetComponent<AnimationComponent>()!;
			// 	animComponent.Play("idle");
			// }
		}

		protected override void OnLateUpdate(float ts)
		{

		}

		protected override void OnDestroy()
		{

		}
	}
}
