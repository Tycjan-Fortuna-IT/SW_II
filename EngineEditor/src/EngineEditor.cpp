#include <Engine.hpp>
#include <Entrypoint.hpp>

#include "EditorLayer.hpp"

#include "icons/embed/SW_Icon.embed"

class EngineEditor final : public SW::Application
{
public:
    explicit EngineEditor(const SW::ApplicationSpecification& specification)
        : SW::Application(specification) {

        this->PushLayer(new SW::EditorLayer("Test"));
    }

protected:
    ~EngineEditor() override {
        this->PopLayer();
    }
};

SW::Application* CreateApplication() {
    SW::ApplicationSpecification spec = {
        .Title = "SW Editor",
        .Width = 1280,
        .Height = 720,
        .VSync = false,
		.Icon = {
			.Data = e_SW_Icon,
			.Size = sizeof(e_SW_Icon)
		},
		.DisableToolbar = true,
		.Fullscreen = true,
    };

    return new EngineEditor(spec);
}
