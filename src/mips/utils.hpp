#pragma once

#include <mips/instruction.hpp>

namespace mips::utils
{
    bool is_branch_instr(const Instruction& instr);
}