project "Scripting.Example.Native"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"
    staticruntime "Off"
    debuggertype "NativeWithManagedCore"

    architecture "x86_64"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.cpp",
        "src/**.hpp",
    }


    externalincludedirs { "../../Scripting.Native/src/" }

	-- postbuildcommands {
	-- 	'{COPYFILE} "%{wks.location}/Coral.Managed/Coral.Managed.runtimeconfig.json" "%{cfg.targetdir}"',
	-- }

	links {
		"Scripting.Native",
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

