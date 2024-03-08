project "Engine.ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "net8.0"
	clr "Unsafe"

	targetdir ("../EngineEditor/assets/scripts")
	objdir ("../EngineEditor/assets/scripts/intermediates")

    links {
        "../EngineEditor/assets/dotnet/Coral.Managed.dll"
    }

	property_tags {
        { "AppendTargetFrameworkToOutputPath", "false" },
        { "Nullable", "enable" },
    }

	files {
		"src/**.cs",
	}