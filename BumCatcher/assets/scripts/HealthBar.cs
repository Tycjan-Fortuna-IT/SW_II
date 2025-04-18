﻿using SW;

namespace BumCatcher
{
	internal class HealthBar : Entity
	{
		private float m_Health = 200.0f;

		public readonly float MaxHealth = 200.0f;

		private Entity m_HP = default!;

		private TransformComponent m_HPTransform = default!;

		public System.Action? OnHealthBarEmpty;

		protected override void OnCreate()
		{
			m_HP = Scene.GetEntityByTag("HP"); // todo Entity.GetChildByTag("HP");

			m_HPTransform = m_HP.GetComponent<TransformComponent>()!;
		}

		public void TakeDamage(float damage)
		{
			float percentage = (m_Health - damage) / m_Health;
			float reduction = 1 - percentage;

			float percentageVal = percentage < 0 ? 0 : percentage;
			float reductionVal = reduction < 0 ? 0 : reduction;

			m_Health -= damage;

			if (m_Health < 0) {
				OnHealthBarEmpty?.Invoke();
			}

			m_HPTransform.Position = new Vector3(m_HPTransform.Position.X - (m_HPTransform.Scale.X * reductionVal) / 2,
												 m_HPTransform.Position.Y, m_HPTransform.Position.Z);

			m_HPTransform.Scale = new Vector3(m_HPTransform.Scale.X * percentageVal,
											  m_HPTransform.Scale.Y, m_HPTransform.Scale.Z);
		}

		// Does not heal from 0!
		public void HealDamage(float heal)
		{
			float newHealth = m_Health + heal;

			newHealth = Mathf.Min(newHealth, MaxHealth);

			float percentage = newHealth / m_Health;
			float increase = percentage - 1;

			m_Health = newHealth;

			m_HPTransform.Position = new Vector3(m_HPTransform.Position.X + (m_HPTransform.Scale.X * increase) / 2,
												m_HPTransform.Position.Y, m_HPTransform.Position.Z);

			m_HPTransform.Scale = new Vector3(m_HPTransform.Scale.X * percentage,
											m_HPTransform.Scale.Y, m_HPTransform.Scale.Z);
		}
	}
}
