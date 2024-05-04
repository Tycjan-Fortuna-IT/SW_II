/**
 * @file Prefab.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-13
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset/Asset.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Scene/Scene.hpp"

namespace SW {

	class Prefab : public Asset
	{
	public:
		Prefab() = default;
		Prefab(Entity entity);

		static AssetType GetStaticType() { return AssetType::Prefab; }
		AssetType GetAssetType() const override { return AssetType::Prefab; }

		const Scene* GetScene() const { return  &m_Scene; }

		Scene* GetSceneRaw() { return &m_Scene; }

		Entity GetRootEntity() const { return m_PrefabEntity; }
		void SetRootEntity(Entity root) { m_PrefabEntity = root; } // used by AssetSerializer

	private:
		Entity m_PrefabEntity = {};
		Scene m_Scene;

	private:
		Entity DuplicateEntityForPrefab(Entity src, std::unordered_map<u64, Entity>& duplicatedEntities);
	};

}

