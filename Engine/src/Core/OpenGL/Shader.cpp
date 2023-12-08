#include "pch.hpp"
#include "Shader.hpp"

#include <glad/glad.h>

#include "Core/Utils/Utils.hpp"

namespace SW {
    Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath) {
        ShaderData data = this->ProcessShaderFiles(vertexFilePath, fragmentFilePath);

        this->CompileShaders(data);

        TRACE("Vertex shader `%s` compiled successfully!", vertexFilePath.c_str());
        TRACE("Fragment shader `%s` compiled successfully!", fragmentFilePath.c_str());
    }

    Shader::~Shader() {
        glDeleteProgram(m_ShaderHandle);
    }

    ShaderData Shader::ProcessShaderFiles(const std::string& vertexFilePath, const std::string& fragmentFilePath) const {
        return {
            .vertexSource = Utils::ReadFile(vertexFilePath),
            .fragmentSource = Utils::ReadFile(fragmentFilePath)
        };
    }

    void Shader::CompileShaders(const ShaderData& data) {
        const u32 program = glCreateProgram();

        const u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* vertexSource = data.vertexSource.c_str();
        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);

        i32 isVertexCompiled;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isVertexCompiled);

        if (!isVertexCompiled) {
            GLint maxLength = 0;
            std::vector<GLchar> infoLog(maxLength);

            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());
            glDeleteShader(vertexShader);

            ERROR("%s", infoLog.data());
            ERROR("Vertex shader compilation failure!");

            return;
        }

        const u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        const GLchar* fragmentSource = data.vertexSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(fragmentShader);

        i32 isFragmentCompiled;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isFragmentCompiled);

        if (!isFragmentCompiled) {
            GLint maxLength = 0;
            std::vector<GLchar> infoLog(maxLength);

            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            ERROR("%s", infoLog.data());
            ERROR("Fragment shader compilation failure!");

            return;
        }

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

        if (!isLinked) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            ERROR("%s", infoLog.data());
            ERROR("Shader link failure!");

            return;
        }

        //glDetachShader(program, vertexShader);
        //glDetachShader(program, fragmentShader);

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
}
