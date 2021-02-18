#include "runner.hpp"

namespace mips::testing
{
    void Runner::execute_test(emulation::Emulator& emulator, const Test& test) const
    {
        for (const auto& initializer : test.initializers)
            initializer.invoke(emulator.get_state().regs);

        emulator.execute(utils::copy(test.code));
    }

    void Runner::log_test_failure(const Test& test, const std::string& error)
    {
        std::cout << "\n";
        std::cout << colorize(test.name, strtools::AnsiColor::Red) << "\n";

        if (!test.description.empty())
            std::cout << test.description << "\n";

        std::cout << error;
    }

    template<> void Runner::get_statistics<emulation::Runtime>(const emulation::Runtime& emulator, TestResult& result) const
    {
        for (const auto& [_, block] : emulator.get_blocks())
        {
            result.block_count++;
            result.blocks_executed += block.execution_count;
            result.host_instr_count += block.host_instr_count;
            result.source_instr_count += block.source_instr_count;
            result.host_instrs_executed += block.host_instr_count * block.execution_count;
            result.source_instrs_emulated += block.source_instr_count * block.execution_count;
        }
    }

    template<> void Runner::get_statistics<emulation::Interpreter>(const emulation::Interpreter& emulator, TestResult& result) const
    {
        result.source_instrs_emulated = emulator.get_instruction_count();
    }

    template<> void Runner::get_statistics<emulation::HybridRuntime>(const emulation::HybridRuntime& emulator, TestResult& result) const
    {
        result.source_instrs_emulated = emulator.get_interpreted_instruction_count();
        for (const auto& [_, block] : emulator.get_blocks())
        {
            result.block_count++;
            result.blocks_executed += block.execution_count;
            result.host_instr_count += block.host_instr_count;
            result.source_instr_count += block.source_instr_count;
            result.host_instrs_executed += block.host_instr_count * block.execution_count;
            result.source_instrs_emulated += block.source_instr_count * block.execution_count;
        }
    }
}
