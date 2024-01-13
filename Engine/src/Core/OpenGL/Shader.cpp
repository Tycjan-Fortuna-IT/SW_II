#include "pch.hpp"
#include "Shader.hpp"

#include <glad/glad.h>

#include "Core/Utils/Utils.hpp"

namespace SW {
    Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
	{
        ShaderData data = this->ProcessShaderFiles(vertexFilePath, fragmentFilePath);

        this->CompileShaders(data);
    }

    Shader::~Shader()
	{
        glDeleteProgram(m_Handle);
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

		SW_DEBUG("Compiling - Vertex shader: \n{}", data.vertexSource);

		i32 isVertexCompiled;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isVertexCompiled);

		if (!isVertexCompiled) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertexShader);

			SW_ERROR("{}", infoLog.data());
			SW_ERROR("Vertex shader compilation failure!");

			return;
		}

		SW_INFO("Vertex shader `{}` compiled successfully!", data.vertexSourcePath);

		u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar* fragmentSource = data.fragmentSource.c_str();

		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);

		SW_DEBUG("Compiling - Fragment shader: \n{}", data.fragmentSource);

		i32 isFragmentCompiled;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isFragmentCompiled);

		if (!isFragmentCompiled) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			SW_ERROR("{}", infoLog.data());
			SW_ERROR("Fragment shader compilation failure!");

			return;
		}

		SW_INFO("Fragment shader `{}` compiled successfully!", data.fragmentSourcePath);

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

			SW_ERROR("Shader link failure!");

			return;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		m_Handle = program;
	}

	void Shader::Bind() const {
		glUseProgram(m_Handle);
	}

	void Shader::Unbind() const {
		glUseProgram(0);
	}

	i32 Shader::GetUniformLocation(const std::string name) const {
		auto location = m_UniformLocations.find(name);

		if (location == m_UniformLocations.end()) {
			i32 newLocation = glGetUniformLocation(m_Handle, name.c_str());

			m_UniformLocations[name] = newLocation;

			return newLocation;
		}

		return location->second;
	}

    void Shader::UploadUniformInt(const std::string& name, int value) const
	{
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::UploadUniformIntArray(const std::string& name, int* values, u32 count) const
	{
		glUniform1iv(GetUniformLocation(name), count, values);
    }

    void Shader::UploadUniformMat4(const std::string& name, const Matrix4<f32>& value) const
	{
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value.ValuePtr());
    }
}
