#include "VectorBuffer.h"

void VectorBuffer::reset()
{
	_buffer.clear();
}

void VectorBuffer::copy(void* ptr) const
{
	memcpy(ptr, _buffer.data(), _buffer.size());
}

size_t VectorBuffer::size() const noexcept
{
	return _buffer.size();
}
