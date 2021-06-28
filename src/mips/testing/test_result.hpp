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
        std::string errors;

        std::chrono::duration<double> time;
        size_t block_count;
        size_t host_instr_count;
        size_t source_instr_count;
        size_t blocks_executed;
        size_t host_instrs_executed;
        size_t source_instrs_emulated;

        TestResult();
    };
}