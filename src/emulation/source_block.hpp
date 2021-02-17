#pragma once

#include <span>
#include <mips/instruction.hpp>

namespace emulation
{
    struct SourceBlock
    {
        SourceBlock(std::span<const mips::Instruction> code, uint32_t addr);

        std::span<const mips::Instruction> code;
        uint32_t addr;
    };
}