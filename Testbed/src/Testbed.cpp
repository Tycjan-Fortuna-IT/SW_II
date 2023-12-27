#include <Engine.hpp>
#include <Entrypoint.hpp>
#include <iostream>

#include <glad/glad.h>

#include "Core/Debug/Logger.hpp"
#include "Core/Math/Math.hpp"
#include "Core/OpenGL/Shader.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Core/Math/Matrix4.hpp"
#include "Core/Math/Vector3.hpp"
#include "Core/Utils/Random.hpp"

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

        const SW::Vector3<f32> vec = { 1.2f, 1.4f, 1.3f };
        const SW::Matrix4<f32> mat = { 2.4f };

        APP_TRACE("SW::Vector3<f32>, %s", vec.ToString().c_str());
        APP_TRACE("SW::Vector3<f32>, %s", vec.ToCString());

        APP_TRACE("SW::Matrix4<f32>, %s", mat.ToString().c_str());
        APP_TRACE("SW::Matrix4<f32>, %s", mat.ToCString());

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
        shader->UploadUniformInt("u_Texture1", 0);
        shader->UploadUniformInt("u_Texture2", 1);

        SW::Matrix4<f32> testMatrix = { 1.0f };

        // make it smaller
        testMatrix.data[0] = 0.85f;
        testMatrix.data[5] = 1.f;

        // move it to the right
        testMatrix.data[12] = 0.5f;

        //testMatrix.RotateX(SW::Math::ToRadians(45.f));
        //testMatrix.RotateY(SW::Math::ToRadians(-85.f));
        //testMatrix.RotateZ(SW::Math::ToRadians(90.f));

        shader->UploadUniformMat4("u_Transform", testMatrix);
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
