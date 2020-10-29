#include "source_block.hpp"

SourceBlock::SourceBlock(std::span<const mips::Instruction>&& code, const uint32_t addr)
    : code(code)
    , addr(addr)
{ }

SourceBlock::SourceBlock(const std::vector<mips::Instruction>& code, const uint32_t addr)
    : code(code)
    , addr(addr)
{ }
