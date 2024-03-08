/**
 * @file InternalCallManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-08
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace Coral {
	class AssemblyLoadContext; 
	class HostInstance;
	class ManagedAssembly;
	class Type;
}

namespace SW {

	class Scene;
	class Project;

	struct AssemblyData {
		Coral::ManagedAssembly* Assembly;
		std::unordered_map<u64, Coral::Type*> CachedTypes;
	};

	class ScriptingCore
	{
	public:
		void InitializeHost();
		void ShutdownHost();

		void Initialize();
		void Shutdown();

		void SetCurrentScene(Scene* scene) { m_CurrentScene = scene; }
		Scene* GetCurrentScene() { return m_CurrentScene; }

		static ScriptingCore& Get();

	private:
		ScriptingCore() = default;

		ScriptingCore(const ScriptingCore&) = delete;
		ScriptingCore(ScriptingCore&&) = delete;

		ScriptingCore& operator=(const ScriptingCore&) = delete;
		ScriptingCore& operator=(ScriptingCore&&) = delete;

		Coral::HostInstance* m_Host = nullptr;
		Coral::AssemblyLoadContext* m_AssemblyContext = nullptr;

		Scene* m_CurrentScene = nullptr;

		AssemblyData* m_CoreAssemblyData = nullptr;
		AssemblyData* m_AppAssemblyData = nullptr;
	};

}

