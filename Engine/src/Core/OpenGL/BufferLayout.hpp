/**
 * @file BufferLayout.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.2
 * @date 2024-01-11
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "BufferElement.hpp"

namespace SW
{

	/**
	 * @brief Class representing layout of the buffer.
	 *		  It is used to define how the data is stored in the buffer.
	 * 		  Allows us to define what we want to pass to the shader.
	 */
	class BufferLayout final
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		BufferLayout() = default;

		/**
		 * @brief Constructor.
		 * @param elements List of elements that we want to store in the buffer.
		 */
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		/**
		 * @brief Get the list of elements.
		 * @return List of elements.
		 */
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		/**
		 * @brief Get the stride of the buffer.
		 * @return Stride of the buffer.
		 */
		u32 GetStride() const { return m_Stride; }

		/**
		 * @brief Get the begin iterator.
		 * @return Begin iterator.
		 */
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }

		/**
		 * @brief Get the end iterator.
		 * @return End iterator.
		 */
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		/**
		 * @brief Get the constant begin iterator.
		 * @return Constant egin iterator.
		 */
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }

		/**
		 * @brief Get the constant end iterator.
		 * @return Constant end iterator.
		 */
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		std::vector<BufferElement> m_Elements; /** @brief List of elements. */

		u32 m_Stride = 0; /** @brief Stride of the buffer. */

		/**
		 * @brief Calculates the offsets and stride of the buffer.
		 * @warning This function should be called only once and after the elements are set.
		 */
		void CalculateOffsetsAndStride();
	};

} // namespace SW
