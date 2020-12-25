#pragma once

#include <unordered_map>
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
        std::unordered_map<uint32_t, uint32_t> _map;
    };

    template <typename T>
    uint32_t MemoryMap::read(const uint32_t addr)
    {
        if constexpr (std::is_same_v<T, uint32_t>) return read_word(addr);
        if constexpr (std::is_same_v<T, int32_t>)  return read_word(addr);

        const uint32_t offset = addr % sizeof(uint32_t);
        const uint32_t word = _map[addr - offset];
        const int shamt = 8 * (sizeof(uint32_t) - offset - sizeof(T));

        T data = static_cast<T>(word >> shamt);
        return static_cast<uint32_t>(data);
    }

    template <typename T>
    void MemoryMap::write(const uint32_t addr, const uint32_t value)
    {
        if constexpr (std::is_same_v<T, uint32_t>)     write_word(addr, value);
        else if constexpr (std::is_same_v<T, int32_t>) write_word(addr, value);
        else
        {
            const uint32_t offset = addr % sizeof(uint32_t);
            uint32_t& word = _map[addr - offset];

            const int shamt = 8 * (sizeof(uint32_t) - offset - sizeof(T));
            const uint32_t mask = ~0 ^ (static_cast<uint32_t>(T(~0)) << shamt);

            word &= mask;
            word |= value << shamt;
        }
    }
}