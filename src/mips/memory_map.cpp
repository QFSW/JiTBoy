#include "memory_map.hpp"

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
}