#pragma once

namespace SW {

    struct Event;

    class Layer
    {
    public:
        API Layer(const std::string& name = "DefaultLayer");
        virtual ~Layer() = default;

        //Layer(const Layer& application) = delete;
        //Layer(Layer&& application) = delete;
        //Layer& operator=(const Layer& application) = delete;
        //Layer& operator=(Layer&& application) = delete;

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        virtual void OnUpdate(float dt) {}
        virtual void OnEvent(Event& event) {}

        virtual void OnImGuiRender() {}

        const std::string& GetName() const { return m_Name; }

    private:
        std::string m_Name;
    };

}
