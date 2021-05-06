#include "memory_map.hpp"

#include <utils/utils.hpp>

namespace mips
{
    uint32_t MemoryMap::read_word(const uint32_t addr)
    {
        return _map[addr];
    }

    void MemoryMap::write_word(const uint32_t addr, const uint32_t word)
    {
        _map[addr] = word;
    }

    uint32_t MemoryMap::load_word_left(const uint32_t addr, const uint32_t in)
    {
        const uint8_t offset = addr % sizeof(uint32_t);
        const uint32_t word_addr = addr - offset;
        const uint32_t raw_word = read_word(word_addr);

        const uint8_t new_bytes = sizeof(uint32_t) - offset;
        const uint32_t new_mask = utils::create_high_bitmask<uint32_t>(new_bytes * 8);

        const uint32_t new_word = new_mask & (raw_word << offset * 8);
        const uint32_t old_word = ~new_mask & in;

        return new_word | old_word;
    }

    uint32_t MemoryMap::load_word_right(const uint32_t addr, const uint32_t in)
    {
        const uint8_t offset = addr % sizeof(uint32_t);
        const uint32_t word_addr = addr - offset;
        const uint32_t raw_word = read_word(word_addr);

        const uint8_t new_bytes = 1 + offset;
        const uint32_t new_mask = utils::create_low_bitmask<uint32_t>(new_bytes * 8);

        const uint32_t new_word = new_mask & (raw_word >> (sizeof(uint32_t) - new_bytes) * 8);
        const uint32_t old_word = ~new_mask & in;

        return new_word | old_word;
    }

    template<> uint32_t MemoryMap::read<uint32_t>(uint32_t addr) { return read_word(addr); }
    template<> uint32_t MemoryMap::read<int32_t>(uint32_t addr) { return read_word(addr); }

    template<> void MemoryMap::write<uint32_t>(uint32_t addr, uint32_t value) { write_word(addr, value); }
    template<> void MemoryMap::write<int32_t>(uint32_t addr, uint32_t value) { write_word(addr, value); }
}