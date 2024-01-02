-- When adding a new dependency, add it to the table below, where:
-- - The key is the name of the dependency
-- - The value is a table containing the following:
--   - IncludeDir: The directory containing the headers of the dependency
--   - LinkAs: The name of the library to link to (if compiled as a static library)
Dependencies = {
    GLFW = { -- premake handles this library's directory automatically
        IncludeDir = "%{wks.location}/Engine/vendor/glfw/include",
        LinkAs = "glfw",
    },
    GLAD = { -- premake handles this library's directory automatically
        IncludeDir = "%{wks.location}/Engine/vendor/glad/include",
        LinkAs = "glad",
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
}

function IncludeEngineDependencies()
    includetable = {}

    for name, dependency in pairs(Dependencies) do
        table.insert(includetable, dependency.IncludeDir)
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