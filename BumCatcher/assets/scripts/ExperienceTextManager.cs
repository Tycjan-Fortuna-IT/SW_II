using System.Collections.Generic;
using SW;
using static System.Net.Mime.MediaTypeNames;

namespace BumCatcher
{
	internal class ExperienceTextManager : Entity
	{
		[SerializeField]
		private Prefab ExpTextPrefab = default!;

		class ExpTextData
		{
			public Entity Entity;
			public float Life = 1.0f;

			public ExpTextData(Entity entity)
			{
				Entity = entity;
			}
		}

		private List<ExpTextData> m_Texts = new List<ExpTextData>();

		protected override void OnUpdate(float ts)
		{
			for (int i = 0; i < m_Texts.Count; i++) {
				var data = m_Texts[i];

				data.Life -= ts;

				TextComponent tc = data.Entity.GetComponent<TextComponent>()!;
				Vector4 color = tc.Color;
				tc.Color = new Vector4(color.X, color.Y, color.Z, tc.Color.W - ts);

				if (data.Life < 0) {
					data.Entity.Destroy();
					m_Texts.RemoveAt(i--);
				}
			}
		}

		public void SpawnExpText(Vector3 position, int expAmount)
		{
			Entity entity = Scene.InstantiatePrefab(ExpTextPrefab, position)!;
			entity.GetComponent<TextComponent>()!.Text = $"{expAmount} exp";

			ExpTextData data = new ExpTextData(entity);

			m_Texts.Add(data);
		}
	}
}
