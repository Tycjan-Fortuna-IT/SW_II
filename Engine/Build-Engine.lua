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
        "EXPORT"
    }

    includedirs
    {
        "src"
    }

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    postbuildcommands {
        "{COPY} %{wks.location}/Binaries/" .. OutputDir .. "/Engine/Engine.dll %{wks.location}/Binaries/" .. OutputDir .. "/Testbed"
    }

    filter "system:windows"
        systemversion "latest"
        defines { }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"
