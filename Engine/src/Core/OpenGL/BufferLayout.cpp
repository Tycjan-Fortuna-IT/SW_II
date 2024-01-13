#include "BufferLayout.hpp"

namespace SW {

	void BufferLayout::CalculateOffsetsAndStride()
	{
		u32 offset = 0;

		for (BufferElement& element : m_Elements) {
			element.Offset = offset;
			offset += element.Size;
		}

		m_Stride = offset;
	}

}
