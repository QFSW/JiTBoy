#include "hybrid_runtime.hpp"

#include <mips/utils.hpp>
#include <iostream>

namespace emulation
{
    HybridRuntime::HybridRuntime()
        : _compiler(_state.regs, _state.mem)
        , _interpreter(_state)
        , _thread_pool(1)
    {
        _state.pc = instruction_mem_addr;
    }

    HybridRuntime::~HybridRuntime()
    {
        _thread_pool.shutdown();
    }

    void HybridRuntime::load_source(std::vector<mips::Instruction>&& code, const uint32_t addr)
    {
        _state.source = std::move(code);
    }

    bool HybridRuntime::valid_pc(const uint32_t addr) const noexcept
    {
        if (addr < instruction_mem_addr)
            return false;
        if (addr >= instruction_mem_addr + _state.source.size() * 4)
            return false;
        return true;
    }

    SourceBlock HybridRuntime::partition_block(const uint32_t addr) const
    {
        if (!valid_pc(addr)) [[unlikely]]
            throw std::logic_error(strtools::catf("Address 0x%x is out of bounds", addr));

        const size_t start_index = (addr - instruction_mem_addr) / 4;
        uint32_t end_index = start_index;
        for (; end_index < _state.source.size(); end_index++)
        {
            const auto& instr = _state.source[end_index];
            if (mips::utils::is_branch_instr(instr))
            {
                end_index++;
                break;
            }
        }

        const auto code = std::span<const mips::Instruction>(_state.source.data() + start_index, end_index - start_index);
        return SourceBlock(code, addr);
    }

    const CompiledBlock* HybridRuntime::try_get_block(const uint32_t addr)
    {
        // consume_results();
        if (const auto it = _blocks.find(addr); it != _blocks.end())
            return &it->second;

        compile_block(addr);
        return nullptr;
    }

    void HybridRuntime::compile_block(const uint32_t addr)
    {
        _thread_pool.schedule_job(threading::Job([&, addr]
        {
            const SourceBlock input = partition_block(addr);
            const CompiledBlock block = _compiler.compile(input, CompilerConfig());

            /*if constexpr (debug)
            {
                _debug_stream << _compiler.get_debug() << "\n"
                    << strtools::catf("Registering compiled block 0x%p to 0x%x\n", block.code, input.addr);
            }*/

            _result_queue.push(Result{
                .addr = addr,
                .block = block,
            });
        }));
    }

    void HybridRuntime::consume_results()
    {
        while (!_result_queue.empty())
        {
            Result result = _result_queue.pop_wait();
            _blocks[result.addr] = result.block;
        }
    }

    void HybridRuntime::execute(std::vector<mips::Instruction>&& code)
    {
        load_source(std::move(code));
        execute(instruction_mem_addr);
    }

    void HybridRuntime::execute(const uint32_t addr)
    {
        _state.pc = addr;
        bool jumped = true;

        while (valid_pc(_state.pc))
        {
            if (jumped)
            {
                if (const CompiledBlock* block = try_get_block(_state.pc))
                {
                    _state.pc = (*block)();
                    continue;
                }
            }

            jumped = mips::utils::is_branch_instr(_state.source[_state.pc / 4]);
            _interpreter.execute_current();
        }
    }

    std::string HybridRuntime::get_debug() const
    {
        return _debug_stream.str();
    }

    std::string HybridRuntime::get_debug_with_dumps() const
    {
        std::stringstream ss;
        ss << get_debug() << "\n" << _state.regs.generate_dump() << "\n";
        return ss.str();
    }
}