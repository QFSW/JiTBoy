#pragma once

#include <map>
#include <stdexcept>
#include <type_traits>

namespace mips
{
    class MemoryMap
    {
    public:
        [[nodiscard]] uint32_t read_word(uint32_t addr);
        void write_word(uint32_t addr, uint32_t word);

        template <typename T>
        [[nodiscard]] uint32_t read(uint32_t addr);

        template <typename T>
        void write(uint32_t addr, uint32_t value);

    private:
        std::map<uint32_t, uint32_t> _map;
    };

    template <typename T>
    uint32_t MemoryMap::read(const uint32_t addr)
    {
        if constexpr (std::is_same_v<T, uint32_t>) return read_word(addr);
        if constexpr (std::is_same_v<T, int32_t>)  return read_word(addr);

        throw std::logic_error("MemoryMap::read<T> is not implemented");
    }

    template <typename T>
    void MemoryMap::write(const uint32_t addr, const uint32_t value)
    {
        if constexpr (std::is_same_v<T, uint32_t>)     write_word(addr, value);
        else if constexpr (std::is_same_v<T, int32_t>) write_word(addr, value);
        else
        {
            throw std::logic_error("MemoryMap::write<T> is not implemented");
        }
    }
}