#include "runner.hpp"

namespace mips::testing
{
    void Runner::log_test_failure(const Test& test, const std::string& error)
    {
        std::cout << "\n";
        std::cout << colorize(test.name, strtools::AnsiColor::Red) << "\n";

        if (!test.description.empty())
            std::cout << test.description << "\n";

        std::cout << error;
    }
}
