#pragma once

#include <mips/instruction.hpp>

namespace mips::utils
{
    [[nodiscard]] bool is_branch_instr(const Instruction& instr);
    [[nodiscard]] bool writes_reg(const Instruction& instr, Register reg);
    [[nodiscard]] bool reads_reg(const Instruction& instr, Register reg);
}