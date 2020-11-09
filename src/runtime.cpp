#include "runtime.hpp"

#include <iostream>
#include <stdexcept>

Runtime::Runtime()
    : _compiler(_regs, _allocator)
    , _current_pc(instruction_mem_addr)
{ }

void Runtime::load_source(std::vector<mips::Instruction>&& code, const uint32_t addr)
{
    _source = std::move(code);
}

bool Runtime::valid_pc(const uint32_t addr) const noexcept
{
    if (addr < instruction_mem_addr)
        return false;
    if (addr >= instruction_mem_addr + _source.size() * 4)
        return false;
    return true;
}

SourceBlock Runtime::partition_block(const uint32_t addr) const
{
    if (!valid_pc(addr))
        throw std::logic_error(strtools::catf("Address 0x%x is out of bounds", addr));

    const size_t start_index = (addr - instruction_mem_addr) / 4;

    return SourceBlock(_source, addr);
}

CompiledBlock Runtime::get_or_compile_block(const uint32_t addr)
{
    if (_blocks.find(addr) != _blocks.end())
        return _blocks[addr];

    const SourceBlock input = partition_block(addr);
    const CompiledBlock block = _compiler.compile(input, CompilerConfig());
    _blocks[input.addr] = block;

    if constexpr (debug)
    {
        _debug_stream << _compiler.get_debug() << "\n"
            << strtools::catf("Registering compiled block 0x%p to 0x%x\n", block.code, input.addr);
    }

    return block;
}

void Runtime::execute(std::vector<mips::Instruction>&& code)
{
    load_source(std::move(code));
    execute(instruction_mem_addr);
}

void Runtime::execute(const uint32_t addr)
{
    const CompiledBlock block = get_or_compile_block(addr);

    if constexpr (debug) _debug_stream << strtools::catf("Executing block 0x%x\n\n", addr);
    _current_pc = block();

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
