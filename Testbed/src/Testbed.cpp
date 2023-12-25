#include <Engine.hpp>
#include <Entrypoint.hpp>
#include <iostream>

#include <glad/glad.h>

#include "Core/Debug/Logger.hpp"
#include "Core/OpenGL/Shader.hpp"
#include "Core/OpenGL/Texture2D.hpp"

class TestLayer final : public SW::Layer
{
public:
    explicit TestLayer(const std::string& name)
        : SW::Layer(name) {}

    u32 VBO, VAO, EBO = 0;

    SW::Shader* shader = nullptr;
    SW::Texture2D* boxTexture = nullptr;
    SW::Texture2D* faceTexture = nullptr;

    void OnAttach() override {
        APP_TRACE("ON attach!");

        shader = new SW::Shader("assets/shaders/Initial.vert.glsl", "assets/shaders/Initial.frag.glsl");
        boxTexture = new SW::Texture2D("assets/textures/container_512x512.jpg");
        faceTexture = new SW::Texture2D("assets/textures/awesomeface_512x512.png");

        float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader->Bind();
        glUniform1i(glGetUniformLocation(shader->GetHandle(), "u_Texture1"), 0);
        glUniform1i(glGetUniformLocation(shader->GetHandle(), "u_Texture2"), 1);

    }

    void OnDetach() override {
        APP_TRACE("ON detach!");

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        delete shader;
        delete boxTexture;
        delete faceTexture;
    }

    void OnUpdate(float dt) override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->Bind();
        boxTexture->Bind(0);
        faceTexture->Bind(1);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
