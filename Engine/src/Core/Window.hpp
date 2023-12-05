#pragma once

#include <string>

#include "Defines.hpp"

struct GLFWwindow;

namespace SW {

    struct WindowSpecification final
    {
        std::string Title = "No title";
        u16 Width = 1280;
        u16 Height = 720;
        bool VSync = false;
    };

    class Window final
    {
    public:
        Window(const WindowSpecification& specification);
        ~Window();

        void OnUpdate() const;

        u16 GetWidth() const { return m_Specification.Width; }
        u16 GetHeight() const { return m_Specification.Height; }

        const GLFWwindow* GetHandle() const { return m_Handle; }

        void SetVSync(bool enabled);

    private:
        WindowSpecification m_Specification;
        GLFWwindow* m_Handle = nullptr;
    };

}
