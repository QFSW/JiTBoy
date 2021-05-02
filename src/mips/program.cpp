#include "program.hpp"

namespace mips
{
    bool Program::valid_addr(const uint32_t addr) const noexcept
    {
        if (addr < start_addr)
            return false;
        if (addr >= start_addr + source.size() * 4)
            return false;
        return true;
    }
}