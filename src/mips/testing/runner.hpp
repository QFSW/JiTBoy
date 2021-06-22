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

        Runner();
        Runner(Config config);

        void initialize_test(emulation::Emulator& emulator, const Test& test) const;
        void evaluate_test(emulation::Emulator& emulator, const Test& test, TestResult& result) const;
        TestResult execute_test(emulation::Emulator& emulator, const Test& test) const;

        template <typename Emulator>
        std::vector<TestResult> run(const std::vector<Test>& tests);

        template <typename Emulator>
        std::vector<TestResult> run(const std::vector<Test>& tests, const typename Emulator::Config& emulator_config);

    private:
        Config _config;

        void log_test_failure(const Test& test, const TestResult& result) const;

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

        size_t executed_count = 0;
        size_t executed_digits = utils::count_digits(tests.size());

        size_t pass_count = 0;
        size_t fail_count = 0;
        size_t fault_count = 0;

        std::vector<TestResult> results;

        size_t name_width = 0;
        for (const auto& test : tests)
        {
            name_width = (std::max)(name_width, test.name.length());
        }

        for (const auto& test : tests)
        {
            executed_count++;
            Emulator emulator(emulator_config);

            constexpr auto counter_col = strtools::AnsiColor::Blue;
            constexpr auto counter_delim = "0";

            std::cout << strtools::catf("[%s/%s] ",
                strtools::colorize(strtools::fixed_digits(executed_count, executed_digits, counter_delim), counter_col).c_str(),
                strtools::colorize(strtools::fixed_digits(tests.size(), executed_digits, counter_delim), counter_col).c_str()
            );

            std::cout << std::left << std::setw(name_width) << test.name << std::right;

            TestResult result = execute_test(emulator, test);

            switch (result.status)
            {
                case TestResult::Status::Passed:
                {
                    pass_count++;
                    std::cout << colorize(" pass\n", strtools::AnsiColor::Green);
                    result.time = measure_execution_time<Emulator>(test, emulator_config);
                    break;
                }
                case TestResult::Status::Failed:
                {
                    fail_count++;
                    std::cout << colorize(" failed\n", strtools::AnsiColor::Red);
                    break;
                }
                case TestResult::Status::Faulted:
                {
                    fault_count++;
                    std::cout << colorize(" faulted\n", strtools::AnsiColor::Red);
                    break;
                }
                default: break;
            }           

            results.push_back(std::move(result));
        }

        for (int i = 0; i < tests.size(); i++)
            log_test_failure(tests[i], results[i]);

        const std::string pass_ctr = strtools::colorize(
            strtools::catf("%d/%d", pass_count, tests.size()),
            pass_count == tests.size() ? strtools::AnsiColor::Green : strtools::AnsiColor::Red
        );

        const std::string fail_ctr = strtools::colorize(
            strtools::catf("%d", fail_count),
            fail_count == 0 ? strtools::AnsiColor::Green : strtools::AnsiColor::Red
        );

        const std::string fault_ctr = strtools::colorize(
            strtools::catf("%d", fault_count),
            fault_count == 0 ? strtools::AnsiColor::Green : strtools::AnsiColor::Red
        );

        std::cout << strtools::catf("\n%s tests passed, %s failed, %s faulted\n", pass_ctr.c_str(), fail_ctr.c_str(), fault_ctr.c_str());
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