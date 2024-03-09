/**
 * @file InternalCallManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-08
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "ScriptStorage.hpp"
#include "Core/Buffer.hpp"

#include <Coral/ManagedObject.hpp>
#include <Coral/Type.hpp>
#include <Coral/Array.hpp>

namespace Coral {
	class AssemblyLoadContext; 
	class HostInstance;
	class ManagedAssembly;
}

namespace SW {

	class Scene;
	class Project;

	struct AssemblyData {
		Coral::ManagedAssembly* Assembly;
		std::unordered_map<u64, Coral::Type*> CachedTypes;
	};

	struct FieldMetadata {
		std::string Name;
		DataType Type;
		Coral::Type* ManagedType;

		Buffer DefaultValue;

		template<typename T>
		void SetDefaultValue(Coral::ManagedObject& temp)
		{
			if (ManagedType->IsSZArray()) {
				Coral::Array<T> value = temp.GetFieldValue<Coral::Array<T>>(Name);

				DefaultValue = Buffer::Copy(value.Data(), (u32)value.ByteLength());

				Coral::Array<T>::Free(value);
			} else {
				DefaultValue.Allocate(sizeof(T));

				T value = temp.GetFieldValue<T>(Name);

				DefaultValue.Write(&value, sizeof(T));
			}
		}
	};

	struct ScriptMetadata {
		std::string FullName;
		std::unordered_map<u32, FieldMetadata> Fields;
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

		void BuildAssemblyCache(AssemblyData* assemblyData);

		bool IsValidScript(u64 scriptID) const;

		const ScriptMetadata& GetScriptMetadata(u64 scriptID) const;
		const std::unordered_map<u64, ScriptMetadata>& GetAllScripts() const { return m_ScriptMetadata; }

		const Coral::Type* GetTypeByName(std::string_view name) const;

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
	
		std::unordered_map<u64, ScriptMetadata> m_ScriptMetadata;

		void LoadProjectAssembly();
	};

}

