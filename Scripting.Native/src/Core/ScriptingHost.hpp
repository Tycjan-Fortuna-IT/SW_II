#pragma once

#include <iostream>
#include <filesystem>

#include <coreclr_delegates.h>
#include <hostfxr.h>

#include "HostFXRErrorCodes.hpp"

#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_BG_COLOR_RED  "\x1b[41m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define TRACE(msg) std::cout << ANSI_COLOR_YELLOW << msg << ANSI_COLOR_RESET << std::endl;
#define SERROR(msg) std::cout << ANSI_BG_COLOR_RED << msg << ANSI_COLOR_RESET << std::endl;
#define BERROR(msg) { std::cout << ANSI_BG_COLOR_RED << msg << ANSI_COLOR_RESET << std::endl; return false; }

#ifdef _WCHAR_T_DEFINED
	#define SW_STR(s) L##s
	#define SW_WIDE_CHARS

	using CharType = wchar_t;
	using StringView = std::wstring_view;
#else
	#define SW_STR(s) s

	using CharType = unsigned short;
	using StringView = std::string_view;
#endif

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
		std::filesystem::path FXRPath;
		std::filesystem::path ScriptingModulePath;
	};

	class ScriptingHost
	{
	public:
		ScriptingHost();
		~ScriptingHost();

		bool Initialize(const ScriptingHostSettings& settings);
		void Shutdown();

		template<typename FnPtr>
		FnPtr LoadManagedFunctionPtr(const CharType* type, const CharType* method) const
		{
			void* funcPtr = nullptr;

			int status = m_CLRFunctions.GetManagedFunctionPtr(
				m_MainManagedLibrarypath.c_str(), type, method, UNMANAGEDCALLERSONLY_METHOD, nullptr, &funcPtr
			);
			
			if (status != StatusCode::Success && funcPtr != nullptr)
				SERROR("Managed function not found!");
			
			return (FnPtr)funcPtr;
		}

	private:
		ScriptingHostSettings m_Settings;
		CoreCLRFunctions m_CLRFunctions;

		void* m_HostFXRContext = nullptr;
		std::filesystem::path m_MainManagedLibrarypath;
	};

}

