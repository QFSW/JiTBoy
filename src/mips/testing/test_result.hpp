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

        TestResult();
    };
}