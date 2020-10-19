#include "runtime.hpp"
#include <iostream>

Runtime::Runtime()
	: _compiler(_regs, _allocator)
{ }

void Runtime::execute(const std::vector<mips::Instruction>& code)
{
	const uint64_t addr = 0;

	Compiler::func func;
	if (_blocks.find(addr) == _blocks.end())
	{
		func = _compiler.compile(code);
		std::cout << _compiler.get_debug() << "\n";
		std::cout << strtools::catf("Registering compiled block %p to 0x%x\n", func, addr);
		_blocks[addr] = func;
	}
	else func = _blocks[addr];

	std::cout << strtools::catf("Executing block 0x%x\n\n", addr);
	func();

	for (int i = 0; i < _regs.size(); i++)
	{
		std::cout << strtools::catf("$%d: %d\n", i, _regs.data()[i]);
	}
}
