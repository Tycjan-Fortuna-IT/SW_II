/**
 * @file BufferElement.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-03
 * 
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

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

	class BufferElement final
	{
	public:

	private:

	};

}
