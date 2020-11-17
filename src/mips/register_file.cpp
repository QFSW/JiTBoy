#include "register_file.hpp"

namespace mips
{
    uint32_t& RegisterFile::operator[](const size_t index)
    {
        return _regs[index];
    }

    uint32_t& RegisterFile::operator[](Register reg)
    {
        return _regs[static_cast<size_t>(reg)];
    }

    const uint32_t& RegisterFile::operator[](Register reg) const
    {
        return _regs[static_cast<size_t>(reg)];
    }

    const uint32_t& RegisterFile::operator[](const size_t index) const
    {
        return _regs[index];
    }
}