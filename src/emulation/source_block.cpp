#include "source_block.hpp"

namespace emulation
{
    SourceBlock::SourceBlock(const std::span<const mips::Instruction> code, const uint32_t addr)
        : code(code)
        , addr(addr)
    { }
}
