#pragma once

#include <windows.h>
#include <cstdint>
#include <iostream>

template <size_t BufferSize>
class ExecutableAllocator
{
public:
	ExecutableAllocator()
	{
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		_page_size = system_info.dwPageSize;

		MEMORY_BASIC_INFORMATION info;
		VirtualQuery(_buffer, &info, BufferSize);

		auto page_start = reinterpret_cast<uint8_t*>(info.BaseAddress) + _page_size;
		_consumed = page_start - _buffer;
	}

	uint8_t* alloc(size_t size);
	void commit(void* buffer, size_t size) const;
	
private:
	uint8_t _buffer[BufferSize] = {};
	size_t _page_size;
	size_t _consumed;

	[[nodiscard]] size_t page_aligned(size_t size) const;
};

template <size_t BufferSize>
uint8_t* ExecutableAllocator<BufferSize>::alloc(size_t size)
{
	auto const buffer = _buffer + _consumed;
	size = page_aligned(size);

	_consumed += size;
	if (_consumed > BufferSize)
	{
		throw "Buffer full!";
	}

	return buffer;
}

template <size_t BufferSize>
void ExecutableAllocator<BufferSize>::commit(void* buffer, const size_t size) const
{
	DWORD dummy;
	VirtualProtect(buffer, page_aligned(size), PAGE_EXECUTE_READ, &dummy);
}

template <size_t BufferSize>
size_t ExecutableAllocator<BufferSize>::page_aligned(const size_t size) const
{
	return (size / _page_size + 1) * _page_size;
}

