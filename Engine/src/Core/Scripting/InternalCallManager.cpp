#include "InternalCallManager.hpp"

#include <Coral/Assembly.hpp>
#include <box2d/b2_body.h>

#include "Core/ECS/Entity.hpp"
#include "Core/Scripting/ScriptingCore.hpp"
#include "Core/Application.hpp"
#include "Core/Utils/TypeInfo.hpp"
#include "Core/Utils/Input.hpp"

#ifdef SW_WINDOWS
	#define SW_FUNCTION_NAME __func__
#else
	#define SW_FUNCTION_NAME __FUNCTION__
#endif

#define ADD_INTERNAL_CALL(fn) coreAssembly->AddInternalCall("SW.InternalCalls", #fn, &fn);
#define ADD_INTERNAL_CALL_FN(name, fn) coreAssembly->AddInternalCall("SW.InternalCalls", #name, &fn);

#define INTERNAL_CALL_VALIDATE_PARAM_VALUE(param, value)														\
	if (!(param))																								\
	{																											\
		SW_FATAL("{} called with an invalid value ({}) for parameter '{}'.", SW_FUNCTION_NAME, value, #param);	\
	}

namespace SW {

	std::unordered_map<Coral::TypeId, std::function<void(Entity&)>> s_AddComponentFuncs;
	std::unordered_map<Coral::TypeId, std::function<bool(Entity&)>> s_HasComponentFuncs;
	std::unordered_map<Coral::TypeId, std::function<void(Entity&)>> s_RemoveComponentFuncs;

	template<typename T>
	static void RegisterManagedComponent(Coral::ManagedAssembly* coreAssembly)
	{
		const TypeNameString& componentTypeName = TypeInfo<T, true>().Name(); // demangling
		std::string componentName = std::format("SW.{}", componentTypeName);

		Coral::Type& type = coreAssembly->GetType(componentName);

		if (type) {
			s_AddComponentFuncs[type.GetTypeId()] = [](Entity& entity) { entity.AddComponent<T>(); };
			s_HasComponentFuncs[type.GetTypeId()] = [](Entity& entity) { return entity.HasComponent<T>(); };
			s_RemoveComponentFuncs[type.GetTypeId()] = [](Entity& entity) { entity.RemoveComponent<T>(); };
		} else {
			ASSERT(false, "No C# component class found for {}!", componentName);
		}
	}

	static inline Entity GetEntityById(u64 entityID)
	{
		Scene* scene = ScriptingCore::Get().GetCurrentScene();

		ASSERT(scene, "No active scene!");

		return scene->TryGetEntityByID(entityID);
	};

	static inline Entity GetEntityByTag(std::string tag)
	{
		Scene* scene = ScriptingCore::Get().GetCurrentScene();

		ASSERT(scene, "No active scene!");

		return scene->TryGetEntityByTag(tag);
	};

	void InternalCallManager::Initialize(Coral::ManagedAssembly* coreAssembly)
	{
		s_AddComponentFuncs.clear();
		s_HasComponentFuncs.clear();
		s_RemoveComponentFuncs.clear();

		RegisterTypes(coreAssembly);
		RegisterInternalCalls(coreAssembly);

		coreAssembly->UploadInternalCalls();
    }

    void InternalCallManager::RegisterTypes(Coral::ManagedAssembly* coreAssembly)
	{
		RegisterManagedComponent<TagComponent>(coreAssembly);
		RegisterManagedComponent<TransformComponent>(coreAssembly);
		RegisterManagedComponent<AnimatedSpriteComponent>(coreAssembly);
		RegisterManagedComponent<RigidBody2DComponent>(coreAssembly);
    }

	u32 Application_GetVieportWidth() { return ScriptingCore::Get().GetCurrentScene()->GetViewportWidth(); }
	
	u32 Application_GetVieportHeight() { return ScriptingCore::Get().GetCurrentScene()->GetViewportHeight(); }
	
	void Application_Shutdown() { return Application::Get()->Close(); }


	void Log_TraceMessage(Coral::String msg)
	{
		std::string message = msg;

		APP_TRACE(message);

		Coral::String::Free(msg);
	}

	void Log_InfoMessage(Coral::String msg)
	{
		std::string message = msg;

		APP_INFO(message);

		Coral::String::Free(msg);
	}

	void Log_DebugMessage(Coral::String msg)
	{
		std::string message = msg;

		APP_DEBUG(message);

		Coral::String::Free(msg);
	}

	void Log_WarnMessage(Coral::String msg)
	{
		std::string message = msg;

		APP_WARN(message);

		Coral::String::Free(msg);
	}

	void Log_ErrorMessage(Coral::String msg)
	{
		std::string message = msg;

		APP_ERROR(message);

		Coral::String::Free(msg);
	}

	bool Entity_HasComponent(u64 entityID, Coral::ReflectionType componentType)
	{
		Entity entity = GetEntityById(entityID);
		Coral::Type& type = componentType;

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		if (!entity || !type)
			return false;

		if (!s_HasComponentFuncs.contains(type.GetTypeId())) {
			ASSERT(
				false,
				"Cannot check if entity '{}' has a component of type '{}'. That component hasn't been registered with the engine.",
				entity.GetID(),
				type.GetFullName().Data()
			);

			return false;
		}

		return s_HasComponentFuncs.at(type.GetTypeId())(entity);
	}

	void Entity_AddComponent(u64 entityID, Coral::ReflectionType componentType)
	{
		Entity entity = GetEntityById(entityID);
		Coral::Type& type = componentType;

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		if (!entity || !type)
			return;

		if (!s_AddComponentFuncs.contains(type.GetTypeId())) {
			ASSERT(
				false,
				"Cannot add to entity '{}' a component of type '{}'. That component doesn't have create handler registered with the engine.",
				entity.GetID(),
				type.GetFullName().Data()
			);

			return;
		}

		s_AddComponentFuncs.at(type.GetTypeId())(entity);
	}

	void Entity_RemoveComponent(u64 entityID, Coral::ReflectionType componentType)
	{
		Entity entity = GetEntityById(entityID);
		Coral::Type& type = componentType;

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		if (!entity || !type)
			return;

		if (!s_RemoveComponentFuncs.contains(type.GetTypeId())) {
			ASSERT(
				false,
				"Cannot remove from entity '{}' a component of type '{}'. That component doesn't have remove handler registered with the engine.",
				entity.GetID(),
				type.GetFullName().Data()
			);

			return;
		}

		s_RemoveComponentFuncs.at(type.GetTypeId())(entity);
	}

	u64 Scene_TryGetEntityByID(u64 id)
	{
		Entity entity = GetEntityById(id);

		return entity ? id : 0;
	}

	u64 Scene_TryGetEntityByTag(Coral::String tag)
	{
		std::string nativeTag = tag;

		Entity entity = GetEntityByTag(nativeTag);

		Coral::String::Free(tag);

		return entity ? entity.GetID() : 0;
	}

	Coral::String TagComponent_GetTag(u64 entityID)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		const TagComponent& tagComponent = entity.GetComponent<TagComponent>();

		return Coral::String::New(tagComponent.Tag);
	}

	void TagComponent_SetTag(u64 entityID, Coral::String inTag)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		entity.GetComponent<TagComponent>().Tag = inTag;
	}

	void TransformComponent_GetPosition(u64 entityID, glm::vec3* outPosition)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		*outPosition = entity.GetComponent<TransformComponent>().Position;
	}

	void TransformComponent_SetPosition(u64 entityID, glm::vec3* inPosition)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		if (inPosition == nullptr) {
			ASSERT(false, "Attempting to set null translation for entity '{}'", entity.GetID());

			return;
		}

		if (entity.HasComponent<RigidBody2DComponent>()) {
			RigidBody2DComponent& rbc = entity.GetComponent<RigidBody2DComponent>();

			if (rbc.Type != PhysicBodyType::Static) {
				SW_WARN("[SCRIPT]: Trying to set translation for non-static RigidBody2D for entity {}. This isn't allowed, and would result in unstable physics behavior.", entityID);
				return;
			}

			b2Body* body = static_cast<b2Body*>(rbc.Handle);
			body->SetTransform({ inPosition->x, inPosition->y }, body->GetAngle());
		}

		entity.GetComponent<TransformComponent>().Position = *inPosition;
	}

	void TransformComponent_GetRotation(u64 entityID, glm::vec3* outRotation)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	void TransformComponent_SetRotation(u64 entityID, glm::vec3* inRotation)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		entity.GetComponent<TransformComponent>().Rotation = *inRotation;
	}

	void TransformComponent_GetScale(u64 entityID, glm::vec3* outScale)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}

	void TransformComponent_SetScale(u64 entityID, glm::vec3* inScale)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		entity.GetComponent<TransformComponent>().Scale = *inScale;
	}

	void AnimatedSpriteComponent_Play(u64 entityID, Coral::String name)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		AnimatedSpriteComponent& asc = entity.GetComponent<AnimatedSpriteComponent>();

		std::string animationName = name;

		Coral::String::Free(name);

		auto it = asc.Animations.find(animationName);
		if (it == asc.Animations.end()) {
			APP_ERROR("Could not find animation {} for entity {}", animationName, entityID);
			return;
		}

		asc.CurrentFrame = 0;
		asc.CurrentAnimation = it->second;
	}

	void AnimatedSpriteComponent_Stop(u64 entityID)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		AnimatedSpriteComponent& asc = entity.GetComponent<AnimatedSpriteComponent>();

		asc.CurrentFrame = 0;
		asc.CurrentAnimation = asc.DefaultAnimation;
	}
	
	void Rigidbody2DComponent_GetVelocity(u64 entityID, glm::vec2* outVelocity)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		RigidBody2DComponent& rbc = entity.GetComponent<RigidBody2DComponent>();

		ASSERT(rbc.Handle);

		b2Body* body = (b2Body*)rbc.Handle;
		const b2Vec2 velocity = body->GetLinearVelocity();

		outVelocity->x = velocity.x;
		outVelocity->y = velocity.y;
	}

	void Rigidbody2DComponent_SetVelocity(u64 entityID, glm::vec2* inVelocity)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		RigidBody2DComponent& rbc = entity.GetComponent<RigidBody2DComponent>();

		ASSERT(rbc.Handle);

		b2Body* body = (b2Body*)rbc.Handle;
		body->SetLinearVelocity({ inVelocity->x, inVelocity->y });
	}

	void RigidBody2DComponent_ApplyForce(u64 entityID, glm::vec2* inForce, glm::vec2* inOffset, bool wake)
	{
		Entity entity = GetEntityById(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		RigidBody2DComponent& rbc = entity.GetComponent<RigidBody2DComponent>();
		
		ASSERT(rbc.Handle);

		if (rbc.Type != PhysicBodyType::Dynamic) {
			SW_WARN("[SCRIPT]: Trying to apply force for non-dynamic RigidBody2D for entity with ID: {}.", entityID);
			return;
		}

		b2Body* body = (b2Body*)rbc.Handle;
		body->ApplyForce(*(const b2Vec2*)inForce, body->GetWorldCenter() + *(const b2Vec2*)inOffset, wake);
	}

	void Input_GetWindowMousePosition(glm::vec2* outMousePosition)
	{
		*outMousePosition = Input::GetMousePosition();
	}

	void Input_GetViewportMousePosition(glm::vec2* outMousePosition)
	{
		Scene* scene = ScriptingCore::Get().GetCurrentScene();

		glm::vec2 viewportPosition = scene->GetViewportPosition();

		glm::vec2 mousePos = Input::GetMousePosition();

		*outMousePosition = {
			mousePos.x - viewportPosition.x - 5.f, // substracting imgui window padding
			mousePos.y - viewportPosition.y - 35.f
		};
	}

	void InternalCallManager::RegisterInternalCalls(Coral::ManagedAssembly* coreAssembly)
	{
		ADD_INTERNAL_CALL(Application_GetVieportWidth);
		ADD_INTERNAL_CALL(Application_GetVieportHeight);
		ADD_INTERNAL_CALL(Application_Shutdown);


		ADD_INTERNAL_CALL_FN(Input_IsKeyPressed, Input::IsKeyPressed);
		ADD_INTERNAL_CALL_FN(Input_IsKeyHeld, Input::IsKeyHeld);
		ADD_INTERNAL_CALL_FN(Input_IsKeyDown, Input::IsKeyDown);
		ADD_INTERNAL_CALL_FN(Input_IsKeyReleased, Input::IsKeyReleased);

		ADD_INTERNAL_CALL_FN(Input_IsMouseButtonPressed, Input::IsMouseButtonPressed);
		ADD_INTERNAL_CALL_FN(Input_IsMouseButtonHeld, Input::IsMouseButtonHeld);
		ADD_INTERNAL_CALL_FN(Input_IsMouseButtonDown, Input::IsMouseButtonDown);
		ADD_INTERNAL_CALL_FN(Input_IsMouseButtonReleased, Input::IsMouseButtonReleased);

		ADD_INTERNAL_CALL(Input_GetWindowMousePosition);
		ADD_INTERNAL_CALL(Input_GetViewportMousePosition);


		ADD_INTERNAL_CALL(Log_TraceMessage);
		ADD_INTERNAL_CALL(Log_InfoMessage);
		ADD_INTERNAL_CALL(Log_DebugMessage);
		ADD_INTERNAL_CALL(Log_WarnMessage);
		ADD_INTERNAL_CALL(Log_ErrorMessage);


		ADD_INTERNAL_CALL(Entity_HasComponent);
		ADD_INTERNAL_CALL(Entity_AddComponent);
		ADD_INTERNAL_CALL(Entity_RemoveComponent);


		ADD_INTERNAL_CALL(Scene_TryGetEntityByID);
		ADD_INTERNAL_CALL(Scene_TryGetEntityByTag);

		
		ADD_INTERNAL_CALL(TagComponent_GetTag);
		ADD_INTERNAL_CALL(TagComponent_SetTag);


		ADD_INTERNAL_CALL(TransformComponent_GetPosition);
		ADD_INTERNAL_CALL(TransformComponent_SetPosition);

		ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		ADD_INTERNAL_CALL(TransformComponent_SetRotation);

		ADD_INTERNAL_CALL(TransformComponent_GetScale);
		ADD_INTERNAL_CALL(TransformComponent_SetScale);
		

		ADD_INTERNAL_CALL(AnimatedSpriteComponent_Play);
		ADD_INTERNAL_CALL(AnimatedSpriteComponent_Stop);


		ADD_INTERNAL_CALL(Rigidbody2DComponent_GetVelocity);
		ADD_INTERNAL_CALL(Rigidbody2DComponent_SetVelocity);

		ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyForce);
    }
}
