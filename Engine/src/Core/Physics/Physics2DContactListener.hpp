/**
 * @file Physics2DContactListener.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-02-11
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <box2d/box2d.h>

#include "Core/Timestep.hpp"

namespace SW {

	class Scene;

	class Physics2DContactListener final : public b2ContactListener
	{
	public:
		Physics2DContactListener(Scene* scene);

		void Step(Timestep dt);

		void BeginContact(b2Contact* contact) final override;

		void EndContact(b2Contact* contact) final override;

		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) final override;

		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) final override;

	private:
		Scene* m_Scene = nullptr;

		std::set<std::pair<b2Fixture*, b2Fixture*>> m_BuoyancyFluidFixturePairs;
	};

}

