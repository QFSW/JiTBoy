#include "runtime.hpp"
#include <iostream>

Runtime::Runtime()
    : _compiler(_regs, _allocator)
{ }

void Runtime::execute(const std::vector<mips::Instruction>& code)
{
    const SourceBlock input(code, 0);

    CompiledBlock block;
    if (_blocks.find(input.addr) == _blocks.end())
    {
        block = _compiler.compile(input, CompilerConfig());
        _blocks[input.addr] = block;

        if constexpr (debug)
        {
            _debug_stream << _compiler.get_debug() << "\n"
                          << strtools::catf("Registering compiled block %p to 0x%x\n", block.code, input.addr);
        }
    }
    else block = _blocks[input.addr];

    if constexpr (debug) _debug_stream << strtools::catf("Executing block 0x%x\n\n", input.addr);
    block();

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
