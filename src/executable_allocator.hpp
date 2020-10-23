#pragma once

#include <windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>

static constexpr size_t win_page_size = 4096;

template <size_t BufferSize>
class ExecutableAllocator
{
public:
    ExecutableAllocator();
    ~ExecutableAllocator();

    uint8_t* alloc(size_t size);
    void commit(void* buffer, size_t size);

    [[nodiscard]] size_t get_used() const noexcept;
    [[nodiscard]] size_t get_free() const noexcept;
    [[nodiscard]] size_t get_total() const noexcept;
    
private:
    __declspec(align(win_page_size))
    uint8_t _buffer[BufferSize] = {};
    size_t _page_size;
    size_t _consumed;
    size_t _dead_space;

    [[nodiscard]] size_t page_aligned(size_t size) const noexcept;
};

template <size_t BufferSize>
ExecutableAllocator<BufferSize>::ExecutableAllocator()
{
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    _page_size = system_info.dwPageSize;

    MEMORY_BASIC_INFORMATION page_info;
    if (!VirtualQuery(_buffer, &page_info, sizeof(page_info)))
    {
        const auto error = GetLastError();
        throw std::runtime_error("VirtualQuery failed with " + std::to_string(error));
    }

    auto page_start = reinterpret_cast<uint8_t*>(page_info.BaseAddress) + _page_size;
    _dead_space = (page_start - _buffer) % _page_size;
    _consumed = 0;
}

template <size_t BufferSize>
ExecutableAllocator<BufferSize>::~ExecutableAllocator()
{
    DWORD dummy;
    VirtualProtect(_buffer, BufferSize, PAGE_READWRITE, &dummy);
}

template <size_t BufferSize>
uint8_t* ExecutableAllocator<BufferSize>::alloc(const size_t size)
{
    auto const buffer = _buffer + get_used();
    
    _consumed += size;
    if (get_used() > BufferSize)
    {
        throw std::runtime_error("Buffer full");
    }

    DWORD dummy;
    VirtualProtect(buffer, size, PAGE_READWRITE, &dummy);

    return buffer;
}

template <size_t BufferSize>
void ExecutableAllocator<BufferSize>::commit(void* buffer, const size_t size)
{
    DWORD dummy;
    VirtualProtect(buffer, size, PAGE_EXECUTE_READ, &dummy);
}

template <size_t BufferSize>
size_t ExecutableAllocator<BufferSize>::get_used() const noexcept
{
    return _consumed + _dead_space;
}

template <size_t BufferSize>
size_t ExecutableAllocator<BufferSize>::get_free() const noexcept
{
    return get_total() - get_used();
}

template <size_t BufferSize>
size_t ExecutableAllocator<BufferSize>::get_total() const noexcept
{
    return BufferSize;
}

template <size_t BufferSize>
size_t ExecutableAllocator<BufferSize>::page_aligned(const size_t size) const noexcept
{
    return (size / _page_size + 1) * _page_size;
}

