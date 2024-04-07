include "../Engine/vendor/Coral/Premake/CSExtensions.lua"

workspace "Sandbox"
	startproject "Sandbox"
	configurations { "Debug", "Release", "Dist" }

project "Sandbox"
	kind "SharedLib"
	language "C#"
	dotnetframework "net8.0"
	dependson { "Engine.ScriptCore", "Coral.Managed" }

	targetdir ("assets/build")
	objdir ("obj")

	propertytags {
        { "AppendTargetFrameworkToOutputPath", "false" },
        { "Nullable", "enable" },
    }

	files  {
		"assets/scripts/**.cs", 
	}

	links {
		"Coral.Managed",
        "Engine.ScriptCore",
	}

	postbuildcommands {
		-- "{COPY} %{wks.location}/EngineEditor/assets %{wks.location}/bin/" .. outputdir .. "/EngineEditor/assets",
 	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

	group "Engine"
		include "../Engine.ScriptCore/Build-ScriptCore.lua"
	group ""
