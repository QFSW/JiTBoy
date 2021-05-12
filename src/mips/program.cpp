#include "program.hpp"

#include <map>
#include <utils/strtools.hpp>

namespace mips
{
    Program::Program()
        : _source()
        , _entry_point(0)
    { }

    Program::Program(common::unordered_map<uint32_t, Instruction>&& source, uint32_t entry_point)
        : _source(source)
        , _entry_point(entry_point)
    { }

    bool Program::valid_addr(const uint32_t addr) const noexcept
    {
        return _source.find(addr) != _source.end();
    }

    const Instruction& Program::at(const uint32_t addr) const
    {
        return _source.at(addr);
    }

    std::ostream& operator<<(std::ostream& os, const Program& p)
    {
        if (p._ordered_cache.size() == 0)
        {
            for (const auto& [addr, instr] : p._source)
            {
                p._ordered_cache[addr] = instr;
            }
        }

        for (const auto& [addr, instr] : p._ordered_cache)
        {
            os << strtools::catf("%04x: ", addr) << instr << "\n";
        }

        return os;
    }
}
