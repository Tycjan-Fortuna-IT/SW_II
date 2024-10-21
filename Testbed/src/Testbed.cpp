#include <Entrypoint.hpp>
#include "TestbedLayer.hpp"

class EngineTestbed final : public SW::Application
{
public:
    explicit EngineTestbed(const SW::ApplicationSpecification& specification)
        : SW::Application(specification) {

        this->PushLayer(new SW::TestbedLayer("Test"));
    }

protected:
    ~EngineTestbed() override {
        this->PopLayer();
    }
};

SW::Application* CreateApplication() {
    SW::ApplicationSpecification spec = {
        .Title = "SW Testbed",
        .Width = 1280,
        .Height = 720,
        .VSync = false,
		.DisableToolbar = false,
		.Fullscreen = true,
		.EnableCSharpSupport = false,
    };

    return new EngineTestbed(spec);
}
