#pragma once

#include <string>

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
    };
}