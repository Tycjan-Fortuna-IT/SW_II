return function(basePath)
    return {
        Name = "SW Eventing Module",
        LibsToLink = { "Eventing" },
        IncludeDirs = {
            basePath .. "/src",
        },
    }
end
