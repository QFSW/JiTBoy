#include "compiled_block.hpp"

CompiledBlock::CompiledBlock()
	: code(nullptr)
	, size(0)
{ }

CompiledBlock::CompiledBlock(func code, size_t size, CompilerConfig config)
	: code(code)
	, size(size)
	, config(config)
{ }

void CompiledBlock::operator()() const
{
	code();
}
