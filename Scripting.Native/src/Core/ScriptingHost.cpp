#include "ScriptingHost.hpp"

#include <ShlObj_core.h>

#include "HostFXRErrorCodes.hpp"

namespace SW {

	ScriptingHost::ScriptingHost()
	{

	}

	ScriptingHost::~ScriptingHost()
	{

	}

	template <typename Fn>
	static Fn GetFunctionPointer(void* lib, const char* name)
	{
		Fn result = (Fn)GetProcAddress((HMODULE)lib, name);

		if (!result) {
			char errorMessage[256];
			
			snprintf(errorMessage, sizeof(errorMessage), "Failed to load function %s", name);

			SERROR(errorMessage);
		}

		return result;
	}

	bool ScriptingHost::Initialize(const ScriptingHostSettings& settings)
	{
		if (!settings.MessageCallback)
			BERROR("Message callback must be passed to the ScriptingHost!");

		if (settings.FXRPath.empty())
			BERROR("Path to hostfxr.dll must not be empty!");

		if (settings.ScriptingModulePath.empty())
			BERROR("Path to scripting module must not be empty!");

		m_Settings = settings;

		void* handle = LoadLibraryA(m_Settings.FXRPath.c_str());

		if (!handle)
			BERROR("Failed to load hostfxr.dll library!");

		TRACE("Library hostfxr.dll found");

		m_CLRFunctions.SetHostFXRErrorWriter = GetFunctionPointer<hostfxr_set_error_writer_fn>(handle, "hostfxr_set_error_writer");
		m_CLRFunctions.InitHostFXRForRuntimeConfig = GetFunctionPointer<hostfxr_initialize_for_runtime_config_fn>(handle, "hostfxr_initialize_for_runtime_config");
		m_CLRFunctions.GetRuntimeDelegate = GetFunctionPointer<hostfxr_get_runtime_delegate_fn>(handle, "hostfxr_get_runtime_delegate");
		m_CLRFunctions.CloseHostFXR = GetFunctionPointer<hostfxr_close_fn>(handle, "hostfxr_close");

		std::filesystem::path configPath = std::filesystem::path(m_Settings.ScriptingModulePath) / "Scripting.Managed.runtimeconfig.json";

		if (!std::filesystem::exists(configPath))
			BERROR("Could not find Scripting.Managed.runtimeconfig.json file!");

		StatusCode status = (StatusCode)m_CLRFunctions.InitHostFXRForRuntimeConfig(configPath.c_str(), nullptr, &m_HostFXRContext);

		if (
			status != StatusCode::Success &&
			status != StatusCode::Success_HostAlreadyInitialized &&
			status != StatusCode::Success_DifferentRuntimeProperties
		) {
			BERROR("Host failed to initialize!");
		}

		status = (StatusCode)m_CLRFunctions.GetRuntimeDelegate(
			m_HostFXRContext, hdt_load_assembly_and_get_function_pointer, (void**)&m_CLRFunctions.GetManagedFunctionPtr
		);

		if (status != StatusCode::Success)
			BERROR("Runtime delegate failed to load!");

		return true;
	}

	void ScriptingHost::Shutdown()
	{

	}

}
