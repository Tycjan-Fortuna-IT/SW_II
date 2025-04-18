#include "ScriptingCore.hpp"

#include "Core/Hash.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Scripting/InternalCallManager.hpp"
#include <Coral/HostInstance.hpp>
#include <Coral/TypeCache.hpp>

namespace SW
{

	static std::unordered_map<std::string, DataType> s_DataTypeLookup = {
	    {"System.Byte", DataType::Byte},    {"System.Int16", DataType::Short},  {"System.UInt16", DataType::UShort},
	    {"System.Int32", DataType::Int},    {"System.UInt32", DataType::UInt},  {"System.Int64", DataType::Long},
	    {"System.UInt64", DataType::ULong}, {"System.Single", DataType::Float}, {"System.Double", DataType::Double},
	    {"System.Boolean", DataType::Bool}, {"SW.Entity", DataType::Entity},    {"SW.Prefab", DataType::Prefab},
	};

	static void OnCoralMessage(std::string_view message, Coral::MessageLevel level)
	{
		switch (level)
		{
		case Coral::MessageLevel::Info:
			SYSTEM_INFO("CORAL: {}", std::string(message));
			break;
		case Coral::MessageLevel::Warning:
			SYSTEM_WARN("CORAL: {}", std::string(message));
			break;
		case Coral::MessageLevel::Error:
			SYSTEM_ERROR("CORAL: {}", std::string(message));
			break;
		}
	}

	static void OnCSharpException(std::string_view message)
	{
		SYSTEM_FATAL("C# Exception: {}", message);
	}

	void ScriptingCore::InitializeHost()
	{
		m_Host = new Coral::HostInstance();

		Coral::HostSettings settings = {.CoralDirectory =
		                                    (std::filesystem::current_path() / "assets" / "dotnet").string(),
		                                .MessageCallback   = OnCoralMessage,
		                                .ExceptionCallback = OnCSharpException};

		ASSERT(m_Host->Initialize(settings), "Failed to initialize C# Host");

		SYSTEM_INFO("C# host initialized successfully.");
	}

	void ScriptingCore::ShutdownHost()
	{
		Coral::TypeCache::Get().Clear();

		m_Host->Shutdown();
		delete m_Host;

		SYSTEM_INFO("C# host has been properly shut down");
	}

	void ScriptingCore::Initialize()
	{
		m_AssemblyContext = new Coral::AssemblyLoadContext(m_Host->CreateAssemblyLoadContext("MainLoadContext"));

		// TODO: Improve this
		std::string scriptCorePath =
		    (std::filesystem::current_path() / "assets" / "dotnet" / "Engine.ScriptCore.dll").string();
		m_CoreAssemblyData = new AssemblyData();

		m_CoreAssemblyData->Assembly = &m_AssemblyContext->LoadAssembly(scriptCorePath);
		BuildAssemblyCache(m_CoreAssemblyData);

		InternalCallManager::Initialize(m_CoreAssemblyData->Assembly);

		LoadProjectAssembly();
	}

	void ScriptingCore::Shutdown()
	{
		delete m_CoreAssemblyData;
		delete m_AppAssemblyData;

		m_Host->UnloadAssemblyLoadContext(*m_AssemblyContext);

		delete m_AssemblyContext;

		Coral::TypeCache::Get().Clear();
	}

