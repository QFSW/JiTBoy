#pragma once

#include <map>

namespace mips
{
    class MemoryMap
    {
    public:
        [[nodiscard]] uint32_t read_word(uint32_t addr);
        void write_word(uint32_t addr, uint32_t word);

    private:
        std::map<uint32_t, uint32_t> _map;
    };
}