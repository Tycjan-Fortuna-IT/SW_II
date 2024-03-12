project "Engine.ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "net8.0"
	clr "Unsafe"
	dependson { "Coral.Managed" }

	targetdir ("../EngineEditor/assets/dotnet")
	objdir ("../EngineEditor/assets/dotnet/intermediates")

    links {
		"../EngineEditor/assets/dotnet/Coral.Managed.dll",
    }

	propertytags {
        { "AppendTargetFrameworkToOutputPath", "false" },
        { "Nullable", "enable" },
    }

	files {
		"src/**.cs",
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