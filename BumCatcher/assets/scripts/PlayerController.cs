using SW;

namespace BumCatcher
{
	internal class PlayerController : Entity
	{
		[SerializeField]
		private float Speed = 12.0f;

		private TransformComponent Transform = default!;
		private RigidBody2DComponent Body = default!;
		private AnimatedSpriteComponent AnimationComponent = default!;

		private float m_Exp = 0.0f;
		private TextComponent ScoreText = default!;

		private ExperienceTextManager m_ExperienceTextManager = default!;

		protected override void OnCreate()
		{
			Transform = GetComponent<TransformComponent>()!;
			Body = GetComponent<RigidBody2DComponent>()!;
			AnimationComponent = GetComponent<AnimatedSpriteComponent>()!;

			ScoreText = Scene.GetEntityByTag("ScoreText").GetComponent<TextComponent>()!;

			OnCollision2DBegin += PlayerController_OnCollision2DBegin;
			m_ExperienceTextManager = Scene.GetEntityByTag("ExperienceTextManager").As<ExperienceTextManager>()!;
		}

		private void PlayerController_OnCollision2DBegin(Entity obj)
		{
			string tag = obj.GetComponent<TagComponent>()!.Tag;

			if (tag == "Beer") {
				obj.Destroy();

				m_Exp += 15;
				m_ExperienceTextManager.SpawnExpText(Transform.Position, 15);
				ScoreText.Text = string.Format("Exp: {0}", m_Exp);
			}
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
			}
		}

		protected override void OnLateUpdate(float ts)
		{

		}

		protected override void OnDestroy()
		{

		}

		protected override void OnCleanup()
		{

		}
	}
}
