-- When adding a new dependency, add it to the table below, where:
-- - The key is the name of the dependency
-- - The value is a table containing the following:
--   - IncludeDir: The directory containing the headers of the dependency
--   - LinkAs: The name of the library to link to (if compiled as a static library)
Dependencies = {
    GLFW = { -- premake handles this library's directory automatically
        IncludeDir = "%{wks.location}/Engine/vendor/GLFW/include",
        LinkAs = "GLFW",
    },
    GLAD = { -- premake handles this library's directory automatically
        IncludeDir = "%{wks.location}/Engine/vendor/GLAD/include",
        LinkAs = "GLAD",
    },
    ENTT = {
        IncludeDir = "%{wks.location}/Engine/vendor/entt",
    },
    STB_IMAGE = {
        IncludeDir = "%{wks.location}/Engine/vendor/stb_image",
    },
    SPDLOG = { -- premake handles this library's directory automatically
        IncludeDir = "%{wks.location}/Engine/vendor/spdlog/include",
        LinkAs = "spdlog",
    },
    IMGUI = {
        IncludeDir = "%{wks.location}/Engine/vendor/ImGui",
        LinkAs = "ImGui",
    },
    MATERIAL_ICONS = {
        IncludeDir = "%{wks.location}/Engine/vendor/icons/include",
    },
    YAML_CPP = {
        IncludeDir = "%{wks.location}/Engine/vendor/YamlCpp/include",
        LinkAs = "yaml-cpp",
    },
    NFD_EXTENDED = {
        IncludeDir = "%{wks.location}/Engine/vendor/NFD-Extended/src/include",
        LinkAs = "NFD-Extended",
    },
    BOX2D = {
        IncludeDir = "%{wks.location}/Engine/vendor/box2d/include",
        LinkAs = "Box2D",
    },
    IMGUIZMO = {
        IncludeDir = "%{wks.location}/Engine/vendor/ImGuizmo",
    },
    GLM = {
        IncludeDir = "%{wks.location}/Engine/vendor/glm",
    },
    MSDF_ATLAS_GEN = {
        IncludeDir = "%{wks.location}/Engine/vendor/msdf-atlas-gen",
        LinkAs = "msdf-atlas-gen",
    },
    MSDFGEN = {
        IncludeDir = "%{wks.location}/Engine/vendor/msdf-atlas-gen/msdfgen",
        LinkAs = "msdfgen",
    },
    FREETYPE = {
        IncludeDir = "%{wks.location}/Engine/vendor/msdf-atlas-gen/msdfgen/freetype/include",
        LinkAs = "freetype",
    },
}

function IncludeEngineDependencies(additional)
    includetable = {}

    for name, dependency in pairs(Dependencies) do
        table.insert(includetable, dependency.IncludeDir)
    end

    if additional == nil then additional = {} end

    for name, dependency in pairs(additional) do
        table.insert(includetable, dependency)
    end

    includedirs(includetable)
end

function LinkEngineDependencies()
    linktable = {}

    for name, dependency in pairs(Dependencies) do
        table.insert(linktable, dependency.LinkAs)
    end

    links(linktable)
end