project "Testbed"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files {
        "assets/**.glsl",
        "src/**.hpp",
        "src/**.cpp"
    }

    defines {
        "GLFW_INCLUDE_NONE",
    }

    includedirs {
        "src",
        "../Engine/src"
    }

    externalincludedirs
    {
        "%{IncludeDir.glfw}",
        "%{IncludeDir.glad}",
    }

    links
    {
        "Engine"
    }

    targetdir ("../Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")

    postbuildcommands {
        "{COPY} assets %{wks.location}/Binaries/" .. outputdir .. "/Testbed/assets"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "WINDOWS" }

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
