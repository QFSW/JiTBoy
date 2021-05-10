#include "hybrid_runtime.hpp"

#include <common/environment.hpp>
#include <mips/utils.hpp>

namespace emulation
{
    HybridRuntime::HybridRuntime()
        : HybridRuntime(Config())
    { }

    HybridRuntime::HybridRuntime(Config config)
        : _config(config)
        , _interpreter(_state)
        , _worker_pool(
              &common::Environment::get().thread_pool()
            , [this] { return make_compiler(); }
        )
        , _interpreted_instructions(0)
    { }

    HybridRuntime::~HybridRuntime()
    {
        _worker_pool.shutdown();
    }

    void HybridRuntime::load_source(mips::Program&& program)
    {
        _state.program = std::move(program);
    }

    const CompiledBlock* HybridRuntime::try_get_block(const uint32_t addr)
    {
        consume_results();
        if (const auto it = _blocks.find(addr); it != _blocks.end())
            return &it->second;

        if (++_block_requests[addr] == _config.compilation_threshold)
            compile_block(addr);

        return nullptr;
    }

    void HybridRuntime::compile_block(const uint32_t addr)
    {
        _worker_pool.schedule_job(threading::WorkerJob<Compiler>([this, addr](Compiler& compiler)
        {
            const SourceBlock input = _block_partitioner.partition_block(_state.program, addr);
            CompiledBlock block = compiler.compile(input, CompilerConfig{
                .direct_linking = _config.direct_linking
            });

            /*if constexpr (debug)
            {
                _debug_stream << _compiler.get_debug() << "\n";
            }*/

            _result_queue.enqueue(Result{
                .addr = addr,
                .block = std::move(block),
            });
        }));
    }

    void HybridRuntime::consume_results()
    {
        if (!_result_queue.expecting_results())
            return;

        size_t consumed = 0;
        Result result;

        _worker_pool.handle_exceptions(std::rethrow_exception);

        while (_result_queue.try_dequeue(result))
        {
            _blocks[result.addr] = result.block;
            consumed++;

            if constexpr (debug)
            {
                _debug_stream << strtools::catf("Registering compiled block 0x%p to 0x%x\n", result.block.code, result.addr);
            }
        }

        if (_config.direct_linking && consumed > 0)
        {
            for (auto it = _blocks.begin(); it != _blocks.end(); ++it)
            {
                _jump_resolver.resolve_jumps(it.value(), _blocks);
                if constexpr (debug) _debug_stream << _jump_resolver.get_debug();
            }
        }
    }

    std::unique_ptr<Compiler> HybridRuntime::make_compiler()
    {
        return std::make_unique<Compiler>(_state.regs, _state.mem);
    }

    void HybridRuntime::execute(mips::Program&& program)
    {
        load_source(std::move(program));
        execute(_state.program.start_addr);
    }

    void HybridRuntime::execute(const uint32_t addr)
    {
        _state.pc = addr;
        bool jumped = true;

        while (_state.valid_pc())
        {
            if (jumped)
            {
                if (const CompiledBlock* block = try_get_block(_state.pc))
                {
                    _state.pc = (*block)();
                    continue;
                }
            }

            jumped = mips::utils::is_branch_instr(_state.program.at(_state.pc));
            _interpreter.execute_current();
            _interpreted_instructions++;
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