#pragma once

#include <vector>

#include <mips/instruction.hpp>
#include <mips/parser.hpp>

namespace mips
{
    class Loader
    {
    public:
        std::vector<Instruction> load_assembly(const std::string& filepath) const;
        std::vector<Instruction> load_binary(const std::string& filepath) const;

    private:
        Parser _parser;
    };
}

