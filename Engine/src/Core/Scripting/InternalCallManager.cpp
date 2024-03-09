#include "InternalCallManager.hpp"

#include <Coral/Assembly.hpp>

#include "Core/ECS/Entity.hpp"
#include "ScriptingCore.hpp"

namespace SW {

	std::unordered_map<Coral::TypeId, std::function<void(Entity&)>> s_CreateComponentFuncs;
	std::unordered_map<Coral::TypeId, std::function<bool(Entity&)>> s_HasComponentFuncs;
	std::unordered_map<Coral::TypeId, std::function<void(Entity&)>> s_RemoveComponentFuncs;

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

    }

	u32 Application_GetWidth() { return 500u; }

    void InternalCallManager::RegisterInternalCalls(Coral::ManagedAssembly* coreAssembly)
    {
		coreAssembly->AddInternalCall("SW.InternalCalls", "Application_GetWidth", &Application_GetWidth);
    }

}
