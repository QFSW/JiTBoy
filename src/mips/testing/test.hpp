#pragma once

#include <string>
#include <vector>
#include <mips/instruction.hpp>
#include <mips/testing/assertion.hpp>
#include <mips/testing/initializer.hpp>

namespace mips::testing
{
    struct Test
    {
        std::string name;
        std::string src;
        std::vector<Instruction> code;
        std::vector<Assertion> assertions;
        std::vector<Initializer> initializers;
    };
}

