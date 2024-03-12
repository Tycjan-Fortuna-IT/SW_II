include "../PropertyTags.lua"

workspace "Sandbox"
	startproject "Sandbox"
	configurations { "Debug", "Release", "Dist" }

group "Engine"

include "../Engine.ScriptCore/Build-ScriptCore.lua"

group ""

project "Sandbox"
	kind "SharedLib"
	language "C#"
	dotnetframework "net8.0"
	dependson { "Engine.ScriptCore" }

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
		"../EngineEditor/assets/dotnet/Coral.Managed.dll",
        "../EngineEditor/assets/dotnet/Engine.ScriptCore.dll",
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
