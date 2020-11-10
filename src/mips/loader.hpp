#pragma once

#include <vector>
#include <mips/instruction.hpp>

namespace mips
{
    class Loader
    {
    public:
        static std::vector<Instruction> load_assembly(const std::string& filepath);
        static std::vector<Instruction> load_binary(const std::string& filepath);
    };
}

