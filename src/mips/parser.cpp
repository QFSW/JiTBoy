#include "parser.hpp"

#include <map>
#include <stdexcept>

#include <utils/strtools.hpp>

namespace mips
{
    const std::regex Parser::comment_regex(R"(\s*;.*)");

    Instruction Parser::parse_instruction(const std::string& instr)
    {
        const auto parts = strtools::split(instr, ' ');

        if (parts.empty())
            throw std::logic_error("Cannot parse empty instruction");

        const auto& op = parts[0];

        if (op == "nop")    return parse_nop(instr, parts);

        if (op == "add")    return parse_instruction_r(OpcodeR::ADD, instr, parts);
        if (op == "addu")   return parse_instruction_r(OpcodeR::ADDU, instr, parts);
        if (op == "sub")    return parse_instruction_r(OpcodeR::SUB, instr, parts);
        if (op == "subu")   return parse_instruction_r(OpcodeR::SUBU, instr, parts);
        if (op == "and")    return parse_instruction_r(OpcodeR::AND, instr, parts);
        if (op == "or")     return parse_instruction_r(OpcodeR::OR, instr, parts);
        if (op == "nor")    return parse_instruction_r(OpcodeR::NOR, instr, parts);
        if (op == "xor")    return parse_instruction_r(OpcodeR::XOR, instr, parts);
        if (op == "slt")    return parse_instruction_r(OpcodeR::SLT, instr, parts);
        if (op == "sltu")   return parse_instruction_r(OpcodeR::SLTU, instr, parts);
        if (op == "sllv")   return parse_instruction_r(OpcodeR::SLLV, instr, parts);
        if (op == "srav")   return parse_instruction_r(OpcodeR::SRAV, instr, parts);
        if (op == "srlv")   return parse_instruction_r(OpcodeR::SRLV, instr, parts);

        if (op == "mult")   return parse_instruction_r_no_dst(OpcodeR::MULT, instr, parts);
        if (op == "multu")  return parse_instruction_r_no_dst(OpcodeR::MULTU, instr, parts);
        if (op == "div")    return parse_instruction_r_no_dst(OpcodeR::DIV, instr, parts);
        if (op == "divu")   return parse_instruction_r_no_dst(OpcodeR::DIVU, instr, parts);

        if (op == "jr")     return parse_instruction_r_1_src(OpcodeR::JR, instr, parts);

        if (op == "addi")   return parse_instruction_i(OpcodeI::ADDI, instr, parts);
        if (op == "addiu")  return parse_instruction_i(OpcodeI::ADDIU, instr, parts);
        if (op == "andi")   return parse_instruction_i(OpcodeI::ANDI, instr, parts);
        if (op == "ori")    return parse_instruction_i(OpcodeI::ORI, instr, parts);
        if (op == "xori")   return parse_instruction_i(OpcodeI::XORI, instr, parts);
        if (op == "slti")   return parse_instruction_i(OpcodeI::SLTI, instr, parts);
        if (op == "sltiu")  return parse_instruction_i(OpcodeI::SLTIU, instr, parts);

        if (op == "beq")    return parse_instruction_i_branch(OpcodeI::BEQ, instr, parts);
        if (op == "bne")    return parse_instruction_i_branch(OpcodeI::BNE, instr, parts);

        if (op == "j")      return parse_instruction_j(OpcodeJ::J, instr, parts);
        if (op == "jal")    return parse_instruction_j(OpcodeJ::JAL, instr, parts);

        throw std::logic_error("Could not parse opcode " + op);
    }

    std::vector<Instruction> Parser::parse_instructions(const std::string& assembly)
    {
        const auto lines = strtools::split(assembly, '\n');

        std::vector<Instruction> instrs;
        instrs.reserve(lines.size());

        for (const auto& line : lines)
        {
            if (line.empty()) continue;
            if (std::regex_match(line, comment_regex)) continue;

            instrs.push_back(parse_instruction(line));
        }

        return instrs;
    }

    InstructionR Parser::parse_nop(const std::string& instr, const std::vector<std::string>& parts)
    {
        if (parts.size() != 1)
            throw std::logic_error("nop does not take any arguments - " + instr);

        return InstructionR
        {
            .op = OpcodeR::SLL,
            .dst = Register::zero,
            .src1 = Register::zero,
            .src2 = Register::zero,
            .shamt = 0
        };
    }

    InstructionR Parser::parse_instruction_r(OpcodeR opcode, const std::string& instr, const std::vector<std::string>& parts)
    {
        if (parts.size() != 4)
            throw std::logic_error("This R type instruction should have 4 parts - cannot parse " + instr);

        Register dst = parse_register(parts[1]);
        Register src1 = parse_register(parts[2]);
        Register src2 = parse_register(parts[3]);

        return InstructionR
        {
            .op = opcode,
            .dst = dst,
            .src1 = src1,
            .src2 = src2,
            .shamt = 0
        };
    }

    InstructionR Parser::parse_instruction_r_no_dst(OpcodeR opcode, const std::string& instr, const std::vector<std::string>& parts)
    {
        if (parts.size() != 3)
            throw std::logic_error("This R type instruction should have 3 parts - cannot parse " + instr);

        Register src1 = parse_register(parts[1]);
        Register src2 = parse_register(parts[2]);

        return InstructionR
        {
            .op = opcode,
            .dst = Register::zero,
            .src1 = src1,
            .src2 = src2,
            .shamt = 0
        };
    }

