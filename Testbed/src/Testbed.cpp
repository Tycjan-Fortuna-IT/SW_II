#include <Engine.hpp>
#include <Entrypoint.hpp>

class TestLayer final : public SW::Layer
{
public:
    explicit TestLayer(const std::string& name)
        : SW::Layer(name) {}
};

class Testbed final : public SW::Application
{
public:
    explicit Testbed(const SW::ApplicationSpecification& specification)
        : SW::Application(specification) {

        const TestLayer layer("Test");

        this->PushLayer(layer);
    }

protected:
    ~Testbed() override {
        this->PopLayer();
    }
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
