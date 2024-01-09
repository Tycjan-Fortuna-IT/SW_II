#include <Engine.hpp>
#include <Entrypoint.hpp>
#include <iostream>

#include "TestLayer.hpp"

#include "icons/embed/SW_Icon.embed"

class Testbed final : public SW::Application
{
public:
    explicit Testbed(const SW::ApplicationSpecification& specification)
        : SW::Application(specification) {

        this->PushLayer(new SW::TestLayer("Test"));
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
        .VSync = false,
		.Icon = {
			.Data = e_SW_Icon,
			.Size = sizeof(e_SW_Icon)
		},
		.DisableToolbar = true,
    };

    return new Testbed(spec);
}
