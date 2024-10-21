FBS.Configurations = FBS.Enum { "Debug", "Release", "Dist" }

FBS.Dependencies = {
    {
        name = "Defines",
        Defines = { 
            "GLFW_INCLUDE_NONE",
            "IMGUI_DEFINE_MATH_OPERATORS",
            "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
            "YAML_CPP_STATIC_DEFINE",
            "TRACY_ENABLE",
            "TRACY_ON_DEMAND",
            "TRACY_CALLSTACK=10",
        },
    },
    {
        Name = "GLFW",
        IncludeDirs = { "%{wks.location}/Engine/vendor/GLFW/include" },
        LibsToLink = { "GLFW" },
    },
    {
        Name = "GLAD",
        IncludeDirs = { "%{wks.location}/Engine/vendor/GLAD/include" },
        LibsToLink = { "GLAD" },
    },
    {
        Name = "ENTT",
        IncludeDirs = { "%{wks.location}/Engine/vendor/entt" },
    },
    {
        Name = "STB Image",
        IncludeDirs = { "%{wks.location}/Engine/vendor/stb_image" },
    },
    {
        Name = "ImGui",
        IncludeDirs = { "%{wks.location}/Engine/vendor/ImGui" },
        LibsToLink = { "ImGui" },
    },
    {
        Name = "ImGuizmo",
        IncludeDirs = { "%{wks.location}/Engine/vendor/ImGuizmo" },
    },
    {
        Name = "ImGui Node Editor",
        IncludeDirs = { "%{wks.location}/Engine/vendor/imgui-node-editor" },
    },
    {
        Name = "Material Icons",
        IncludeDirs = { "%{wks.location}/Engine/vendor/icons/include" },
    },
    {
        Name = "Yaml CPP",
        IncludeDirs = { "%{wks.location}/Engine/vendor/YamlCpp/include" },
        LibsToLink = { "yaml-cpp" },
    },
    {
        Name = "NFD Extended",
        IncludeDirs = { "%{wks.location}/Engine/vendor/NFD-Extended/src/include" },
        LibsToLink = { "NFD-Extended" },
    },
    {
        Name = "Box 2D",
        IncludeDirs = { "%{wks.location}/Engine/vendor/box2d/include" },
        LibsToLink = { "Box2D" },
    },
    {
        Name = "GLM",
        IncludeDirs = { "%{wks.location}/Engine/vendor/glm" },
    },
    {
        Name = "msdfgen",
        IncludeDirs = { "%{wks.location}/Engine/vendor/msdf-atlas-gen/msdfgen" },
        LibsToLink = { "msdfgen" },
    },
    {
        Name = "msdf-atlas-gen",
        IncludeDirs = { "%{wks.location}/Engine/vendor/msdf-atlas-gen" },
        LibsToLink = { "msdf-atlas-gen" },
    },
    {
        Name = "freetype",
        IncludeDirs = { "%{wks.location}/Engine/vendor/msdf-atlas-gen/msdfgen/freetype/include" },
        LibsToLink = { "freetype" },
    },
    {
        Name = "Tracy",
        IncludeDirs = { "%{wks.location}/Engine/vendor/tracy/tracy/public" },
        LibsToLink = { "Tracy" },
        Windows = {
            LibsToLink = { "ws2_32", "Dbghelp" },
        },
    },
    {
        Name = "Coral",
        IncludeDirs = { "%{wks.location}/Engine/vendor/Coral/Coral.Native/Include" },
        LibsToLink = { "Coral.Native" },
    },
    {
        Name = "MiniAudio",
        IncludeDirs = { "%{wks.location}/Engine/vendor/miniaudio" },
    },

    -- SW Modules
    {
		Name = "SW Logger Module",
		LibsToLink = { "Logger" },
		IncludeDirs = {
			"%{wks.location}/Engine/modules/Logger/src",
			"%{wks.location}/Engine/modules/Logger/vendor/spdlog/include"
		},
	},
}
