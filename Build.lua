include "SolutionItems.lua"
include "Dependencies.lua"

workspace "SW_II"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Testbed"
    conformancemode "On"
    language "C++"
	cppdialect "C++20"
	staticruntime "Off"

    -- Include editorconfig file for consistent coding style
    solution_items { ".editorconfig" }

    -- Enable Visual Studio to use multiple compiler processes when building.
    flags { "MultiProcessorCompile" }

    -- Workspace-wide build options for MSVC
    filter "system:windows"
        buildoptions {
            -- This option specifies the model of exception handling to be used by the compiler.
            -- "EHsc" is the standard C++ exception handling model.
            "/EHsc",

            -- This option enforces compliance with the C++ preprocessor standard.
            "/Zc:preprocessor",

            -- This option enables the __cplusplus macro to report an accurate value for the C++
            -- language standard supported by the compiler.
            "/Zc:__cplusplus"
        }

    -- Folder name containing compiled output in a following format: /[config]-[platform]-[architecture]/.
    -- e.g. folder Windows-x64/Debug
    outputdir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Engine"
	include "Engine/Build-Engine.lua"
group ""

include "Testbed/Build-Testbed.lua"
