#pragma once

#include <string>
#include <vector>
#include <mips/program.hpp>
#include <mips/testing/assertion.hpp>
#include <mips/testing/initializer.hpp>

namespace mips::testing
{
    struct Test
    {
        std::string name;
        std::string description;
        std::string src;
        Program program;
        std::vector<Assertion> assertions;
        std::vector<Initializer> initializers;
    };
}
