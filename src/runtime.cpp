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
		_blocks[addr] = func;

		if constexpr (debug)
		{
			_debug_stream << _compiler.get_debug() << "\n"
						  << strtools::catf("Registering compiled block %p to 0x%x\n", func, addr);
		}
	}
	else func = _blocks[addr];

	if constexpr (debug) _debug_stream << strtools::catf("Executing block 0x%x\n\n", addr);
	func();

	if constexpr (debug)
	{
		_debug_stream << "Register file (zeroed registers omitted)\n";
		for (int i = 0; i < _regs.size(); i++)
		{
			const auto reg = _regs[i];
			if (reg > 0) _debug_stream << strtools::catf("$%d: %d\n", i, reg);
		}
	}
}

std::string Runtime::get_debug() const
{
	return _debug_stream.str();
}
