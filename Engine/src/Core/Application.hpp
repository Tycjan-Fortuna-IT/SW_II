#pragma once

#include <string>

#include "Defines.hpp"

namespace SW {

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
        API static const Application* Get() { return s_Instance; }

        bool OnInit();
        bool OnShutdown();

        void OnEvent();
        void OnUpdate();

        API void Run();
        API void Close();

    protected:
        API Application(const ApplicationSpecification& specification);
        API virtual ~Application() = default;

    private:
        Application(const Application& application) = delete;
        Application(Application&& application) = delete;
        Application& operator=(const Application& application) = delete;
        Application& operator=(Application&& application) = delete;

        static Application* s_Instance;

        ApplicationSpecification specification;

        bool m_IsRunning = false;
    };

}
