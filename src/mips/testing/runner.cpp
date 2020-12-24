#include "runner.hpp"

#include <map>
#include <iostream>
#include <sstream>
#include <runtime.hpp>
#include <utils/benchmark.hpp>
#include <utils/strtools.hpp>

namespace mips::testing
{
    std::vector<TestResult> Runner::run(const std::vector<Test>& tests)
    {
        std::cout << "Running tests\n";

        size_t pass_count = 0;
        std::vector<std::tuple<Test, std::string>> failures;
        std::vector<TestResult> results;

        for (const auto& test : tests)
        {
            Runtime runtime;
            TestResult result;
            result.name = test.name;

            std::cout << "   - " << test.name;

            try
            {
                for (const auto& initializer : test.initializers)
                    initializer.invoke(runtime.get_regs());

                runtime.execute(utils::copy(test.code));

                bool failed = false;
                std::stringstream ss;
                for (const auto& assertion : test.assertions)
                {
                    if (!assertion.evaluate(runtime.get_regs()))
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
                    ss << runtime.get_debug_with_dumps();
                    failures.push_back(std::tuple(test, ss.str()));
                }
                else
                {
                    pass_count++;
                    result.status = TestResult::Status::Passed;
                    result.time = measure_execution_time(test);
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
        {
            std::cout << "\n";
            std::cout << colorize(test.name, strtools::AnsiColor::Red) << "\n";
            if (!test.description.empty())
                std::cout << test.description << "\n";
            std::cout << err;
        }

        std::cout << strtools::catf("\n%d/%d tests passed\n", pass_count, tests.size());
        return results;
    }

    std::chrono::duration<double> Runner::measure_execution_time(const Test& test) const
    {
        constexpr size_t batch_size = config::debug
            ? 1
            : 100;

        constexpr double precision = config::debug
            ? 0.1
            : 0.01;

        return benchmark::measure_auto([&]
        {
            Runtime runtime;

            for (const auto& initializer : test.initializers)
                initializer.invoke(runtime.get_regs());

            runtime.execute(utils::copy(test.code));
        }, batch_size, precision, 10);
    }
}
