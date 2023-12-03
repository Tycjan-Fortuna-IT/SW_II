workspace "SW_II"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Testbed"

    -- Workspace-wide build options for MSVC
    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }


    -- Folder name containing compiled output in a following format: /[config]-[platform]-[architecture]/.
    -- e.g. folder Windows-x64/Debug
    outputdir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

    -- List of directories leading to already compiled libraries that are in use
    LibDir = {}
    LibDir["glfw"] = "%{wks.location}/Engine/vendor/glfw/" .. outputdir .. "/glfw/glfw3.lib"
    LibDir["glad"] = "%{wks.location}/Engine/vendor/glad/" .. outputdir .. "/glad/glad.lib"

    -- List of directories leading to headers of the libraries in use
    IncludeDir = {}
    IncludeDir["glfw"] = "%{wks.location}/Engine/vendor/glfw/include"
    IncludeDir["glad"] = "%{wks.location}/Engine/vendor/glad/include"

group "Engine"
	include "Engine/Build-Engine.lua"
group ""

include "Testbed/Build-Testbed.lua"
