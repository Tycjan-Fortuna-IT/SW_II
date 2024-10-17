FBS.Configurations = FBS.Enum { "Debug", "Release", "Dist" }

FBS.Dependencies = {
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
        Name = "spdlog",
        IncludeDirs = { "%{wks.location}/Engine/vendor/spdlog/include" },
        LibsToLink = { "spdlog" },
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
    }
}
