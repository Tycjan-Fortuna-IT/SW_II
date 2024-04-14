#include "Prefab.hpp"

namespace SW {
/*#define CopyReferencedEntitiesPref(T) \
	{ \
		if (src.HasComponent<T>()) { \
			T& component = src.GetComponent<T>(); \
			T& newComponent = dst.GetComponent<T>(); \
			if (component.ConnectedEntityID) { \
				if (duplicatedEntities.contains(component.ConnectedEntityID)) { \
					newComponent.ConnectedEntityID = duplicatedEntities.at(component.ConnectedEntityID).GetID(); \
				} else { \
					Entity connectedEntity = srcScene->GetEntityByID(component.ConnectedEntityID); \
					Entity duplicatedConnectedEntity = DuplicateEntityForPrefab(connectedEntity, duplicatedEntities); \
					newComponent.ConnectedEntityID = duplicatedConnectedEntity.GetID(); \
					duplicatedEntities[component.ConnectedEntityID] = duplicatedConnectedEntity; \
				} \
			} \
		} \
	}*/

// WELP
#define CopyReferencedEntities(T) \
	{ \
		if (src.HasComponent<T>()) { \
			T& component = src.GetComponent<T>(); \
			if (component.ConnectedEntityID) { \
				Entity connectedEntity = srcScene->GetEntityByID(component.ConnectedEntityID); \
				Entity duplicatedConnectedEntity = srcScene->DuplicateEntity(connectedEntity, duplicatedEntities); \
				T& newComponent = dst.GetComponent<T>(); \
				newComponent.ConnectedEntityID = duplicatedConnectedEntity.GetID(); \
			} \
		} \
	}

	Prefab::Prefab(Entity entity)
	{
		std::unordered_map<u64, Entity> duplicatedEntities;

		m_PrefabEntity = DuplicateEntityForPrefab(entity, duplicatedEntities);
	}

	Entity Prefab::DuplicateEntityForPrefab(Entity src, std::unordered_map<u64, Entity>& duplicatedEntities)
	{
		if (duplicatedEntities.count(src.GetID()) > 0) {
			return duplicatedEntities[src.GetID()];
		}

		Scene* srcScene = src.GetScene();

		entt::registry& destReg = m_Scene.GetRegistry().GetRegistryHandle();

		Entity dst = m_Scene.CreateEntity();

		srcScene->CopyComponentIfExists<TagComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<TransformComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<SpriteComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<AnimatedSpriteComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<CircleComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<TextComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<ScriptComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<CameraComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<RigidBody2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<BoxCollider2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<CircleCollider2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<PolygonCollider2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<BuoyancyEffector2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<DistanceJoint2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<RevolutionJoint2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<PrismaticJoint2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<SpringJoint2DComponent>(dst, destReg, src);
		srcScene->CopyComponentIfExists<WheelJoint2DComponent>(dst, destReg, src);

		// TODO
		/*CopyReferencedEntities(DistanceJoint2DComponent);
		CopyReferencedEntities(RevolutionJoint2DComponent);
		CopyReferencedEntities(PrismaticJoint2DComponent);
		CopyReferencedEntities(SpringJoint2DComponent);
		CopyReferencedEntities(WheelJoint2DComponent);*/

		u64 id = dst.GetID();
		
		duplicatedEntities[src.GetID()] = dst;

		std::vector<u64> childIds = src.GetRelations().ChildrenIDs;

		for (u64 childId : childIds) {
			Entity childDuplicate = DuplicateEntityForPrefab(srcScene->GetEntityByID(childId), duplicatedEntities);

			childDuplicate.SetParent(dst);
		}

		if (dst.HasComponent<ScriptComponent>())
		{
			const ScriptComponent& sc = dst.GetComponent<ScriptComponent>();
			dst.GetScene()->GetScriptStorage().InitializeEntityStorage(sc.ScriptID, dst.GetID());
			src.GetScene()->GetScriptStorage().CopyEntityStorage(src.GetID(), dst.GetID(), dst.GetScene()->GetScriptStorage());
		}

		return dst;
	}

}
