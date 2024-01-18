project "EngineEditor"
    kind "ConsoleApp"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "assets/**.*",
        "assets/**.embed",
        "src/**.hpp",
        "src/**.cpp"
    }

    defines { 
        "GLFW_INCLUDE_NONE",
        "IMGUI_DEFINE_MATH_OPERATORS",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
    }
    links { "Engine" }

    includedirs {
        "assets",
        "src",
        "../Engine/src",
        "../Engine/vendor",
    }

    IncludeEngineDependencies()
    LinkEngineDependencies()

    postbuildcommands {
        "{COPY} assets %{wks.location}/bin/" .. outputdir .. "/EngineEditor/assets"
    }

    filter "configurations:Debug"
        defines { "SW_DEBUG_BUILD" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "SW_RELEASE_BUILD" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "SW_DIST_BUILD" }
        runtime "Release"
        optimize "On"
        symbols "Off"
