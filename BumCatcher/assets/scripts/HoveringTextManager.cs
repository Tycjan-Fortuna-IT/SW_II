using System.Collections.Generic;
using SW;

namespace BumCatcher
{
	internal class HoveringTextManager : Entity
	{
		[SerializeField]
		private Prefab HoveringTextPrefab = default!;

		class HoveringTextData
		{
			public Entity Entity;
			public float Life = 1.0f;

			public HoveringTextData(Entity entity)
			{
				Entity = entity;
			}
		}

		private List<HoveringTextData> m_Texts = new List<HoveringTextData>();

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

		public void SpawnHoveringText(Vector3 position, string text, Vector4 color)
		{
			Entity entity = Scene.InstantiatePrefab(HoveringTextPrefab, position)!;
			TextComponent tc = entity.GetComponent<TextComponent>()!;

			tc.Text = text;
			tc.Color = color;

			HoveringTextData data = new HoveringTextData(entity);
			m_Texts.Add(data);
		}
	}
}
