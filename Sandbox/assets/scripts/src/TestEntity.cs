using SW;
using System;

namespace Sandbox
{
	internal class TestEntity : Entity
	{
		[SerializeField]
		private float Velocity = 20.0f;

		protected override void OnCreate()
		{
			Console.WriteLine("OnCreate");
			Console.WriteLine(Game.GetVieportWidth());
			Console.WriteLine(Game.GetVieportHeight());

			Console.WriteLine(HasComponent<TransformComponent>());
		}

		protected override void OnUpdate(float ts)
		{
			Velocity += 1.0f;

			//if (Velocity > 500.0f) {
			//	Game.ShutDown();
			//}
		}

		protected override void OnLateUpdate(float ts)
		{

		}

		protected override void OnDestroy()
		{
			Console.WriteLine("OnDestroy");
			Console.WriteLine(Velocity);
		}
	}
}
