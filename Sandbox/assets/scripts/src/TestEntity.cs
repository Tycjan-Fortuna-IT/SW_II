using Engine.ScriptCore;
using System;

namespace Sandbox
{
	internal class TestEntity : Entity
	{
		protected override void OnCreate()
		{
			Console.WriteLine(10000);
		}

		protected override void OnUpdate(float ts)
		{

		}

		protected override void OnLateUpdate(float ts)
		{

		}

		protected override void OnPhysicsUpdate(float ts)
		{

		}

		protected override void OnDestroy()
		{

		}
	}
}
