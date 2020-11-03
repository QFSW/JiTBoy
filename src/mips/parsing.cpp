#include "parsing.hpp"

#include <map>
#include <stdexcept>

#include <utils/strtools.hpp>

namespace mips
{
    Instruction Parser::parse_instruction(const std::string& instr)
    {
        const auto parts = strtools::split(instr, ' ');

        if (parts.empty())
            throw std::logic_error("Cannot parse empty instruction");

        const auto& op = parts[0];

        if (op == "add")  return parse_instruction_r(OpcodeR::ADD, instr, parts);
        if (op == "addu") return parse_instruction_r(OpcodeR::ADDU, instr, parts);
        if (op == "sub")  return parse_instruction_r(OpcodeR::SUB, instr, parts);
        if (op == "subu") return parse_instruction_r(OpcodeR::SUBU, instr, parts);
        if (op == "and")  return parse_instruction_r(OpcodeR::AND, instr, parts);
        if (op == "or")   return parse_instruction_r(OpcodeR::OR, instr, parts);
        if (op == "nor")  return parse_instruction_r(OpcodeR::NOR, instr, parts);
        if (op == "xor")  return parse_instruction_r(OpcodeR::XOR, instr, parts);

        if (op == "addi") return parse_instruction_i(OpcodeI::ADDI, instr, parts);

        throw std::logic_error("Could not parse opcode " + op);
    }

    std::vector<Instruction> Parser::parse_instructions(const std::string& assembly)
    {
        const auto lines = strtools::split(assembly, '\n');

        std::vector<Instruction> instrs;
        instrs.reserve(lines.size());

        for (const auto& line : lines)
        {
            if (!line.empty())
            {
                instrs.push_back(parse_instruction(line));
            }
        }

        return instrs;
    }

    InstructionR Parser::parse_instruction_r(OpcodeR opcode, const std::string& instr, const std::vector<std::string>& parts)
    {
        if (parts.size() != 4)
            throw std::logic_error("Could not parse R type instructions should have 4 parts - cannot parse " + instr);

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

    InstructionI Parser::parse_instruction_i(OpcodeI opcode, const std::string& instr, const std::vector<std::string>& parts)
    {
        if (parts.size() != 4)
            throw std::logic_error("Could not parse R type instructions should have 4 parts - cannot parse " + instr);

        Register dst = parse_register(parts[1]);
        Register src = parse_register(parts[2]);
        uint16_t constant = parse_constant(parts[3]);

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
        throw std::logic_error("Parsing J type instructions not implemented");
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

    uint16_t Parser::parse_constant(const std::string& value)
    {
        return std::atoi(value.c_str());
    }
}
