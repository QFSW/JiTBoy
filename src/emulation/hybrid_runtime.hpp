#pragma once

#include <threading/worker_pool.hpp>
#include <emulation/hybrid_runtime_config.hpp>
#include <emulation/emulator.hpp>
#include <emulation/compiler.hpp>
#include <emulation/interpreter_core.hpp>
#include <emulation/jump_resolver.hpp>

namespace emulation
{
    class HybridRuntime final : public Emulator
    {
    public:
        using Config = HybridRuntimeConfig;
        HybridRuntime();
        HybridRuntime(Config config);
        ~HybridRuntime();

        void execute(std::vector<mips::Instruction>&& code) override;
        void load_source(std::vector<mips::Instruction>&& code, uint32_t addr = instruction_mem_addr);
        void execute(uint32_t addr);

        [[nodiscard]] EmulatorState& get_state() noexcept override { return _state; }
        [[nodiscard]] const EmulatorState& get_state() const noexcept override { return _state; }
        [[nodiscard]] const common::unordered_map<uint32_t, CompiledBlock>& get_blocks() const noexcept { return _blocks; }
        [[nodiscard]] size_t get_interpreted_instruction_count() const noexcept { return _interpreted_instructions; }
        [[nodiscard]] std::string get_debug() const;
        [[nodiscard]] std::string get_debug_with_dumps() const;

    private:
        Config _config;
        EmulatorState _state;
        InterpreterCore _interpreter;
        JumpResolver _jump_resolver;
        threading::WorkerPool<int> _worker_pool;
        common::concurrent_queue<std::unique_ptr<Compiler>> _compiler_pool;
        common::unordered_map<uint32_t, CompiledBlock> _blocks;
        common::unordered_map<uint32_t, size_t> _block_requests;
        size_t _interpreted_instructions;

        struct Result
        {
            uint32_t addr;
            CompiledBlock block;
        };

        common::concurrent_queue<Result> _result_queue;

        static constexpr uint32_t instruction_mem_addr = 0x0;
        static constexpr bool debug = config::debug;
        std::stringstream _debug_stream;

        [[nodiscard]] bool valid_pc(uint32_t addr) const noexcept;
        [[nodiscard]] SourceBlock partition_block(uint32_t addr) const;
        [[nodiscard]] const CompiledBlock* try_get_block(uint32_t addr);
        void compile_block(uint32_t addr);
        void consume_results();
    };
}

