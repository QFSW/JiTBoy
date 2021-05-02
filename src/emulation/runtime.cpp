#include "runtime.hpp"

#include <stdexcept>
#include <mips/utils.hpp>

namespace emulation
{
    Runtime::Runtime()
        : Runtime(Config())
    { }

    Runtime::Runtime(Config config)
        : _config(config)
        , _compiler(_state.regs, _state.mem)
    { }

    void Runtime::load_source(mips::Program&& program)
    {
        _state.program = std::move(program);
    }

    SourceBlock Runtime::partition_block(const uint32_t addr) const
    {
        if (!_state.program.valid_addr(addr)) [[unlikely]]
            throw std::logic_error(strtools::catf("Address 0x%x is out of bounds", addr));

        const size_t start_index = (addr - _state.program.start_addr) / 4;
        uint32_t end_index = start_index;
        for (; end_index < _state.program.source.size(); end_index++)
        {
            const auto& instr = _state.program.source[end_index];
            if (mips::utils::is_branch_instr(instr))
            {
                end_index++;
                break;
            }
        }

        const auto code = std::span<const mips::Instruction>(_state.program.source.data() + start_index, end_index - start_index);
        return SourceBlock(code, addr);
    }

    const CompiledBlock& Runtime::get_or_compile_block(const uint32_t addr)
    {
        if (auto it = _blocks.find(addr); it != _blocks.end())
        {
            return it->second;
        }

        const SourceBlock input = partition_block(addr);
        CompiledBlock block = _compiler.compile(input, CompilerConfig{
            .direct_linking = _config.direct_linking
        });

        if constexpr (debug)
        {
            _debug_stream << _compiler.get_debug() << "\n"
                << strtools::catf("Registering compiled block 0x%p to 0x%x\n", block.code, input.addr);
        }

        const auto& ret = _blocks[input.addr] = std::move(block);

        if (_config.direct_linking)
        {
            for (auto it = _blocks.begin(); it != _blocks.end(); ++it)
            {
                _jump_resolver.resolve_jumps(it.value(), _blocks);
                if constexpr (debug) _debug_stream << _jump_resolver.get_debug();
            }
        }

        return ret;
    }

    void Runtime::execute(mips::Program&& program)
    {
        load_source(std::move(program));
        execute(_state.program.start_addr);
    }

    void Runtime::execute(const uint32_t addr)
    {
        _state.pc = addr;
        while (_state.valid_pc())
        {
            const CompiledBlock& block = get_or_compile_block(_state.pc);

            if constexpr (debug) _debug_stream << strtools::catf("Executing block 0x%x\n", _state.pc);
            _state.pc = block();
        }
    }

    std::string Runtime::get_debug() const
    {
        return _debug_stream.str();
    }

    std::string Runtime::get_debug_with_dumps() const
    {
        std::stringstream ss;
        ss << get_debug() << "\n" << _state.regs.generate_dump() << "\n";
        return ss.str();
    }
}
