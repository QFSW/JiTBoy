#include "compiled_block.hpp"

CompiledBlock::CompiledBlock()
    : code(nullptr)
    , size(0)
    , host_instr_count(0)
    , source_instr_count(0)
    , execution_count(0)
{ }

uint32_t CompiledBlock::operator()() const
{
    execution_count++;
    return code();
}
