using SW;

namespace BumCatcher
{
	internal class DroppableSpawner : Entity
	{
		[SerializeField]
		private Prefab BottlePrefab = default!;

		[SerializeField]
		private Prefab DenaturePrefab = default!;

		[SerializeField]
		private Prefab WinePrefab = default!;

		private float timer = 0f;
		private float spawnInterval = 1f;

		protected override void OnUpdate(float ts)
		{
			timer += ts;

			if (timer >= spawnInterval) {
				int x = Random.Range(-7, 7);
				int pref = Random.Range(0, 2);

				if (pref == 0) {
					Scene.InstantiatePrefab(BottlePrefab, new Vector3(x, 2.0f, 0.0f));
				} else if (pref == 1) {
					Scene.InstantiatePrefab(DenaturePrefab, new Vector3(x, 2.0f, 0.0f));
				} else {
					Scene.InstantiatePrefab(WinePrefab, new Vector3(x, 2.0f, 0.0f));
				}

				timer = 0f;
			}
		}
	}
}
