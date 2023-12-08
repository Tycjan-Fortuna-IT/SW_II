#pragma once

#include <vector>

#include "Defines.hpp"
#include "Core/OpenGL/Window.hpp"
#include "Core/Layer.hpp"

int main();

namespace SW {

    struct Event;

    struct ApplicationSpecification final
    {
        std::string Title = "No title";
        u16 Width = 1280;
        u16 Height = 720;
        bool VSync = false;
    };

    class Application
    {
    public:
        API static Application* Get() { return s_Instance; }

        API const Window* GetWindow() const { return m_Window;  }

        bool OnInit();
        bool OnShutdown();

        void OnEvent();

        API void Run();
        API void Close();

        API void PushLayer(Layer* layer);
        API void PopLayer();

    protected:
        API Application(const ApplicationSpecification& specification);
        API virtual ~Application() = default;

    private:
        Application(const Application& application) = delete;
        Application(Application&& application) = delete;
        Application& operator=(const Application& application) = delete;
        Application& operator=(Application&& application) = delete;

        static Application* s_Instance;

        ApplicationSpecification m_Specification;
        Window* m_Window = nullptr;

        bool m_IsRunning = false;
        float m_LastFrameTime = 0.f;

        std::vector<Layer*> m_Layers;

        friend int ::main();
    };

}
