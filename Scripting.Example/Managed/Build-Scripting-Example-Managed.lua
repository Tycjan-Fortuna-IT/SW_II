project "Scripting.Example.Managed"
    language "C#"
    dotnetframework "net8.0"
    kind "SharedLib"
	clr "Unsafe"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")
    
    property_tags {
        { "AppendTargetFrameworkToOutputPath", "false" },
        { "Nullable", "enable" }
    }

    files {
        "src/**.cs"
    }
    
    links { 
        "Scripting.Managed"
    }