	void ScriptingCore::BuildAssemblyCache(AssemblyData* assemblyData)
	{
		std::vector<Coral::Type*> types = assemblyData->Assembly->GetTypes();
		Coral::Type entityType          = assemblyData->Assembly->GetType("SW.Entity");

		for (Coral::Type* type : types)
		{
			std::string fullName = type->GetFullName();
			u32 scriptID         = Hash::GenerateFNVHash(fullName);

			assemblyData->CachedTypes[scriptID] = type;

			if (!type->IsSubclassOf(entityType))
				continue;

			ScriptMetadata& metadata = m_ScriptMetadata[scriptID];
			metadata.FullName        = fullName;

			Coral::ManagedObject temp = type->CreateInstance();

			for (Coral::FieldInfo& fieldInfo : type->GetFields())
			{
				Coral::ScopedString fieldName = fieldInfo.GetName();
				std::string fieldNameStr      = fieldName;

				if (fieldNameStr.find("k__BackingField") != std::string::npos)
					continue;

				Coral::Type* fieldType = &fieldInfo.GetType();

				if (fieldType->IsSZArray())
				{
					fieldType = &fieldType->GetElementType();
				}

				Coral::ScopedString typeName = fieldType->GetFullName();

				if (!s_DataTypeLookup.contains(typeName))
					continue;

				// if (fieldInfo.GetAccessibility() != Coral::TypeAccessibility::Public)
				//	continue;

				// Entity.ID bleeds through to the inheriting scripts
				if (fieldNameStr == "ID")
					continue;

				std::string fullFieldName = std::format("{}.{}", fullName, fieldNameStr);
				u32 fieldID               = Hash::GenerateFNVHash(fullFieldName);

				FieldMetadata& fieldMetadata = metadata.Fields[fieldID];
				fieldMetadata.Name           = fieldName;
				fieldMetadata.Type           = s_DataTypeLookup.at(typeName);
				fieldMetadata.ManagedType    = &fieldInfo.GetType();

				switch (fieldMetadata.Type)
				{
				case DataType::Byte:
					fieldMetadata.SetDefaultValue<u8>(temp);
					break;
				case DataType::Short:
					fieldMetadata.SetDefaultValue<i16>(temp);
					break;
				case DataType::UShort:
					fieldMetadata.SetDefaultValue<i16>(temp);
					break;
				case DataType::Int:
					fieldMetadata.SetDefaultValue<i32>(temp);
					break;
				case DataType::UInt:
					fieldMetadata.SetDefaultValue<u32>(temp);
					break;
				case DataType::Long:
					fieldMetadata.SetDefaultValue<i64>(temp);
					break;
				case DataType::ULong:
					fieldMetadata.SetDefaultValue<u64>(temp);
					break;
				case DataType::Float:
					fieldMetadata.SetDefaultValue<f32>(temp);
					break;
				case DataType::Double:
					fieldMetadata.SetDefaultValue<f64>(temp);
					break;
				case DataType::Bool:
				case DataType::Entity:
				case DataType::Prefab:
					break;
				default:
					break;
				}
			}

			temp.Destroy();
		}
	}

	bool ScriptingCore::IsValidScript(u64 scriptID) const
	{
		if (!m_AppAssemblyData)
			return false;

		return m_AppAssemblyData->CachedTypes.contains(scriptID) && m_ScriptMetadata.contains(scriptID);
	}

	const SW::ScriptMetadata& ScriptingCore::GetScriptMetadata(u64 scriptID) const
	{
		ASSERT(m_ScriptMetadata.contains(scriptID), "Missing script data!");

		return m_ScriptMetadata.at(scriptID);
	}

	void ScriptingCore::LoadProjectAssembly()
	{
		m_AppAssemblyData = new AssemblyData();

		Project* currentProject = ProjectContext::Get();

		std::string library = currentProject->GetName() + ".dll";

		std::string scriptAppPath = (currentProject->GetAssetDirectory() / "build" / library).string();

		m_AppAssemblyData->Assembly = &m_AssemblyContext->LoadAssembly(scriptAppPath);

		ASSERT(m_AppAssemblyData->Assembly->GetLoadStatus() == Coral::AssemblyLoadStatus::Success,
		       "Failed to load project's c# assembly dll!");

		BuildAssemblyCache(m_AppAssemblyData);
	}

	const Coral::Type* ScriptingCore::GetTypeByName(std::string_view name) const
	{
		for (const auto& [id, type] : m_CoreAssemblyData->CachedTypes)
		{
			Coral::String typeName = type->GetFullName();

			if (typeName == name)
			{
				Coral::String::Free(typeName);

				return type;
			}

			Coral::String::Free(typeName);
		}

		for (const auto& [id, type] : m_AppAssemblyData->CachedTypes)
		{
			Coral::String typeName = type->GetFullName();

			if (typeName == name)
			{
				Coral::String::Free(typeName);

				return type;
			}

			Coral::String::Free(typeName);
		}

		ASSERT(false, "Type not found!");

		return nullptr;
	}

	ScriptingCore& ScriptingCore::Get()
	{
		static ScriptingCore s_Instance;

		return s_Instance;
	}

} // namespace SW
