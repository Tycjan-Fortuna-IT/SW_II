#include "InternalCallManager.hpp"

#include <Coral/Assembly.hpp>

#include "Core/ECS/Entity.hpp"
#include "Core/Scripting/ScriptingCore.hpp"
#include "Core/Application.hpp"
#include "Core/Utils/TypeInfo.hpp"

#ifdef SW_WINDOWS
#define SW_FUNCTION_NAME __func__
#else
	#define SW_FUNCTION_NAME __FUNCTION__
#endif

#define ADD_INTERNAL_CALL(fn) coreAssembly->AddInternalCall("SW.InternalCalls", #fn, &fn);

#define INTERNAL_CALL_VALIDATE_PARAM_VALUE(param, value)														\
	if (!(param))																								\
	{																											\
		SW_FATAL("{} called with an invalid value ({}) for parameter '{}'.", SW_FUNCTION_NAME, value, #param);	\
	}

namespace SW {

	std::unordered_map<Coral::TypeId, std::function<void(Entity&)>> s_CreateComponentFuncs;
	std::unordered_map<Coral::TypeId, std::function<bool(Entity&)>> s_HasComponentFuncs;
	std::unordered_map<Coral::TypeId, std::function<void(Entity&)>> s_RemoveComponentFuncs;

	template<typename T>
	static void RegisterManagedComponent(Coral::ManagedAssembly* coreAssembly)
	{
		const TypeNameString& componentTypeName = TypeInfo<T, true>().Name(); // demangling
		std::string componentName = std::format("SW.{}", componentTypeName);

		Coral::Type& type = coreAssembly->GetType(componentName);

		if (type) {
			s_CreateComponentFuncs[type.GetTypeId()] = [](Entity& entity) { entity.AddComponent<T>(); };
			s_HasComponentFuncs[type.GetTypeId()] = [](Entity& entity) { return entity.HasComponent<T>(); };
			s_RemoveComponentFuncs[type.GetTypeId()] = [](Entity& entity) { entity.RemoveComponent<T>(); };
		} else {
			ASSERT(false, "No C# component class found for {}!", componentName);
		}
	}

	static inline Entity GetEntity(uint64_t entityID)
	{
		Scene* scene = ScriptingCore::Get().GetCurrentScene();
		
		ASSERT(scene, "No active scene!");
		
		return scene->TryGetEntityByID(entityID);
	};

    void InternalCallManager::Initialize(Coral::ManagedAssembly* coreAssembly)
    {
		s_CreateComponentFuncs.clear();
		s_HasComponentFuncs.clear();
		s_RemoveComponentFuncs.clear();

		RegisterTypes(coreAssembly);
		RegisterInternalCalls(coreAssembly);

		coreAssembly->UploadInternalCalls();
    }

    void InternalCallManager::RegisterTypes(Coral::ManagedAssembly* coreAssembly)
    {
		RegisterManagedComponent<TransformComponent>(coreAssembly);
    }

	u32 Application_GetVieportWidth() { return ScriptingCore::Get().GetCurrentScene()->GetViewportWidth(); }
	
	u32 Application_GetVieportHeight() { return ScriptingCore::Get().GetCurrentScene()->GetViewportHeight(); }
	
	void Application_Shutdown() { return Application::Get()->Close(); }

	bool Entity_HasComponent(u64 entityID, Coral::ReflectionType componentType)
	{
		Entity entity = GetEntity(entityID);

		INTERNAL_CALL_VALIDATE_PARAM_VALUE(entity, entityID);

		if (!entity)
			return false;

		Coral::Type& type = componentType;

		if (!type)
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

    void InternalCallManager::RegisterInternalCalls(Coral::ManagedAssembly* coreAssembly)
    {
		ADD_INTERNAL_CALL(Application_GetVieportWidth);
		ADD_INTERNAL_CALL(Application_GetVieportHeight);
		ADD_INTERNAL_CALL(Application_Shutdown);

		ADD_INTERNAL_CALL(Entity_HasComponent);
    }

}
