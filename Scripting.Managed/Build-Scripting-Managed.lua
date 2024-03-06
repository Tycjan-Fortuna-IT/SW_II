include "../PropertyTags.lua"

project "Scripting.Managed"
    language "C#"
    dotnetframework "net8.0"
    kind "SharedLib"
	clr "Unsafe"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    property_tags {
        { "AppendTargetFrameworkToOutputPath", "false" },
        { "Nullable", "enable" },
    }

    disablewarnings {
        "CS8500"
    }

    files {
        "src/**.cs"
    }
