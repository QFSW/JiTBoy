#include "runner.hpp"

namespace mips::testing
{
    Runner::Runner()
        : Runner(Config())
    { }

    Runner::Runner(Config config)
        : _config(config)
    { }

    void Runner::initialize_test(emulation::Emulator& emulator, const Test& test) const
    {
        for (const auto& initializer : test.initializers)
            initializer.invoke(emulator.get_state().regs);
    }

    void Runner::evaluate_test(emulation::Emulator& emulator, const Test& test, TestResult& result) const
    {
        result.status = TestResult::Status::Passed;
        std::stringstream errors;

        for (const auto& assertion : test.assertions)
        {
            if (!assertion.evaluate(emulator.get_state().regs))
            {
                result.status = TestResult::Status::Failed;
                errors << "Assertion failed: " << assertion << "\n";
            }
        }

        if (result.status == TestResult::Status::Failed)
        {
            errors << emulator.get_debug_with_dumps();
            result.errors = errors.str();
        }
        else
        {
            get_statistics(emulator, result);
        }
    }

    TestResult Runner::execute_test(emulation::Emulator& emulator, const Test& test) const
    {
        TestResult result;
        result.name = test.name;

        initialize_test(emulator, test);

        try
        {
            emulator.execute(utils::copy(test.program));
        }
        catch (const std::exception& e)
        {
            result.status = TestResult::Status::Faulted;
            result.errors = strtools::catf("%s\n", e.what());
        }

        if (result.status != TestResult::Status::Faulted)
            evaluate_test(emulator, test, result);

        return result;
    }

    void Runner::log_test_failure(const Test& test, const TestResult& result) const
    {
        if (result.status == TestResult::Status::Passed)
            return;

        std::cout << "\n";
        std::cout << colorize(test.name, strtools::AnsiColor::Red) << "\n";

        if (!test.description.empty())
            std::cout << test.description << "\n";

        std::cout << result.errors;
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
