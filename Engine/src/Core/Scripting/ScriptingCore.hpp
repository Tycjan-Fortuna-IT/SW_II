/**
 * @file InternalCallManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-09
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <Coral/ManagedObject.hpp>
#include <Coral/Type.hpp>
#include <Coral/StableVector.hpp>
#include <Coral/Array.hpp>
#include <Coral/Assembly.hpp>

#include "ScriptStorage.hpp"
#include "Core/Buffer.hpp"
#include "CSharpObject.hpp"

namespace Coral {
	class AssemblyLoadContext; 
	class HostInstance;
}

namespace SW {

	class Scene;
	class Project;

	struct AssemblyData
	{
		Coral::ManagedAssembly* Assembly; 					/**< The managed assembly used for scripting. */
		std::unordered_map<u64, Coral::Type*> CachedTypes;  /**< A map of cached types for efficient lookup. */
	};

	struct FieldMetadata
	{
		std::string Name;			/**< The name of the field. */
		DataType Type;				/**< The type of the field. */
		Coral::Type* ManagedType;	/**< The managed type of the field. */

		Buffer DefaultValue;		/**< The default value of the field. */

		/**
		 * Sets the default value for a given ManagedObject field.
		 * 
		 * @tparam T The type of the field.
		 * @param temp The ManagedObject instance.
		 */
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

	struct ScriptMetadata
	{
		std::string FullName;							/**< The full name of the script. */
		std::unordered_map<u32, FieldMetadata> Fields;	/**< A map of fields for the script. */
	};

	/**
	 * @class ScriptingCore
	 * @brief The ScriptingCore class is responsible for managing the scripting functionality of the engine.
	 *
	 * It provides methods for initializing and shutting down the scripting host, as well as managing scripts and their metadata.
	 * The ScriptingCore class also allows for instantiating and destroying script instances, and accessing script types and fields.
	 */
	class ScriptingCore
	{
	public:
		/**
		 * @brief Initializes the scripting host.
		 */
		void InitializeHost();

		/**
		 * @brief Shuts down the scripting host.
		 */
		void ShutdownHost();

		/**
		 * @brief Initializes the ScriptingCore DLLs.
		 */
		void Initialize();

		/**
		 * @brief Shuts down the ScriptingCore.
		 */
		void Shutdown();


		/**
		 * @brief Sets the current scene.
		 * @param scene The pointer to the current scene.
		 */
		void SetCurrentScene(Scene* scene) { m_CurrentScene = scene; }
		
		/**
		 * @brief Gets the current scene.
		 * @return The pointer to the current scene.
		 */
		Scene* GetCurrentScene() { return m_CurrentScene; }

		/**
		 * @brief Builds the assembly cache using the provided assembly data.
		 * @param assemblyData The pointer to the assembly data.
		 */
		void BuildAssemblyCache(AssemblyData* assemblyData);

		/**
		 * @brief Checks if a script with the given ID is valid.
		 * @param scriptID The ID of the script to check.
		 * @return True if the script is valid, false otherwise.
		 */
		bool IsValidScript(u64 scriptID) const;

		/**
		 * @brief Gets the metadata of a script with the given ID.
		 * @param scriptID The ID of the script.
		 * @return The const reference to the script metadata.
		 */
		const ScriptMetadata& GetScriptMetadata(u64 scriptID) const;

		/**
		 * @brief Gets all the scripts and their metadata.
		 * @return The const reference to the map of script IDs and their metadata.
		 */
		const std::unordered_map<u64, ScriptMetadata>& GetAllScripts() const { return m_ScriptMetadata; }

		/**
		 * @brief Gets the type with the given name.
		 * @param name The name of the type.
		 * @return The const pointer to the type.
		 */
		const Coral::Type* GetTypeByName(std::string_view name) const;

		AssemblyData* GetCoreAssembly() { return m_CoreAssemblyData; }

		AssemblyData* GetAppAssembly() { return m_AppAssemblyData; }

		/**
		 * @brief Gets the instance of the ScriptingCore.
		 * @return The reference to the ScriptingCore instance.
		 */
		static ScriptingCore& Get();

		/**
		 * @brief Instantiates a C# object using the provided entity ID, script storage, and constructor arguments.
		 * @tparam TArgs The types of the constructor arguments.
		 * @param entityID The ID of the entity.
		 * @param storage The reference to the script storage.
		 * @param args The constructor arguments.
		 * @return The instantiated C# object.
		 */
		template<typename... TArgs>
		CSharpObject Instantiate(u64 entityID, ScriptStorage& storage, TArgs&&... args)
		{
			ASSERT(storage.EntityStorage.contains(entityID), "Not present in storage");

			auto& entityStorage = storage.EntityStorage.at(entityID);

			if (!IsValidScript(entityStorage.ScriptID))
				return {};

			Coral::Type* type = m_AppAssemblyData->CachedTypes[entityStorage.ScriptID];
			auto instance = type->CreateInstance(std::forward<TArgs>(args)...);
			auto [index, handle] = m_ManagedObjects.Insert(std::move(instance));

			entityStorage.Instance = &handle;

			for (auto& [fieldID, fieldStorage] : entityStorage.Fields) {
				const auto& fieldMetadata = m_ScriptMetadata[entityStorage.ScriptID].Fields[fieldID];

				if (fieldMetadata.ManagedType->IsSZArray()) {
					struct ArrayContainer {
						void* Data;
						u32 Length;
					} array;

					array.Data = fieldStorage.m_ValueBuffer.Data;
					array.Length = (u32)fieldStorage.GetLength();

					handle.SetFieldValueRaw(fieldStorage.GetName(), &array);
				} else {
					handle.SetFieldValueRaw(fieldStorage.GetName(), fieldStorage.m_ValueBuffer.Data);
				}

				fieldStorage.m_Instance = &handle;
			}

			CSharpObject result;
			result.m_Handle = &handle;
			return result;
		}

		/**
		 * @brief Destroys the instance of a script with the given entity ID.
		 * @param entityID The ID of the entity.
		 * @param storage The reference to the script storage.
		 */
		void DestroyInstance(u64 entityID, ScriptStorage& storage)
		{
			ASSERT(storage.EntityStorage.contains(entityID), "Not present in storage");

			auto& entityStorage = storage.EntityStorage.at(entityID);

			ASSERT(IsValidScript(entityStorage.ScriptID), "Script not valid");

			for (auto& [fieldID, fieldStorage] : entityStorage.Fields)
				fieldStorage.m_Instance = nullptr;

			entityStorage.Instance->Destroy();
			entityStorage.Instance = nullptr;
		}

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

		Coral::StableVector<Coral::ManagedObject> m_ManagedObjects;

		/**
		 * @brief Loads the project assembly.
		 */
		void LoadProjectAssembly();
	};

}

