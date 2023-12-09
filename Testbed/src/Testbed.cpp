#include <Engine.hpp>
#include <Entrypoint.hpp>
#include <iostream>

#include "Core/Debug/Logger.hpp"
#include "Core/OpenGL/Shader.hpp"
#include "glad/glad.h"

class TestLayer final : public SW::Layer
{
public:
    explicit TestLayer(const std::string& name)
        : SW::Layer(name) {}

    u32 VBO, VAO;
    SW::Shader* shader;

    void OnAttach() override {
        APP_TRACE("ON attach!");

        shader = new SW::Shader("assets/shaders/Initial.vert.glsl", "assets/shaders/Initial.frag.glsl");

        const float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        glGenBuffers(1, &VBO);

        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void OnDetach() override {
        APP_TRACE("ON detach!");

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void OnUpdate(float dt) override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->Bind();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
