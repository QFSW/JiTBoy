#pragma once

#include <string>
#include <vector>
#include <mips/instruction.hpp>

namespace mips::testing
{
    struct Test
    {
        std::string name;
        std::string src;
        std::vector<Instruction> code;
        // assertions
        // initializers
    };
}

