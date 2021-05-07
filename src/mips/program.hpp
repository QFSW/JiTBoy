#pragma once

#include <vector>
#include <mips/instruction.hpp>

namespace mips
{
    struct Program
    {
        uint32_t start_addr;
        std::vector<Instruction> source;

        [[nodiscard]] bool valid_addr(uint32_t addr) const noexcept;
        [[nodiscard]] Instruction& at(uint32_t addr);
    };
}