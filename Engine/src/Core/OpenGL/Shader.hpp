#pragma once

#include "Core/Math/Matrix4.hpp"

namespace SW {

	/**
	 * @brief Struct representing the data required for a shader.
	 */
	struct ShaderData final
	{
		std::string vertexSourcePath;    ///< The file path of the vertex shader source code.
		std::string vertexSource;        ///< The vertex shader source code.
		std::string fragmentSourcePath;  ///< The file path of the fragment shader source code.
		std::string fragmentSource;      ///< The fragment shader source code.
	};

	/**
	 * @brief Class representing a shader.
	 */
	class Shader final
	{
	public:
		/**
		 * @brief Constructs a Shader object.
		 * @param vertexFilePath The file path of the vertex shader source code.
		 * @param fragmentFilePath The file path of the fragment shader source code.
		 */
		Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);

		/**
		 * @brief Destroys the Shader object.
		 */
		~Shader();

		/**
		 * @brief Binds the shader for rendering.
		 */
		void Bind() const;

		/**
		 * @brief Unbinds the shader.
		 */
		void Unbind() const;

		/**
		 * @brief Gets the handle of the shader.
		 * @return The handle of the shader.
		 */
		const u32 GetHandle() const;

		/**
		 * @brief Uploads an integer uniform to the shader.
		 * @param name The name of the uniform.
		 * @param value The value of the uniform.
		 */
		void UploadUniformInt(const std::string& name, int value) const;

		/**
		 * @brief Uploads an integer array uniform to the shader.
		 * @param name The name of the uniform.
		 * @param values The value of the uniform.
		 * @param count Count of the elements in the passed value.
		 */
		void UploadUniformIntArray(const std::string& name, int* values, u32 count) const;

		/**
		 * @brief Uploads a 4x4 matrix uniform to the shader.
		 * @param name The name of the uniform.
		 * @param value The value of the uniform.
		 */
		void UploadUniformMat4(const std::string& name, const Matrix4<f32>& value) const;

	private:
		u32 m_Handle; ///< The handle of the shader.

		mutable std::unordered_map<std::string, i32> m_UniformLocations; ///< Cache of uniform locations.

		/**
		 * @brief Gets the location of a uniform in the shader. (cached)
		 * @param name The name of the uniform.
		 * @return The location of the uniform.
		 */
		i32 GetUniformLocation(const std::string name) const;

		/**
		 * @brief Processes the shader files and returns the shader data.
		 * @param vertexFilePath The file path of the vertex shader source code.
		 * @param fragmentFilePath The file path of the fragment shader source code.
		 * @return The shader data.
		 */
		ShaderData ProcessShaderFiles(const std::string& vertexFilePath, const std::string& fragmentFilePath) const;

		/**
		 * @brief Compiles the shaders using the provided shader data.
		 * @param data The shader data.
		 */
		void CompileShaders(const ShaderData& data);
	};

}
