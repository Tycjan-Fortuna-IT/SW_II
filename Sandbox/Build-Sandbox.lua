project "Sandbox"
	kind "SharedLib"
	language "C#"
	dotnetframework "net8.0"

	targetdir ("assets/scripts/src/binaries")
	objdir ("assets/scripts/src/intermediates")

	propertytags {
        { "AppendTargetFrameworkToOutputPath", "false" },
        { "Nullable", "enable" },
    }

	files  {
		"assets/scripts/src/**.cs", 
	}

	links {
		"Engine.ScriptCore"
	}

	postbuildcommands {
		"{COPY} %{wks.location}/EngineEditor/assets %{wks.location}/bin/" .. outputdir .. "/EngineEditor/assets",
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
