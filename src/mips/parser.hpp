#pragma once

#include <string>
#include <vector>
#include <regex>

#include <mips/instruction.hpp>

namespace mips
{
    class Parser
    {
    public:
        static Instruction parse_instruction(const std::string& instr);
        static std::vector<Instruction> parse_instructions(const std::string& assembly);

        static Register parse_register(const std::string& reg);
        static uint32_t parse_constant_32(const std::string& value);
        static uint16_t parse_constant_16(const std::string& value);

    private:
        static InstructionR parse_nop(const std::string& instr, const std::vector<std::string>& parts);
        static InstructionR parse_instruction_r(OpcodeR opcode, const std::string& instr, const std::vector<std::string>& parts);
        static InstructionR parse_instruction_r_no_dst(OpcodeR opcode, const std::string& instr, const std::vector<std::string>& parts);
        static InstructionR parse_instruction_r_1_src(OpcodeR opcode, const std::string& instr, const std::vector<std::string>& parts);
        static InstructionI parse_instruction_i(OpcodeI opcode, const std::string& instr, const std::vector<std::string>& parts);
        static InstructionI parse_instruction_i_branch(OpcodeI opcode, const std::string& instr, const std::vector<std::string>& parts);
        static InstructionJ parse_instruction_j(OpcodeJ opcode, const std::string& instr, const std::vector<std::string>& parts);

        static const std::regex comment_regex;
    };
}
