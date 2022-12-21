#include "CustomPxAllocatorCallback.h"

void* CCustomPxAllocatorCallback::allocate(size_t size, const char* typeName, const char* filename, int line)
{
	void* ptr = platformAlignedAlloc(size);
	PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0);
	return ptr;
}

void CCustomPxAllocatorCallback::deallocate(void* ptr)
{
	platformAlignedFree(ptr);
}


