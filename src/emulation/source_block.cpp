#include "source_block.hpp"

#include <stdexcept>
#include <utils/strtools.hpp>

namespace emulation
{
    SourceBlock::SourceBlock(const mips::Program& program, uint32_t start_addr, uint32_t end_addr)
        : _program(program)
        , _start_addr(start_addr)
        , _end_addr(end_addr)
    { }

    uint32_t SourceBlock::start_addr() const noexcept { return _start_addr; }
    uint32_t SourceBlock::end_addr() const noexcept { return _end_addr; }

    uint32_t SourceBlock::size() const noexcept
    {
        return (_end_addr - _start_addr) / 4;
    }

    bool SourceBlock::valid_addr(uint32_t addr) const noexcept
    {
        return addr >= _start_addr && addr < _end_addr;
    }

    const mips::Instruction& SourceBlock::at(uint32_t addr) const
    {
        if (!valid_addr(addr))
            throw std::logic_error(strtools::catf("Address 0x%x is out of bounds", addr));

        return _program.at(addr);
    }

    std::ostream& operator<<(std::ostream& os, const SourceBlock& b)
    {
        for (uint32_t addr = b._start_addr; addr < b._end_addr; addr += 4)
        {
            os << strtools::catf("%04x: ", addr) << b._program.at(addr) << "\n";
        }

        return os;
    }
}
