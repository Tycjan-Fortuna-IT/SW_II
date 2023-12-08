#pragma once

namespace SW {

    class Shader final
    {
    public:
        Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

    private:
        u32 m_Handle;
    };

}
