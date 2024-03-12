using SW;
using System;

namespace Sandbox
{
	internal class TestEntity : Entity
	{
		private TransformComponent Transform;

		protected override void OnCreate()
		{
			Transform = GetComponent<TransformComponent>();
		}

		protected override void OnUpdate(float ts)
		{

		}

		protected override void OnLateUpdate(float ts)
		{

		}

		protected override void OnDestroy()
		{
			Console.WriteLine("OnDestroy");
		}
	}
}
