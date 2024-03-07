/**
 * @file ScriptingHost.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <iostream>
#include <filesystem>

#include <coreclr_delegates.h>
#include <hostfxr.h>

#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_BG_COLOR_RED  "\x1b[41m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define TRACE(msg) std::cout << ANSI_COLOR_YELLOW << msg << ANSI_COLOR_RESET << std::endl;
#define SERROR(msg) std::cout << ANSI_BG_COLOR_RED << msg << ANSI_COLOR_RESET << std::endl;
#define BERROR(msg) { std::cout << ANSI_BG_COLOR_RED << msg << ANSI_COLOR_RESET << std::endl; return false; }

namespace SW {

	typedef void (*OnMessageCallback)(const char* message);

	struct CoreCLRFunctions
	{
		hostfxr_set_error_writer_fn SetHostFXRErrorWriter = nullptr;
		hostfxr_initialize_for_runtime_config_fn InitHostFXRForRuntimeConfig = nullptr;
		hostfxr_get_runtime_delegate_fn GetRuntimeDelegate = nullptr;
		hostfxr_close_fn CloseHostFXR = nullptr;
		load_assembly_and_get_function_pointer_fn GetManagedFunctionPtr = nullptr;
	};

	struct ScriptingHostSettings
	{
		OnMessageCallback MessageCallback = nullptr;
		std::string FXRPath;
		std::string ScriptingModulePath;
	};

	class ScriptingHost
	{
	public:
		ScriptingHost();
		~ScriptingHost();

		bool Initialize(const ScriptingHostSettings& settings);
		void Shutdown();

	private:
		ScriptingHostSettings m_Settings;
		CoreCLRFunctions m_CLRFunctions;

		void* m_HostFXRContext = nullptr;
	};

}

