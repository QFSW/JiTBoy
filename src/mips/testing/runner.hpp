#pragma once

#include <type_traits>

// Try to reduce
#include <map>
#include <iostream>
#include <sstream>
#include <emulation/runtime.hpp>
#include <utils/benchmark.hpp>
//

#include <emulation/emulator.hpp>
#include <emulation/interpreter.hpp>
#include <mips/testing/test.hpp>
#include <mips/testing/test_result.hpp>
#include <utils/utils.hpp>

namespace mips::testing
{
    class Runner
    {
    public:
        template <typename Emulator>
        std::vector<TestResult> run(const std::vector<Test>& tests);

    private:
        // add debug_stream

        void log_test_failure(const Test& test, const std::string& error);

        template <typename Emulator>
        void execute_test(Emulator& emulator, const Test& test) const;

        template <typename Emulator>
        std::chrono::duration<double> measure_execution_time(const Test& test) const;

        template <typename Emulator>
        void get_statistics(const Emulator& emulator, TestResult& result) const;
    };


    // Try to move as much of this out to the .cpp as possible
    template <typename Emulator>
    std::vector<TestResult> Runner::run(const std::vector<Test>& tests)
    {
        static_assert(std::is_base_of<emulation::Emulator, Emulator>::value, "Runner::run requires an emulator type");
        std::cout << "Running tests" << "\n";

        size_t pass_count = 0;
        std::vector<std::tuple<Test, std::string>> failures;
        std::vector<TestResult> results;

        for (const auto& test : tests)
        {
            Emulator emulator;
            TestResult result;
            result.name = test.name;

            std::cout << "   - " << test.name;

            try
            {
                execute_test(emulator, test);

                bool failed = false;
                std::stringstream ss;
                for (const auto& assertion : test.assertions)
                {
                    if (!assertion.evaluate(emulator.get_regs()))
                    {
                        if (!failed)
                        {
                            result.status = TestResult::Status::Failed;
                            std::cout << colorize(" failed\n", strtools::AnsiColor::Red);
                        }

                        failed = true;
                        ss << "Assertion failed: " << assertion << "\n";
                    }
                }

                if (failed)
                {
                    ss << emulator.get_debug_with_dumps();
                    failures.push_back(std::tuple(test, ss.str()));
                }
                else
                {
                    pass_count++;
                    result.status = TestResult::Status::Passed;
                    result.time = measure_execution_time<Emulator>(test);
                    get_statistics(emulator, result);

                    std::cout << colorize(" pass\n", strtools::AnsiColor::Green);
                }
            }
            catch (const std::exception& e)
            {
                result.status = TestResult::Status::Faulted;
                std::cout << colorize(" faulted\n", strtools::AnsiColor::Red);

                std::stringstream ss;
                ss << e.what() << "\n";
                failures.push_back(std::tuple(test, ss.str()));
            }

            results.push_back(std::move(result));
        }

        for (const auto& [test, err] : failures)
            log_test_failure(test, err);

        std::cout << strtools::catf("\n%d/%d tests passed\n", pass_count, tests.size());
        return results;
    }

    template <typename Emulator>
    void Runner::execute_test(Emulator& emulator, const Test& test) const
    {
        for (const auto& initializer : test.initializers)
            initializer.invoke(emulator.get_regs());

        emulator.execute(utils::copy(test.code));
    }

    template <typename Emulator>
    std::chrono::duration<double> Runner::measure_execution_time(const Test& test) const
    {
        constexpr size_t batch_size = config::debug
            ? 1
            : 10;

        constexpr double precision = config::debug
            ? 0.1
            : 0.01;

        return benchmark::measure_auto([&]
        {
            Emulator emulator;
            execute_test(emulator, test);
        }, batch_size, precision, 10);
    }

    template <typename Emulator>
    void Runner::get_statistics(const Emulator&, TestResult&) const { }

    template<>
    inline void Runner::get_statistics<emulation::Runtime>(const emulation::Runtime& emulator, TestResult& result) const
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

    template<>
    inline void Runner::get_statistics<emulation::Interpreter>(const emulation::Interpreter& emulator, TestResult& result) const
    {
        result.source_instrs_emulated = emulator.get_instruction_count();
    }
}