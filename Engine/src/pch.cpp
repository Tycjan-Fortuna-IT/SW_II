#include "pch.hpp"

size_t g_AllocationSizeSum = 0;

namespace SW
{
	size_t GetAllocationSizeSum()
	{
		return g_AllocationSizeSum;
	}
} // namespace SW

void* operator new(size_t size)
{
	g_AllocationSizeSum += size;

	return malloc(size);
}

void* operator new[](size_t size)
{
	g_AllocationSizeSum += size;

	return malloc(size);
}

void operator delete(void* ptr, size_t size) noexcept
{
	g_AllocationSizeSum -= size;

	free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept
{
	g_AllocationSizeSum -= size;

	free(ptr);
}
