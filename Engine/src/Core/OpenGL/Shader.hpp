#pragma once

namespace SW {

    struct ShaderData final
    {
        std::string vertexSourcePath;
        std::string vertexSource;
        std::string fragmentSourcePath;
        std::string fragmentSource;
    };

    class Shader final
    {
    public:
        Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        const u32 GetHandle() const { return m_ShaderHandle; }

        void UploadUniformInt(const std::string& name, int value) const;

    private:
        u32 m_ShaderHandle;

        ShaderData ProcessShaderFiles(const std::string& vertexFilePath, const std::string& fragmentFilePath) const;
        void CompileShaders(const ShaderData& data);
    };

}
