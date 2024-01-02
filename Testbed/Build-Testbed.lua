project "Testbed"
    kind "ConsoleApp"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "assets/**.*",
        "src/**.hpp",
        "src/**.cpp"
    }

    defines { 
        "GLFW_INCLUDE_NONE",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
    }
    links { "Engine" }

    includedirs {
        "src",
        "../Engine/src",
        "../Engine/vendor",
    }

    IncludeEngineDependencies()
    
    postbuildcommands {
        "{COPY} assets %{wks.location}/bin/" .. outputdir .. "/Testbed/assets"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "SW_WINDOWS" }

    filter "configurations:Debug"
        defines { "SW_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "SW_RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "SW_DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"
