project "Engine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files { "src/**.hpp", "src/**.cpp" }

    -- pchheader "pch.hpp"
    -- pchsource "src/pch.cpp"

    defines
    {
        "EXPORT",
        "GLFW_INCLUDE_NONE",
    }

    includedirs
    {
        "src"
    }

    externalincludedirs {
        "%{IncludeDir.glfw}",
        "%{IncludeDir.glad}",
    }

    links {
        "glfw",
        "glad",
    }

    targetdir ("../Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")

    postbuildcommands {
        "{COPY} %{wks.location}/Binaries/" .. outputdir .. "/Engine/Engine.dll %{wks.location}/Binaries/" .. outputdir .. "/Testbed"
    }

    filter "system:windows"
        systemversion "latest"
        defines { }

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
    group ""
