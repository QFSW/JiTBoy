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

        if (!omit_zeroes || hi() != 0) ss << strtools::catf("$hi: %d\n", hi());
        if (!omit_zeroes || lo() != 0) ss << strtools::catf("$lo: %d\n", lo());

        return ss.str();
    }

    void RegisterFile::write(const Register reg, const uint32_t value)
    {
        if (reg != Register::$zero)
            _regs[static_cast<size_t>(reg)] = value;
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