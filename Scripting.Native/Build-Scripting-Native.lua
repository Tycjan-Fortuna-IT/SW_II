-- premake.api.addAllowed("debuggertype", "NativeWithManagedCore")

project "Scripting.Native"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "Off"

    dependson "Scripting.Managed"

    architecture "x86_64"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.cpp",
        "src/**.hpp",
    }

    includedirs { 
        "src/"
    }
    
    externalincludedirs { 
        "%{wks.location}/Scripting.Native/vendor/DotNetCore"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
        symbols "Off"
