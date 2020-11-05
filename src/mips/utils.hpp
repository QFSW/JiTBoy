#pragma once

#include <mips/instruction.hpp>

namespace mips::utils
{
    constexpr bool is_branch_instr(const Instruction& instr);
}