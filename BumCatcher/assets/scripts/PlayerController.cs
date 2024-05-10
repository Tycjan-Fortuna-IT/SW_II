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

		private HoveringTextManager m_HoveringTextManager = default!;

		private HealthBar m_HPBar = default!;

		private AudioSourceComponent PickupSound = default!;

		protected override void OnCreate()
		{
			Transform = GetComponent<TransformComponent>()!;
			Body = GetComponent<RigidBody2DComponent>()!;
			AnimationComponent = GetComponent<AnimatedSpriteComponent>()!;

			ScoreText = Scene.GetEntityByTag("ScoreText").GetComponent<TextComponent>()!;

			OnCollision2DBegin += PlayerController_OnCollision2DBegin;
			m_HoveringTextManager = Scene.GetEntityByTag("HoveringTextManager").As<HoveringTextManager>()!;

			m_HPBar = Scene.GetEntityByTag("Main HP Bar").As<HealthBar>()!;
			m_HPBar.OnHealthBarEmpty += PlayerController_OnHealthBarEmpty;
			PickupSound = Scene.GetEntityByTag("PickupSound").GetComponent<AudioSourceComponent>()!;
		}

		private void PlayerController_OnHealthBarEmpty()
		{
			Log.Error("YOU ARE DEAD");
		}

		private void PlayerController_OnCollision2DBegin(Entity obj)
		{
			string tag = obj.GetComponent<TagComponent>()!.Tag;

			if (tag == "Beer") {
				PickupSound.Play();
				obj.Destroy();

				m_Exp += 30;
				m_HoveringTextManager.SpawnHoveringText(Transform.Position, "+30 exp", new Vector4(1.0f, 1.0f, 0.0f, 1.0f));
				ScoreText.Text = string.Format("Exp: {0}", m_Exp);
			} else if (tag == "Denature") {
				PickupSound.Play();
				PickupSound.Stop();
				obj.Destroy();

				m_Exp -= 15;
				
				m_HPBar.TakeDamage(25);

				m_HoveringTextManager.SpawnHoveringText(Transform.Position, "-15 exp", new Vector4(1.0f, 0.25f, 0.25f, 1.0f));
				ScoreText.Text = string.Format("Exp: {0}", m_Exp);
			} else if (tag == "Wine") {
				PickupSound.Play();
				obj.Destroy();

				m_HPBar.HealDamage(10);

				m_Exp += 15;
				m_HoveringTextManager.SpawnHoveringText(Transform.Position, "+15 exp", new Vector4(0.25f, 1.0f, 0.25f, 1.0f));
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
