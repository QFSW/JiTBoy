#pragma once

#include <mips/testing/test.hpp>
#include <mips/testing/test_result.hpp>

namespace mips::testing
{
    class Runner
    {
    public:
        std::vector<TestResult> run(const std::vector<Test>& tests);

    private:
        [[nodiscard]] std::chrono::duration<double> measure_execution_time(const Test& test) const;
        // add debug_stream;
    };
}