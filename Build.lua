include "SolutionItems.lua"
include "PropertyTags.lua"
include "Dependencies.lua"

workspace "SW_II"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }
    startproject "EngineEditor"
    conformancemode "On"
    language "C++"
	cppdialect "C++20"
	staticruntime "Off"
	externalwarnings "off"
	rtti "off"

    defines {
        -- Disable warnings for using unsafe functions
		"_CRT_SECURE_NO_WARNINGS",

        -- Disable warnings for using deprecated functions
		"_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
	}

    -- Include editorconfig file for consistent coding style
    solution_items { ".editorconfig" }

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

            -- This option enables the compiler to generate code that's compliant with the C++20 for char8_t literals.
            "/Zc:char8_t-"
        }

    filter "system:linux"
        defines { "SW_LINUX" }
        toolset "clang"
        buildoptions {
            "-fno-char8_t"
        }

-- Folder name containing compiled output in a following format: /[config]-[platform]-[architecture]/.
-- e.g. folder Windows-x64/Debug
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Engine"
	include "Engine/Build-Engine.lua"
group ""

group "Scripting"
    include "Scripting.Managed/Build-Scripting-Managed.lua"
    include "Scripting.Native/Build-Scripting-Native.lua"
    include "Scripting.Example/Managed/Build-Scripting-Example-Managed.lua"
    include "Scripting.Example/Native/Build-Scripting-Example-Native.lua"
group ""

include "EngineEditor/Build-EngineEditor.lua"
