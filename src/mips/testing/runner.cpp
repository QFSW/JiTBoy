#include "runner.hpp"

namespace mips::testing
{
    void Runner::log_test_failure(const Test& test, const std::string& error)
    {
        std::cout << "\n";
        std::cout << colorize(test.name, strtools::AnsiColor::Red) << "\n";

        if (!test.description.empty())
            std::cout << test.description << "\n";

        std::cout << error;
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
            emulation::Runtime runtime;

            for (const auto& initializer : test.initializers)
                initializer.invoke(runtime.get_regs());

            runtime.execute(utils::copy(test.code));
        }, batch_size, precision, 10);
    }
}
