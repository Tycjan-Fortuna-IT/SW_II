#include <Engine.hpp>
#include <Entrypoint.hpp>

class Testbed final : public SW::Application
{
    explicit Testbed(const SW::ApplicationSpecification& specification)
        : Application(specification) {}

    friend SW::Application* CreateApplication();
};

SW::Application* CreateApplication() {
    SW::ApplicationSpecification spec = {
        .Title = "Testbed",
        .Width = 1280,
        .Height = 720,
        .VSync = false
    };

    return new Testbed(spec);
}
