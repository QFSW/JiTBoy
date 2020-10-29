#include "compiled_block.hpp"

CompiledBlock::CompiledBlock()
    : code(nullptr)
    , size(0)
{ }

CompiledBlock::CompiledBlock(const func code, const size_t size, const CompilerConfig config)
    : code(code)
    , size(size)
    , config(config)
{ }

void CompiledBlock::operator()() const
{
    code();
}
