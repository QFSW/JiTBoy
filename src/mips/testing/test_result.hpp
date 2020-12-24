#pragma once

#include <string>
#include <chrono>

namespace mips::testing
{
    struct TestResult
    {
        enum class Status
        {
            Passed,
            Failed,
            Faulted
        };

        std::string name;
        Status status;
        std::chrono::duration<double> time;
        uint32_t source_instr_count;

        TestResult();
    };
}