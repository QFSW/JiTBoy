#include "register_file.hpp"

#include <sstream>
#include <utils/strtools.hpp>

namespace mips
{
    std::string RegisterFile::generate_dump(const bool omit_zeroes) const
    {
        std::stringstream ss;

        ss << "Register file";
        if (omit_zeroes) ss << " (zeroed registers omitted)";
        ss << "\n";

        for (size_t i = 0; i < size(); i++)
        {
            const auto reg = _regs[i];
            if (reg > 0 || !omit_zeroes) ss << strtools::catf("$%d: %d\n", i, reg);
        }

        return ss.str();
    }

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