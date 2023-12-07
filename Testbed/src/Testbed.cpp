#include <Engine.hpp>
#include <Entrypoint.hpp>

#include "Core/Debug/Logger.hpp"

class TestLayer final : public SW::Layer
{
public:
    explicit TestLayer(const std::string& name)
        : SW::Layer(name) {}

    void OnAttach() override {
        APP_TRACE("ON attach!");
    }

    void OnDetach() override {
        APP_TRACE("ON detach!");
    }

    void OnUpdate(float dt) override {

    }

    void OnRender() override {

    }
};

class Testbed final : public SW::Application
{
public:
    explicit Testbed(const SW::ApplicationSpecification& specification)
        : SW::Application(specification) {

        this->PushLayer(new TestLayer("Test"));
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
