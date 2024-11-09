FBS.Configurations = FBS.Enum { "Debug", "Release", "Dist" }

FBS.Dependencies = {
    {
        name = "Defines",
        Defines = { 
            "IMGUI_DEFINE_MATH_OPERATORS",
            "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
            "YAML_CPP_STATIC_DEFINE",
            "TRACY_ENABLE",
            "TRACY_ON_DEMAND",
            "TRACY_CALLSTACK=10",
        },
    },
    {
        Name = "ENTT",
        IncludeDirs = { "%{wks.location}/Engine/vendor/entt" },
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

    FBS.ImportModule("Engine/modules/Logger");
    FBS.ImportModule("Engine/modules/Eventing");
    FBS.ImportModule("Engine/modules/OpenGL");
}
