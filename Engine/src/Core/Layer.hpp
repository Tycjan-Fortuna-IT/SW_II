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

        API virtual void OnAttach() {}
        API virtual void OnDetach() {}

        API virtual void OnUpdate(float dt) {}

        API virtual void OnRender() {}

        const std::string& GetName() const { return m_Name; }

    private:
        std::string m_Name;
    };

}
