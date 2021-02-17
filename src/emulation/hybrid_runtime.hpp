#pragma once

#include <threading/thread_pool.hpp>
#include <emulation/emulator.hpp>
#include <emulation/compiler.hpp>
#include <emulation/interpreter_core.hpp>

namespace emulation
{
    class HybridRuntime final : public Emulator
    {
    public:
        HybridRuntime();
        ~HybridRuntime();

        void execute(std::vector<mips::Instruction>&& code) override;
        void load_source(std::vector<mips::Instruction>&& code, uint32_t addr = instruction_mem_addr);
        void execute(uint32_t addr);

        [[nodiscard]] EmulatorState& get_state() noexcept override { return _state; }
        [[nodiscard]] const EmulatorState& get_state() const noexcept override { return _state; }
        [[nodiscard]] const common::unordered_map<uint32_t, CompiledBlock>& get_blocks() const noexcept { return _blocks; }
        [[nodiscard]] std::string get_debug() const;
        [[nodiscard]] std::string get_debug_with_dumps() const;

    private:
        EmulatorState _state;
        InterpreterCore _interpreter;
        threading::ThreadPool<Compiler> _compiler_pool;
        common::unordered_map<uint32_t, CompiledBlock> _blocks;
        common::unordered_map<uint32_t, size_t> _block_requests;

        struct Result
        {
            uint32_t addr;
            CompiledBlock block;
        };

        threading::concurrent_queue<Result> _result_queue;

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

