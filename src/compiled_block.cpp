#include "compiled_block.hpp"

CompiledBlock::CompiledBlock()
    : code(nullptr)
    , size(0)
{ }

CompiledBlock::CompiledBlock(func code, const size_t size, const CompilerConfig config)
    : code(code)
    , size(size)
    , config(config)
{ }

uint32_t CompiledBlock::operator()() const
{
    return code();
}
