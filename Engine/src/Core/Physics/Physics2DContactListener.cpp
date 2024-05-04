#include "Physics2DContactListener.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Physics/PhysicsUtils.hpp"

namespace SW {

	Physics2DContactListener::Physics2DContactListener(Scene* scene)
		: m_Scene(scene) {}

	void Physics2DContactListener::Step(UNUSED Timestep dt)
	{
		for (auto& [fluid, fixture] : m_BuoyancyFluidFixturePairs) {
			Entity fluidEntity = { static_cast<entt::entity>(static_cast<uint32_t>(fluid->GetUserData().pointer)), m_Scene };

			const BuoyancyEffector2DComponent& bec = fluidEntity.GetComponent<BuoyancyEffector2DComponent>();

			const b2Vec2 gravity = { m_Scene->Gravity.x, m_Scene->Gravity.y };

			PhysicsUtils::HandleBuoyancy(fluid, fixture, gravity, false, bec.Density, bec.DragMultiplier, bec.FlowMagnitude, bec.FlowAngle);
		}
	}

	void Physics2DContactListener::BeginContact(b2Contact* contact)
	{
		b2Fixture* firstFixture = contact->GetFixtureA();
		b2Fixture* secondFixture = contact->GetFixtureB();

		const bool isFirstFixtureSensor = firstFixture->IsSensor();
		const bool isSecondFixtureSensor = secondFixture->IsSensor();

		const Entity firstEntity = { static_cast<entt::entity>(static_cast<u32>(firstFixture->GetUserData().pointer)), m_Scene };
		const Entity secondEntity = { static_cast<entt::entity>(static_cast<u32>(secondFixture->GetUserData().pointer)), m_Scene };

		const bool isFirstEntityFluid = firstEntity.HasComponent<BuoyancyEffector2DComponent>();
		const bool isSecondEntityFluid = secondEntity.HasComponent<BuoyancyEffector2DComponent>();

		if (
			isFirstEntityFluid && isFirstFixtureSensor &&
			!isSecondEntityFluid && secondFixture->GetBody()->GetType() == b2_dynamicBody
		) {
			m_BuoyancyFluidFixturePairs.insert(std::make_pair(firstFixture, secondFixture));
		} else if (
			isSecondEntityFluid && isSecondFixtureSensor &&
			!isFirstEntityFluid && firstFixture->GetBody()->GetType() == b2_dynamicBody
		) {
			m_BuoyancyFluidFixturePairs.insert(std::make_pair(secondFixture, firstFixture));
		}

		const auto onContactBegin = [](Entity entity, Entity other) {
			if (!entity.HasComponent<ScriptComponent>())
				return;

			ScriptComponent& sc = entity.GetComponent<ScriptComponent>();

			sc.Instance.Invoke<u64>("OnCollision2DBeginInternal", other.GetID());
		};

		onContactBegin(firstEntity, secondEntity);
		onContactBegin(secondEntity, firstEntity);
	}

	void Physics2DContactListener::EndContact(b2Contact* contact)
	{
		b2Fixture* firstFixture = contact->GetFixtureA();
		b2Fixture* secondFixture = contact->GetFixtureB();

		const bool isFirstFixtureSensor = firstFixture->IsSensor();
		const bool isSecondFixtureSensor = secondFixture->IsSensor();

		const Entity firstEntity = { static_cast<entt::entity>(static_cast<u32>(firstFixture->GetUserData().pointer)), m_Scene };
		const Entity secondEntity = { static_cast<entt::entity>(static_cast<u32>(secondFixture->GetUserData().pointer)), m_Scene };

		const bool isFirstEntityFluid = firstEntity.HasComponent<BuoyancyEffector2DComponent>();
		const bool isSecondEntityFluid = secondEntity.HasComponent<BuoyancyEffector2DComponent>();

		if (
			isFirstEntityFluid && isFirstFixtureSensor &&
			!isSecondEntityFluid && firstFixture->GetBody()->GetType() == b2_dynamicBody
		) {
			m_BuoyancyFluidFixturePairs.erase(std::make_pair(firstFixture, secondFixture));
		} else if (
			isSecondEntityFluid && isSecondFixtureSensor &&
			!isFirstEntityFluid && secondFixture->GetBody()->GetType() == b2_dynamicBody
		) {
			m_BuoyancyFluidFixturePairs.erase(std::make_pair(secondFixture, firstFixture));
		}

		const auto onContactEnd = [](Entity entity, Entity other) {
			if (!entity.HasComponent<ScriptComponent>())
				return;

			ScriptComponent& sc = entity.GetComponent<ScriptComponent>();

			sc.Instance.Invoke<u64>("OnCollision2DEndInternal", other.GetID());
		};

		onContactEnd(firstEntity, secondEntity);
		onContactEnd(secondEntity, firstEntity);
	}

	void Physics2DContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{

	}

	void Physics2DContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{

	}

}
