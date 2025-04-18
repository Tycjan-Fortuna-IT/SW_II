include "vendor/FancyBuildSystem/FancyBuildSystem.lua"
include "Dependencies.lua"

workspace "SW_II"
    configurations { "Debug", "Release", "Dist" }
    startproject "EngineEditor"
    conformancemode "On"
    language "C++"
	cppdialect "C++20"
	staticruntime "Off"

    externalwarnings "Off"
    warnings "Off"
    externalanglebrackets "On"

    -- using all warnings with those exceptions:
    -- they are disabled because they are not relevant for the project and are too noisy
    disablewarnings {
        "4820", -- disable padding warnings (might be useful to enable in the future)
        "4505", -- unreferenced inline function has been removed
        "4514", -- unreferenced inline function has been removed
        "5045", -- Spectre mitigation warning
        "4868", -- compiler may not enforce left-to-right evaluation order in braced initializer list
        "4061", -- enumerator in switch of enum is not explicitly handled by a case label
        "4062", -- enumerator in switch of enum is not handled
        "5039", -- pointer or reference to potentially throwing function passed to extern C function under -EHc
        "4625", -- copy constructor was implicitly defined as deleted
        "4626", -- assignment operator was implicitly defined as deleted
        "5026", -- move constructor was implicitly defined as deleted
        "5027", -- move assignment operator was implicitly defined as deleted
    }

	-- rtti "off"

    defines {
        -- Disable warnings for using unsafe functions
		"_CRT_SECURE_NO_WARNINGS",

        -- Disable warnings for using deprecated functions
		"_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
	}

    -- Include editorconfig file for consistent coding style
    solution_items { ".clang-format", ".clang-tidy" }

    -- Enable Visual Studio to use multiple compiler processes when building.
    flags { "MultiProcessorCompile" }

    -- Workspace-wide build options for MSVC
    filter "system:windows"
        systemversion "latest"
        defines { "SW_WINDOWS" }
        buildoptions {
            -- This option specifies the model of exception handling to be used by the compiler.
            -- "EHsc" is the standard C++ exception handling model.
            "/EHsc",

            -- This option enforces compliance with the C++ preprocessor standard.
            "/Zc:preprocessor",

            -- This option enables the __cplusplus macro to report an accurate value for the C++
            -- language standard supported by the compiler.
            "/Zc:__cplusplus",
        }

    filter "system:linux"
        defines { "SW_LINUX" }
        toolset "clang"
        buildoptions {
            "-fno-char8_t"
        }

    filter "language:C++ or language:C"
		architecture "x86_64"

-- Folder name containing compiled output in a following format: /[config]-[platform]-[architecture]/.
-- e.g. folder Windows-x64/Debug
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Engine"
	include "Engine/Build-Engine.lua"
group ""

include "EngineEditor/Build-EngineEditor.lua"
include "Testbed/Build-Testbed.lua"