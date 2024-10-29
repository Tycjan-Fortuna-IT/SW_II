#include <Entrypoint.hpp>

#include "icons/embed/SW_Icon.embed"

#include "EngineEditor.hpp"

SW::Application* CreateApplication()
{
	return new EngineEditor({
	    .Title          = "SW Editor",
	    .Width          = 1280,
	    .Height         = 720,
	    .VSync          = false,
	    .Icon           = {.Data = e_SW_Icon, .Size = sizeof(e_SW_Icon)},
	    .DisableToolbar = true,
	    .Fullscreen     = true,
	});
}
