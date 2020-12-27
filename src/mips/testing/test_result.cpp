#include "test_result.hpp"

#include <utils/csv.hpp>

namespace mips::testing
{
    TestResult::TestResult()
        : name()
        , status()
        , time(0)
        , block_count(0)
        , host_instr_count(0)
        , source_instr_count(0)
        , blocks_executed(0)
        , host_instrs_executed(0)
        , source_instrs_emulated(0)
    { }

    std::ostream& operator<<(std::ostream& o, const TestResult::Status status)
    {
        switch (status)
        {
            case TestResult::Status::Passed: return o << "passed";
            case TestResult::Status::Failed: return o << "failed";
            case TestResult::Status::Faulted: return o << "faulted";
            default: return o << "???";
        }
    }
}

template<> void csv::write_header<mips::testing::TestResult>(std::ostream& o)
{
    o <<
    "name, "
    "status, "
    "time (us), "
    "blocks, "
    "blocks executed, "
    "host instructions, "
    "source instructions, "
    "host instructions executed, "
    "source instructions emulated";
}

template<> void csv::write_row<mips::testing::TestResult>(std::ostream& o, const mips::testing::TestResult& result)
{
    o
    << result.name << ", "
    << result.status << ", "
    << result.time.count() * 1000000 << ", "
    << result.block_count << ", "
    << result.blocks_executed << ", "
    << result.host_instr_count << ", "
    << result.source_instr_count << ", "
    << result.host_instrs_executed << ", "
    << result.source_instrs_emulated;
}