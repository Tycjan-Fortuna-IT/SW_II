project "Engine"
    kind "StaticLib"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.hpp",
        "src/**.cpp",
        "vendor/ImGui/backends/imgui_impl_glfw.cpp",
        "vendor/ImGui/backends/imgui_impl_opengl3.cpp",
    }

    pchheader "pch.hpp"
    pchsource "src/pch.cpp"

    forceincludes { "pch.hpp" }

    defines {
        "GLFW_INCLUDE_NONE",
        "IMGUI_DEFINE_MATH_OPERATORS",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
        "YAML_CPP_STATIC_DEFINE",
    }

    includedirs {
        "src"
    }

    IncludeEngineDependencies()
    LinkEngineDependencies()
    
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

    group "Engine/Dependencies"
        include "Engine/vendor/GLFW"
        include "Engine/vendor/GLAD"
        include "Engine/vendor/spdlog"
        include "Engine/vendor/ImGui"
        include "Engine/vendor/YamlCpp"
    group ""
    
    group "Engine"
        include "EngineTest/Build-EngineTest.lua"
    group ""

