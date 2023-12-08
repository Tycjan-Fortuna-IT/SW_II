#pragma once

namespace SW {

    struct ShaderData final
    {
        std::string vertexSource;
        std::string fragmentSource;
    };

    class Shader final
    {
    public:
        API Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
        API ~Shader();

        void Bind() const;
        void Unbind() const;

    private:
        u32 m_ShaderHandle;

        ShaderData ProcessShaderFiles(const std::string& vertexFilePath, const std::string& fragmentFilePath) const;
        void CompileShaders(const ShaderData& data);
    };

}
