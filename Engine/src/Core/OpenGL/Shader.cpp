#include "pch.hpp"
#include "Shader.hpp"

#include "glad/glad.h"

namespace SW {
    Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath) {

    }

    Shader::~Shader() {
        glDeleteProgram(m_Handle);
    }

    void Shader::Bind() const {
        glUseProgram(m_Handle);
    }

    void Shader::Unbind() const {
        glUseProgram(0);
    }
}
