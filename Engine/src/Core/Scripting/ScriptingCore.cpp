#include "ScriptingCore.hpp"

#include <Coral/HostInstance.hpp>
#include <Coral/TypeCache.hpp>
#include "Core/Project/ProjectContext.hpp"
#include "Core/Project/Project.hpp"

namespace SW {

	static void OnCoralMessage(std::string_view message, Coral::MessageLevel level)
	{
		switch (level) {
			case Coral::MessageLevel::Info:
				SW_INFO("CORAL: {}", std::string(message));
				break;
			case Coral::MessageLevel::Warning:
				SW_WARN("CORAL: {}", std::string(message));
				break;
			case Coral::MessageLevel::Error:
				SW_ERROR("CORAL: {}", std::string(message));
				break;
		}
	}

	static void OnCSharpException(std::string_view message)
	{
		SW_FATAL("C# Exception: {}", message);
	}

    void ScriptingCore::InitializeHost()
    {
		m_Host = new Coral::HostInstance();

		Coral::HostSettings settings =
		{
			.CoralDirectory = (std::filesystem::current_path() / "assets" / "dotnet").string(),
			.MessageCallback = OnCoralMessage,
			.ExceptionCallback = OnCSharpException
		};

		ASSERT(m_Host->Initialize(settings), "Failed to initialize C# Host");

		SW_INFO("C# host initialized successfully.");
    }

    void ScriptingCore::ShutdownHost()
    {
		Coral::TypeCache::Get().Clear();

		m_Host->Shutdown();
		delete m_Host;

		SW_INFO("C# host has been properly shut down");
    }

	void ScriptingCore::Initialize()
	{
		m_AssemblyContext = new Coral::AssemblyLoadContext(std::move(m_Host->CreateAssemblyLoadContext("MainLoadContext")));

		// TODO: Improve this
		std::string scriptCorePath = (ProjectContext::Get()->GetAssetDirectory() / "assets" / "scripts" / "src" / "binaries" / "net8.0" / "Engine.ScriptCore.dll").string();
		m_CoreAssemblyData = new AssemblyData();

		m_CoreAssemblyData->Assembly = &m_AssemblyContext->LoadAssembly(scriptCorePath);
	}

	void ScriptingCore::Shutdown()
	{
		delete m_CoreAssemblyData;
		delete m_AppAssemblyData;

		m_Host->UnloadAssemblyLoadContext(*m_AssemblyContext);

		delete m_AssemblyContext;

		Coral::TypeCache::Get().Clear();
	}

	ScriptingCore& ScriptingCore::Get()
    {
		static ScriptingCore s_Instance;

		return s_Instance;
    }

}
