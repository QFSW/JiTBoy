#pragma once

#include <mips/testing/test.hpp>

namespace mips::testing
{
    class Runner
    {
    public:
        // make this return a TestResult
        void run(const std::vector<Test>& tests);

    private:
        // add debug_stream;
    };
}