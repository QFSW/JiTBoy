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

    template<> uint32_t MemoryMap::read<uint32_t>(uint32_t addr) { return read_word(addr); }
    template<> uint32_t MemoryMap::read<int32_t>(uint32_t addr) { return read_word(addr); }

    template<> void MemoryMap::write<uint32_t>(uint32_t addr, uint32_t value) { write_word(addr, value); }
    template<> void MemoryMap::write<int32_t>(uint32_t addr, uint32_t value) { write_word(addr, value); }
}