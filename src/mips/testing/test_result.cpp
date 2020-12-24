#include "test_result.hpp"

#include <utils/csv.hpp>

namespace mips::testing
{
    TestResult::TestResult()
        : name()
        , status()
        , time()
    { }

    std::ostream& operator<<(std::ostream& o, const TestResult::Status status)
    {
        switch (status)
        {
            case TestResult::Status::Passed: o << "Passed"; break;
            case TestResult::Status::Failed: o << "Failed"; break;
            case TestResult::Status::Faulted: o << "Faulted"; break;
            default: o << "???"; break;
        }

        return o;
    }
}

template<> void csv::write_header<mips::testing::TestResult>(std::ostream& o)
{
    o << "name, status, time (us)";
}

template<> void csv::write_row<mips::testing::TestResult>(std::ostream& o, const mips::testing::TestResult& result)
{
    o
    << result.name << ", "
    << result.status << ", "
    << std::chrono::duration_cast<std::chrono::microseconds>(result.time).count();
}