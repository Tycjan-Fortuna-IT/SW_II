project "Testbed"
    kind "ConsoleApp"
    architecture "x86_64"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "assets/**.*",
        "assets/**.embed",
        "src/**.hpp",
        "src/**.cpp"
    }

    pchheader "pch.hpp"
    pchsource "src/pch.cpp"

    forceincludes { "pch.hpp" }

    defines { 
        "GLFW_INCLUDE_NONE",
        "IMGUI_DEFINE_MATH_OPERATORS",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
        "YAML_CPP_STATIC_DEFINE",
        "TRACY_ENABLE",
		"TRACY_ON_DEMAND",
        "TRACY_CALLSTACK=10",
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
    
    filter "system:windows"
	    linkoptions { "/NODEFAULTLIB:LIBCMT" }
        buildoptions {
            -- This option enables the compiler to generate code that's compliant with the C++20 for char8_t literals.
            "/Zc:char8_t-"
        }

    filter "system:linux"
        pic "On"
        systemversion "latest"
        linkoptions { "`pkg-config --libs gtk+-3.0`" }
        links {
            "GL:shared",
            "dl:shared"
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
