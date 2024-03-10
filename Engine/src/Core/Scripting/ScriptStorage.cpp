#include "ScriptStorage.hpp"
#include "ScriptingCore.hpp"

namespace SW {

	void ScriptStorage::InitializeEntityStorage(u64 scriptID, u64 entityID)
	{
		ScriptingCore& scriptEngine = ScriptingCore::Get();

		ASSERT(scriptEngine.IsValidScript(scriptID), "Script not valid!");
		ASSERT(!EntityStorage.contains(entityID), "Storage already initialized!");

		const ScriptMetadata& scriptMetadata = scriptEngine.GetScriptMetadata(scriptID);

		EntityScriptStorage& entityStorage = EntityStorage[entityID];
		entityStorage.ScriptID = scriptID;

		for (const auto& [fieldID, fieldMetadata] : scriptMetadata.Fields) {
			InitializeFieldStorage(entityStorage, fieldID, fieldMetadata);
		}
	}

	void ScriptStorage::ShutdownEntityStorage(u64 scriptID, u64 entityID)
	{
		ScriptingCore& scriptEngine = ScriptingCore::Get();

		ASSERT(scriptEngine.IsValidScript(scriptID), "Script not valid!");
		ASSERT(EntityStorage.contains(entityID), "Storage was not initialized!");

		for (auto& [fieldID, fieldStorage] : EntityStorage[entityID].Fields)
			fieldStorage.m_ValueBuffer.Release();

		EntityStorage.erase(entityID);
	}

	void ScriptStorage::SynchronizeStorage()
	{
		ScriptingCore& scriptEngine = ScriptingCore::Get();

		for (auto& [entityID, entityStorage] : EntityStorage) {
			const auto& scriptMetadata = scriptEngine.GetScriptMetadata(entityStorage.ScriptID);

			for (const auto& [fieldID, fieldMetadata] : scriptMetadata.Fields) {
				if (entityStorage.Fields.contains(fieldID)) {
					entityStorage.Fields[fieldID].m_Type = fieldMetadata.ManagedType;
					continue;
				}

				InitializeFieldStorage(entityStorage, fieldID, fieldMetadata);
			}
		}
	}

	void ScriptStorage::CopyTo(ScriptStorage& other) const
	{
		ScriptingCore& scriptEngine = ScriptingCore::Get();

		for (const auto& [entityID, entityStorage] : EntityStorage) {
			if (!scriptEngine.IsValidScript(entityStorage.ScriptID)) {
				ASSERT(false, "Cannot copy script data for script ID {}. The script is no longer valid", entityStorage.ScriptID);
				continue;
			}

			const auto& scriptMetadata = scriptEngine.GetScriptMetadata(entityStorage.ScriptID);

			auto& otherStorage = other.EntityStorage[entityID];
			otherStorage.ScriptID = entityStorage.ScriptID;
			otherStorage.Instance = nullptr;

			for (const auto& [fieldID, fieldStorage] : entityStorage.Fields) {
				if (!scriptMetadata.Fields.contains(fieldID)) {
					ASSERT(false, "Cannot copy script data for field {}. The field is no longer contained in the script.", fieldStorage.GetName());
					continue;
				}

				auto& otherFieldStorage = otherStorage.Fields[fieldID];
				otherFieldStorage.m_Name = fieldStorage.m_Name;
				otherFieldStorage.m_Type = fieldStorage.m_Type;
				otherFieldStorage.m_DataType = fieldStorage.m_DataType;
				otherFieldStorage.m_ValueBuffer = Buffer::Copy(fieldStorage.m_ValueBuffer);
				otherFieldStorage.m_Instance = nullptr;
			}
		}
	}

	void ScriptStorage::CopyEntityStorage(u64 entityID, u64 targetEntityID, ScriptStorage& targetStorage) const
	{
		if (!targetStorage.EntityStorage.contains(targetEntityID)) {
			ASSERT(false, "Cannot copy script storage to entity {} because InitializeScriptStorage hasn't been called for it.", targetEntityID);
			return;
		}

		ScriptingCore& scriptEngine = ScriptingCore::Get();
		const auto& srcStorage = EntityStorage.at(entityID);

		if (!scriptEngine.IsValidScript(srcStorage.ScriptID)) {
			ASSERT(false, "Cannot copy script data for script ID {}. The script is no longer valid", srcStorage.ScriptID);
			return;
		}

		auto& dstStorage = targetStorage.EntityStorage.at(targetEntityID);

		if (dstStorage.ScriptID != srcStorage.ScriptID) {
			ASSERT(false, "Cannot copy script storage from entity {} to entity {} because they have different scritps!", entityID, targetEntityID);
			return;
		}

		const auto& scriptMetadata = scriptEngine.GetScriptMetadata(srcStorage.ScriptID);

		dstStorage.Instance = nullptr;

		for (const auto& [fieldID, fieldStorage] : srcStorage.Fields) {
			if (!scriptMetadata.Fields.contains(fieldID)) {
				ASSERT(false, "Cannot copy script data for field {}. The field is no longer contained in the script.", fieldStorage.GetName());
				continue;
			}

			auto& otherFieldStorage = dstStorage.Fields[fieldID];
			otherFieldStorage.m_Name = fieldStorage.m_Name;
			otherFieldStorage.m_Type = fieldStorage.m_Type;
			otherFieldStorage.m_DataType = fieldStorage.m_DataType;
			otherFieldStorage.m_ValueBuffer = Buffer::Copy(fieldStorage.m_ValueBuffer);
			otherFieldStorage.m_Instance = nullptr;
		}
	}

	void ScriptStorage::Clear()
	{
		for (auto& [entityID, entityStorage] : EntityStorage) {
			for (auto& [fieldID, fieldStorage] : entityStorage.Fields)
				fieldStorage.m_ValueBuffer.Release();
		}

		EntityStorage.clear();
	}

	void ScriptStorage::InitializeFieldStorage(EntityScriptStorage& storage, u32 fieldID, const FieldMetadata& fieldMetadata)
	{
		auto& fieldStorage = storage.Fields[fieldID];
		fieldStorage.m_Name = fieldMetadata.Name;
		fieldStorage.m_Type = fieldMetadata.ManagedType;
		fieldStorage.m_DataType = fieldMetadata.Type;

		if (fieldMetadata.DefaultValue.Data == nullptr || fieldMetadata.DefaultValue.Size == 0) {
			fieldStorage.m_ValueBuffer.Allocate((u32)DataTypeSize(fieldMetadata.Type));
		} else {
			fieldStorage.m_ValueBuffer = Buffer::Copy(fieldMetadata.DefaultValue);
		}

		fieldStorage.m_Instance = nullptr;
	}

}
