#pragma once

#define NOMINMAX

#include <type_traits>

// Try to reduce
#include <iostream>
#include <sstream>
#include <iomanip>
//

#include <mips/testing/runner_config.hpp>
#include <emulation/runtime.hpp>
#include <emulation/emulator.hpp>
#include <emulation/interpreter.hpp>
#include <emulation/hybrid_runtime.hpp>
#include <mips/testing/test.hpp>
#include <mips/testing/test_result.hpp>
#include <utils/utils.hpp>
#include <utils/benchmark.hpp>

namespace mips::testing
{
    class Runner
    {
    public:
        using Config = RunnerConfig;

        Runner(Config config);

        template <typename Emulator>
        std::vector<TestResult> run(const std::vector<Test>& tests);

        template <typename Emulator>
        std::vector<TestResult> run(const std::vector<Test>& tests, const typename Emulator::Config& emulator_config);

    private:
        Config _config;

        void execute_test(emulation::Emulator& emulator, const Test& test) const;
        void log_test_failure(const Test& test, const std::string& error);

        template <typename Emulator>
        std::chrono::duration<double> measure_execution_time(const Test& test, const typename Emulator::Config& emulator_config) const;

        template <typename Emulator>
        void get_statistics(const Emulator& emulator, TestResult& result) const;
    };

    template <typename Emulator>
    std::vector<TestResult> Runner::run(const std::vector<Test>& tests)
    {
        return run<Emulator>(tests, typename Emulator::Config());
    }

    template <typename Emulator>
    std::vector<TestResult> Runner::run(const std::vector<Test>& tests, const typename Emulator::Config& emulator_config)
    {
        static_assert(std::is_base_of<emulation::Emulator, Emulator>::value, "Runner::run requires an emulator type");
        std::cout << "Running tests" << "\n";

        size_t pass_count = 0;
        std::vector<std::tuple<Test, std::string>> failures;
        std::vector<TestResult> results;

        size_t name_width = 0;
        for (const auto& test : tests)
        {
            name_width = (std::max)(name_width, test.name.length());
        }

        for (const auto& test : tests)
        {
            Emulator emulator(emulator_config);
            TestResult result;
            result.name = test.name;

            std::cout << "   - " << std::left << std::setw(name_width) << test.name << std::right;

            try
            {
                execute_test(emulator, test);

                bool failed = false;
                std::stringstream ss;
                for (const auto& assertion : test.assertions)
                {
                    if (!assertion.evaluate(emulator.get_state().regs))
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
                    result.time = measure_execution_time<Emulator>(test, emulator_config);
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
    std::chrono::duration<double> Runner::measure_execution_time(const Test& test, const typename Emulator::Config& emulator_config) const
    {
        return benchmark::measure_auto([&]
        {
            Emulator emulator(emulator_config);
            execute_test(emulator, test);
        }, _config.timing.batch_size, _config.timing.precision, _config.timing.threshold);
    }

    template <typename Emulator>
    void Runner::get_statistics(const Emulator&, TestResult&) const { }

    template<> void Runner::get_statistics<emulation::Runtime>(const emulation::Runtime& emulator, TestResult& result) const;
    template<> void Runner::get_statistics<emulation::Interpreter>(const emulation::Interpreter& emulator, TestResult& result) const;
    template<> void Runner::get_statistics<emulation::HybridRuntime>(const emulation::HybridRuntime& emulator, TestResult& result) const;
}