#include "pch.hpp"
#include "Shader.hpp"

#include <glad/glad.h>

#include "Core/Utils/Utils.hpp"

namespace SW {
    Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath) {
        ShaderData data = this->ProcessShaderFiles(vertexFilePath, fragmentFilePath);

        this->CompileShaders(data);
    }

    Shader::~Shader() {
        glDeleteProgram(m_ShaderHandle);
    }

    ShaderData Shader::ProcessShaderFiles(const std::string& vertexFilePath, const std::string& fragmentFilePath) const {
        return {
            .vertexSourcePath = vertexFilePath,
            .vertexSource = Utils::ReadFile(vertexFilePath),
            .fragmentSourcePath = fragmentFilePath,
            .fragmentSource = Utils::ReadFile(fragmentFilePath)
        };
    }

    void Shader::CompileShaders(const ShaderData& data) {
        const u32 program = glCreateProgram();

        u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* vertexSource = data.vertexSource.c_str();

        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);

        INFO("Compiling - Vertex shader: \n%s", data.vertexSource.c_str());

        i32 isVertexCompiled;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isVertexCompiled);

        if (!isVertexCompiled)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(vertexShader);

            ERROR("%s", infoLog.data());
            ERROR("Vertex shader compilation failure!");

            return;
        }

        TRACE("Vertex shader `%s` compiled successfully!", data.vertexSourcePath.c_str());

        u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        const GLchar* fragmentSource = data.fragmentSource.c_str();

        glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(fragmentShader);

        INFO("Compiling - Fragment shader: \n%s", data.fragmentSource.c_str());

        i32 isFragmentCompiled;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isFragmentCompiled);

        if (!isFragmentCompiled)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            ERROR("%s", infoLog.data());
            ERROR("Fragment shader compilation failure!");

            return;
        }

        TRACE("Fragment shader `%s` compiled successfully!", data.fragmentSourcePath.c_str());

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            ERROR("Shader link failure!");

            return;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        m_ShaderHandle = program;
    }

    void Shader::Bind() const {
        glUseProgram(m_ShaderHandle);
    }

    void Shader::Unbind() const {
        glUseProgram(0);
    }

    void Shader::UploadUniformInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(m_ShaderHandle, name.c_str()), value);
    }
}
