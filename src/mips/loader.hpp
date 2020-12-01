#pragma once

#include <vector>

#include <mips/instruction.hpp>
#include <mips/parser.hpp>

namespace mips
{
    class Loader
    {
    public:
        [[nodiscard]] std::vector<Instruction> load_assembly(const std::string& filepath);
        [[nodiscard]] std::vector<Instruction> load_binary(const std::string& filepath);
        [[nodiscard]] std::vector<Instruction> load_auto(const std::string& filepath);

    private:
        Parser _parser;
    };
}

