#pragma once

#include <common/common.hpp>

namespace mips
{
    class MemoryMap
    {
    public:
        [[nodiscard]] uint32_t read_word(uint32_t addr);
        void write_word(uint32_t addr, uint32_t word);

        [[nodiscard]] uint32_t load_word_left(uint32_t addr, uint32_t in);
        [[nodiscard]] uint32_t load_word_right(uint32_t addr, uint32_t in);

        template <typename T>
        [[nodiscard]] uint32_t read(uint32_t addr);

        template <typename T>
        void write(uint32_t addr, uint32_t value);

    private:
        common::unordered_map<uint32_t, uint32_t> _map;
    };

    template<> uint32_t MemoryMap::read<uint32_t>(uint32_t addr);
    template<> uint32_t MemoryMap::read<int32_t>(uint32_t addr);

    template <typename T>
    uint32_t MemoryMap::read(const uint32_t addr)
    {
        const uint32_t offset = addr % sizeof(uint32_t);
        const uint32_t word = _map[addr - offset];
        const int shamt = 8 * (sizeof(uint32_t) - offset - sizeof(T));

        T data = static_cast<T>(word >> shamt);
        return static_cast<uint32_t>(data);
    }

    template<> void MemoryMap::write<uint32_t>(uint32_t addr, uint32_t value);
    template<> void MemoryMap::write<int32_t>(uint32_t addr, uint32_t value);

    template <typename T>
    void MemoryMap::write(const uint32_t addr, const uint32_t value)
    {
        const uint32_t offset = addr % sizeof(uint32_t);
        uint32_t& word = _map[addr - offset];

        const int shamt = 8 * (sizeof(uint32_t) - offset - sizeof(T));
        const uint32_t mask = ~0 ^ (static_cast<uint32_t>(T(~0)) << shamt);

        word &= mask;
        word |= value << shamt;
    }
}