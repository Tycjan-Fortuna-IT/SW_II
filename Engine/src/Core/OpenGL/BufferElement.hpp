/**
 * @file BufferElement.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.2
 * @date 2024-01-11
 * 
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <glad/glad.h>

namespace SW {

	/**
	 * @brief Enumeration defining different shader data types
	 * 		  supported by the engine.
	 */
	enum class ShaderDataType : u8
	{
		None = 0, /**< Shader data type for undefined data. */
		Float,    /**< Shader data type for single-precision floating point number. */
		Float2,	  /**< Shader data type for 2D vector of single-precision floating point numbers. */
		Float3,   /**< Shader data type for 3D vector of single-precision floating point numbers. */
		Float4,   /**< Shader data type for 4D vector of single-precision floating point numbers. */
		Mat3,     /**< Shader data type for 3x3 matrix of single-precision floating point numbers. */
		Mat4,	  /**< Shader data type for 4x4 matrix of single-precision floating point numbers. */
		Int,      /**< Shader data type for signed integer. */
		Int2,     /**< Shader data type for 2D vector of signed integers. */
		Int3,     /**< Shader data type for 3D vector of signed integers. */
		Int4,     /**< Shader data type for 4D vector of signed integers. */
		Bool      /**< Shader data type for boolean. */
	};

	/**
	 * @brief Get the specific OpenGL macro for specified shader data type.
	 * @param type Internal engine's type notation enum
	 * @return OpenGL macro representing given type
	 */
	inline GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:  
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
				return GL_FLOAT;
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3: 
			case ShaderDataType::Int4:
				return GL_INT;
			case ShaderDataType::Bool:     
				return GL_BOOL;
			default:
				ASSERT(false, "Unknown ShaderDataType!");
		}

		return 0;
	}

	/**
	 * @brief Get the size of specified shader data type.
	 * @param type Internal engine's type notation enum
	 * @return Size of the type
	 */
	inline u32 GetShaderDataTypeSize(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float:  return 4;
			case ShaderDataType::Float2: return 4 * 2;
			case ShaderDataType::Float3: return 4 * 3;
			case ShaderDataType::Float4: return 4 * 4;
			case ShaderDataType::Mat3:   return 4 * 3 * 3;
			case ShaderDataType::Mat4:   return 4 * 4 * 4;
			case ShaderDataType::Int:    return 4;
			case ShaderDataType::Int2:   return 4 * 2;
			case ShaderDataType::Int3:   return 4 * 3;
			case ShaderDataType::Int4:   return 4 * 4;
			case ShaderDataType::Bool:   return 1;
			default:
				ASSERT(false, "Unknown ShaderDataType!");
		}

		return 0;
	}

	/**
	 * @brief Represents a single element in a buffer layout.
	 * @warning Remember to update the offset when adding new buffer elements to the layout!
	 */
	struct BufferElement
	{
		std::string Name;		/**< The name of the element. */
		ShaderDataType Type;	/**< The data type of the element. */
		u32 Size;				/**< The size of the element. */
		u32 Offset;				/**< The offset of the element. */

		/**
		 * @brief Constructs a BufferElement object.
		 * @param type The data type of the element.
		 * @param name The name of the element.
		 * @param offset The offset of the element (default is 0).
		 */
		BufferElement(ShaderDataType type, const std::string& name, u32 offset = 0)
			: Name(name), Type(type), Size(GetShaderDataTypeSize(type)), Offset(offset) {}

		/**
		 * @brief Gets the number of components in the element.
		 * @return The number of components.
		 */
		u32 GetComponentCount() const
		{
			switch (Type) {
				case ShaderDataType::Float:  return 1;
				case ShaderDataType::Float2: return 2;
				case ShaderDataType::Float3: return 3;
				case ShaderDataType::Float4: return 4;
				case ShaderDataType::Mat3:   return 3 * 3;
				case ShaderDataType::Mat4:   return 4 * 4;
				case ShaderDataType::Int:    return 1;
				case ShaderDataType::Int2:   return 2;
				case ShaderDataType::Int3:   return 3;
				case ShaderDataType::Int4:   return 4;
				case ShaderDataType::Bool:   return 1;
				default:
					ASSERT(false, "Unknown ShaderDataType!");
			}

			return 0;
		}
	};

}
