#pragma once

#include <span>
#include <vector>
#include <mips/instruction.hpp>

struct SourceBlock
{
    SourceBlock(std::span<const mips::Instruction>&& code, uint32_t addr);
    SourceBlock(const std::vector<mips::Instruction>& code, uint32_t addr);

    std::span<const mips::Instruction> code;
    uint32_t addr;
};