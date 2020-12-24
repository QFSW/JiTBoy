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
        // add debug_stream;
    };
}