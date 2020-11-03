#pragma once

#include <string>
#include <vector>

#include <mips/instruction.hpp>

namespace mips
{
    class Parser
    {
    public:
        static Instruction parse_instruction(const std::string& instr);
        static Register parse_register(const std::string& reg);

    private:
        static InstructionR parse_instruction_r(OpcodeR opcode, const std::vector<std::string>& parts);
        static InstructionI parse_instruction_i(OpcodeI opcode, const std::vector<std::string>& parts);
        static InstructionJ parse_instruction_j(OpcodeJ opcode, const std::vector<std::string>& parts);
    };
}
