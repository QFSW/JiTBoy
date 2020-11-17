#pragma once

#include <string>
#include <vector>
#include <mips/instruction.hpp>
#include <mips/testing/assertion.hpp>

namespace mips::testing
{
    struct Test
    {
        std::string name;
        std::string src;
        std::vector<Instruction> code;
        std::vector<Assertion> assertions;
        // initializers
    };
}

