project "Engine"
    kind "StaticLib"
    architecture "x86_64"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.hpp",
        "src/**.cpp",

        "vendor/ImGui/backends/imgui_impl_glfw.cpp",
        "vendor/ImGui/backends/imgui_impl_opengl3.cpp",
    
        "vendor/ImGuizmo/ImGuizmo.h",
        "vendor/ImGuizmo/ImGuizmo.cpp",
    }

    pchheader "pch.hpp"
    pchsource "src/pch.cpp"

    forceincludes { "pch.hpp" }

    defines {
        "GLFW_INCLUDE_NONE",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
        "YAML_CPP_STATIC_DEFINE",
        "TRACY_ENABLE",
		"TRACY_ON_DEMAND",
        "TRACY_CALLSTACK=10",
    }

    includedirs {
        "src"
    }

    IncludeEngineDependencies()
    LinkEngineDependencies()
    
    filter "system:windows"
        buildoptions {
            -- This option enables the compiler to generate code that's compliant with the C++20 for char8_t literals.
            "/Zc:char8_t-"
        }

    filter "system:linux"
        pic "On"
        systemversion "latest"
        buildoptions { "`pkg-config --cflags gtk+-3.0`" }
        linkoptions { "`pkg-config --libs gtk+-3.0`" }
        links {
            "GL:shared",
            "dl:shared",
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

    group "Engine/Dependencies"
        include "Engine/vendor/GLFW"
        include "Engine/vendor/GLAD"
        include "Engine/vendor/spdlog"
        include "Engine/vendor/ImGui"
        include "Engine/vendor/YamlCpp"
        include "Engine/vendor/NFD-Extended"
        include "Engine/vendor/Box2D"
        include "Engine/vendor/msdf-atlas-gen"
        include "Engine/vendor/tracy"
        include "Engine/vendor/Coral/Coral.Native"
        include "Engine/vendor/Coral/Coral.Managed"
    group ""
    
    group "Engine"
        include "Engine.ScriptCore/Build-ScriptCore.lua"
        -- include "EngineTest/Build-EngineTest.lua"
    group ""
