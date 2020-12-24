#include "test_result.hpp"

#include <utils/csv.hpp>

namespace mips::testing
{
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
    o << "name, status";
}

template<> void csv::write_row<mips::testing::TestResult>(std::ostream& o, const mips::testing::TestResult& result)
{
    o
    << result.name << ", "
    << result.status;
}