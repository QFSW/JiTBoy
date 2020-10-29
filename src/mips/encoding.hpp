#pragma once
#include <mips/instruction.hpp>

namespace mips
{
    Instruction decode_instruction(uint32_t binary);
    InstructionR decode_instruction_r(uint32_t binary);
    InstructionI decode_instruction_i(uint32_t binary);
    InstructionJ decode_instruction_j(uint32_t binary);

    uint32_t encode_instruction(Instruction instr);
    uint32_t encode_instruction(InstructionR instr);
    uint32_t encode_instruction(InstructionI instr);
    uint32_t encode_instruction(InstructionJ instr);
}