    InstructionR Parser::parse_instruction_r_1_src(OpcodeR opcode, const std::string& instr, const std::vector<std::string>& parts)
    {
        if (parts.size() != 2)
            throw std::logic_error("This R type instruction should have 2 parts - cannot parse " + instr);

        Register src = parse_register(parts[1]);

        return InstructionR
        {
            .op = opcode,
            .dst = Register::zero,
            .src1 = src,
            .src2 = Register::zero,
            .shamt = 0
        };
    }

    InstructionI Parser::parse_instruction_i(OpcodeI opcode, const std::string& instr, const std::vector<std::string>& parts)
    {
        if (parts.size() != 4)
            throw std::logic_error("This I type instruction should have 4 parts - cannot parse " + instr);

        Register dst = parse_register(parts[1]);
        Register src = parse_register(parts[2]);
        int16_t constant = parse_constant_16(parts[3]);

        return InstructionI
        {
            .op = opcode,
            .dst = dst,
            .src = src,
            .constant = constant
        };
    }

    InstructionI Parser::parse_instruction_i_branch(OpcodeI opcode, const std::string& instr, const std::vector<std::string>& parts)
    {
        if (parts.size() != 4)
            throw std::logic_error("This I type instruction should have 4 parts - cannot parse " + instr);

        Register dst = parse_register(parts[1]);
        Register src = parse_register(parts[2]);
        int16_t constant = static_cast<uint16_t>(parse_constant_32(parts[3]) >> 2);

        return InstructionI
        {
            .op = opcode,
            .dst = dst,
            .src = src,
            .constant = constant
        };
    }

    InstructionJ Parser::parse_instruction_j(OpcodeJ opcode, const std::string& instr, const std::vector<std::string>& parts)
    {
        if (parts.size() != 2)
            throw std::logic_error("This J type instruction should have 2 parts - cannot parse " + instr);

        uint32_t target = parse_constant_16(parts[1]);

        return InstructionJ
        {
            .op = opcode,
            .target = target >> 2
        };
    }

    Register Parser::parse_register(const std::string& reg)
    {
        static const std::map<std::string, Register> reg_mapping =
        {
            { "$0", Register::r0 },
            { "$1", Register::r1 },
            { "$2", Register::r2 },
            { "$3", Register::r3 },
            { "$4", Register::r4 },
            { "$5", Register::r5 },
            { "$6", Register::r6 },
            { "$7", Register::r7 },
            { "$8", Register::r8 },
            { "$9", Register::r9 },
            { "$10", Register::r10 },
            { "$11", Register::r11 },
            { "$12", Register::r12 },
            { "$13", Register::r13 },
            { "$14", Register::r14 },
            { "$15", Register::r15 },
            { "$16", Register::r16 },
            { "$17", Register::r17 },
            { "$18", Register::r18 },
            { "$19", Register::r19 },
            { "$20", Register::r20 },
            { "$21", Register::r21 },
            { "$22", Register::r22 },
            { "$23", Register::r23 },
            { "$24", Register::r24 },
            { "$25", Register::r25 },
            { "$26", Register::r26 },
            { "$27", Register::r27 },
            { "$28", Register::r28 },
            { "$29", Register::r29 },
            { "$30", Register::r30 },
            { "$31", Register::r31 },

            { "$zero", Register::zero },
            { "$at", Register::at },

            { "$v0", Register::v0 },
            { "$v1", Register::v1 },

            { "$a0", Register::a0 },
            { "$a1", Register::a1 },
            { "$a2", Register::a2 },
            { "$a3", Register::a3 },

            { "$t0", Register::t0 },
            { "$t1", Register::t1 },
            { "$t2", Register::t2 },
            { "$t3", Register::t3 },
            { "$t4", Register::t4 },
            { "$t5", Register::t5 },
            { "$t6", Register::t6 },
            { "$t7", Register::t7 },
            { "$t8", Register::t8 },
            { "$t9", Register::t9 },

            { "$s0", Register::s0 },
            { "$s1", Register::s1 },
            { "$s2", Register::s2 },
            { "$s3", Register::s3 },
            { "$s4", Register::s4 },
            { "$s5", Register::s5 },
            { "$s6", Register::s6 },
            { "$s7", Register::s7 },

            { "$k0", Register::k0 },
            { "$k1", Register::k1 },

            { "$gp", Register::gp },
            { "$sp", Register::sp },
            { "$fp", Register::fp },
            { "$ra", Register::ra },
        };

        if (reg_mapping.find(reg) == reg_mapping.end())
        {
            throw std::logic_error("Could not parse register " + reg);
        }

        return reg_mapping.at(reg);
    }

    uint32_t Parser::parse_constant_32(const std::string& value)
    {
        if (value.starts_with("0x"))
            return std::stoul(&value[2], nullptr, 16);

        if (value.starts_with("0b"))
            return std::stoul(&value[2], nullptr, 2);

        return std::stoul(value, nullptr, 10);
    }

    uint16_t Parser::parse_constant_16(const std::string& value)
    {
        return static_cast<uint16_t>(parse_constant_32(value));
    }
}
