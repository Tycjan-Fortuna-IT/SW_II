project "Engine"
    kind "StaticLib"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.hpp",
        "src/**.cpp"
    }

    pchheader "pch.hpp"
    pchsource "src/pch.cpp"

    forceincludes { "pch.hpp" }

    defines {
        "GLFW_INCLUDE_NONE",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
    }

    includedirs {
        "src"
    }

    IncludeEngineDependencies()
    LinkEngineDependencies()

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

    group "Engine/Dependencies"
        include "Engine/vendor/GLFW"
        include "Engine/vendor/GLAD"
        include "Engine/vendor/spdlog"
    group ""
