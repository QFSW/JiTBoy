#pragma once

#include <threading/worker_pool.hpp>
#include <threading/result_queue.hpp>
#include <emulation/hybrid_runtime_config.hpp>
#include <emulation/emulator.hpp>
#include <emulation/compiler.hpp>
#include <emulation/interpreter_core.hpp>
#include <emulation/jump_resolver.hpp>
#include <emulation/block_partitioner.hpp>

namespace emulation
{
    class HybridRuntime final : public Emulator
    {
    public:
        using Config = HybridRuntimeConfig;
        HybridRuntime();
        HybridRuntime(Config config);
        ~HybridRuntime();

        void execute(mips::Program&& program) override;
        void load_source(mips::Program&& program);
        void execute(uint32_t addr);

        [[nodiscard]] EmulatorState& get_state() noexcept override { return _state; }
        [[nodiscard]] const EmulatorState& get_state() const noexcept override { return _state; }
        [[nodiscard]] const common::unordered_map<uint32_t, CompiledBlock>& get_blocks() const noexcept { return _blocks; }
        [[nodiscard]] size_t get_interpreted_instruction_count() const noexcept { return _interpreted_instructions; }
        [[nodiscard]] std::string get_debug() const override;
        [[nodiscard]] std::string get_debug_with_dumps() const override;

    private:
        Config _config;
        EmulatorState _state;
        InterpreterCore _interpreter;
        JumpResolver _jump_resolver;
        BlockPartitioner _block_partitioner;
        threading::WorkerPool<Compiler> _worker_pool;
        common::concurrent_queue<std::unique_ptr<Compiler>> _compiler_pool;
        common::unordered_map<uint32_t, CompiledBlock> _blocks;
        common::unordered_map<uint32_t, size_t> _block_requests;
        size_t _interpreted_instructions;

        struct Result
        {
            uint32_t addr;
            CompiledBlock block;
        };

        threading::ResultQueue<Result> _result_queue;

        static constexpr bool debug = config::debug;
        std::stringstream _debug_stream;

        [[nodiscard]] const CompiledBlock* try_get_block(uint32_t addr);
        void compile_block(uint32_t addr);
        void consume_results();
        void relink_blocks();
        void speculatively_compile();
        std::unique_ptr<Compiler> make_compiler();
    };
}

