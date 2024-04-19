using SW;

namespace BumCatcher
{
	internal class DroppableSpawner : Entity
	{
		[SerializeField]
		private Prefab BottlePrefab = default!;

		private float timer = 0f;
		private float spawnInterval = 1f;

		System.Random random = new System.Random();

		protected override void OnUpdate(float ts)
		{
			timer += ts;

			if (timer >= spawnInterval) {
				float x = random.Next(-7, 7);

				Scene.InstantiatePrefab(BottlePrefab, new Vector3(x, 0.0f, 0.0f));

				timer = 0f;
			}
		}
	}
}
