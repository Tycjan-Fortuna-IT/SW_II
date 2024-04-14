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

		private float Score = 0.0f;
		private TextComponent ScoreText = default!;

		[SerializeField]
		private Prefab BottlePrefab = default!;

		protected override void OnCreate()
		{
			Transform = GetComponent<TransformComponent>()!;
			Body = GetComponent<RigidBody2DComponent>()!;
			AnimationComponent = GetComponent<AnimatedSpriteComponent>()!;

			ScoreText = Scene.GetEntityByTag("ScoreEntity").GetComponent<TextComponent>()!;
		}

		protected override void OnUpdate(float ts)
		{
			if (Input.IsKeyReleased(KeyCode.A) || Input.IsKeyReleased(KeyCode.D))
				AnimationComponent.Stop();

			if (Input.IsKeyDown(KeyCode.A)) {
				Body.ApplyForce(new Vector2(-Speed, 0.0f));

				if (Input.IsKeyDown(KeyCode.LeftShift))
					AnimationComponent.Play("Run_Left");
				else
					AnimationComponent.Play("Walk_Left");
			}

			if (Input.IsKeyDown(KeyCode.D)) {
				Body.ApplyForce(new Vector2(Speed, 0.0f));

				if (Input.IsKeyDown(KeyCode.LeftShift))
					AnimationComponent.Play("Run_Right");
				else
					AnimationComponent.Play("Walk_Right");
			}

			if (Input.IsKeyPressed(KeyCode.Space)) {
				Body.ApplyForce(new Vector2(0.0f, 250.0f));
				
				Score++;

				ScoreText.Text = string.Format("Score: {0}", Score);

				Scene.InstantiatePrefab(BottlePrefab);
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
