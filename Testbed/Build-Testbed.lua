project "Testbed"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files {
        "assets/**.*",
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

    externalincludedirs {
        "%{IncludeDir.glfw}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
    }

    links {
        "Engine"
    }

    targetdir ("../Binaries/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../Binaries/bin-int/" .. outputdir .. "/%{prj.name}")

    postbuildcommands {
        "{COPY} assets %{wks.location}/Binaries/bin" .. outputdir .. "/Testbed/assets"
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
