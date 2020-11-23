#include "parser.hpp"

#include <map>
#include <stdexcept>

namespace mips
{
    const std::regex Parser::comment_regex(R"(\s*#.*)");

    Instruction Parser::parse_instruction(const std::string& instr) const
    {
        static const auto parser = generate_parser<std::string>(R"((\w+).*)", "Could not determine opcode from %s");
        const auto [op] = parser.evaluate(instr);

        if (op == "nop")    return parse_nop(instr);
        if (op == "jalr")   return parse_jalr(instr);

        if (op == "add")    return parse_instruction_r(OpcodeR::ADD, instr);
        if (op == "addu")   return parse_instruction_r(OpcodeR::ADDU, instr);
        if (op == "sub")    return parse_instruction_r(OpcodeR::SUB, instr);
        if (op == "subu")   return parse_instruction_r(OpcodeR::SUBU, instr);
        if (op == "and")    return parse_instruction_r(OpcodeR::AND, instr);
        if (op == "or")     return parse_instruction_r(OpcodeR::OR, instr);
        if (op == "nor")    return parse_instruction_r(OpcodeR::NOR, instr);
        if (op == "xor")    return parse_instruction_r(OpcodeR::XOR, instr);
        if (op == "slt")    return parse_instruction_r(OpcodeR::SLT, instr);
        if (op == "sltu")   return parse_instruction_r(OpcodeR::SLTU, instr);
        if (op == "sllv")   return parse_instruction_r(OpcodeR::SLLV, instr);
        if (op == "srav")   return parse_instruction_r(OpcodeR::SRAV, instr);
        if (op == "srlv")   return parse_instruction_r(OpcodeR::SRLV, instr);

        if (op == "sll")    return parse_instruction_r_sa(OpcodeR::SLL, instr);
        if (op == "sra")    return parse_instruction_r_sa(OpcodeR::SRA, instr);
        if (op == "srl")    return parse_instruction_r_sa(OpcodeR::SRL, instr);

        if (op == "mult")   return parse_instruction_r_no_dst(OpcodeR::MULT, instr);
        if (op == "multu")  return parse_instruction_r_no_dst(OpcodeR::MULTU, instr);
        if (op == "div")    return parse_instruction_r_no_dst(OpcodeR::DIV, instr);
        if (op == "divu")   return parse_instruction_r_no_dst(OpcodeR::DIVU, instr);

        if (op == "jr")     return parse_instruction_r_1_src(OpcodeR::JR, instr);
        if (op == "mfhi")   return parse_instruction_r_1_src(OpcodeR::MFHI, instr);
        if (op == "mflo")   return parse_instruction_r_1_src(OpcodeR::MFLO, instr);
        if (op == "mthi")   return parse_instruction_r_1_src(OpcodeR::MTHI, instr);
        if (op == "mtlo")   return parse_instruction_r_1_src(OpcodeR::MTLO, instr);

        if (op == "addi")   return parse_instruction_i(OpcodeI::ADDI, instr);
        if (op == "addiu")  return parse_instruction_i(OpcodeI::ADDIU, instr);
        if (op == "andi")   return parse_instruction_i(OpcodeI::ANDI, instr);
        if (op == "ori")    return parse_instruction_i(OpcodeI::ORI, instr);
        if (op == "xori")   return parse_instruction_i(OpcodeI::XORI, instr);
        if (op == "slti")   return parse_instruction_i(OpcodeI::SLTI, instr);
        if (op == "sltiu")  return parse_instruction_i(OpcodeI::SLTIU, instr);

        if (op == "lui")    return parse_instruction_i_1_src(OpcodeI::LUI, instr);

        if (op == "beq")    return parse_instruction_i_branch(OpcodeI::BEQ, instr);
        if (op == "bne")    return parse_instruction_i_branch(OpcodeI::BNE, instr);

        if (op == "bgtz")   return parse_instruction_i_branch_no_dst(OpcodeI::BGTZ, instr);
        if (op == "blez")   return parse_instruction_i_branch_no_dst(OpcodeI::BLEZ, instr);
        if (op == "bgez")   return parse_instruction_i_branch_no_dst(OpcodeI::BGEZ, instr);
        if (op == "bltz")   return parse_instruction_i_branch_no_dst(OpcodeI::BLTZ, instr);
        if (op == "bgezal") return parse_instruction_i_branch_no_dst(OpcodeI::BGEZAL, instr);
        if (op == "bltzal") return parse_instruction_i_branch_no_dst(OpcodeI::BLTZAL, instr);

        if (op == "lw")     return parse_instruction_i_memory(OpcodeI::LW, instr);
        if (op == "lb")     return parse_instruction_i_memory(OpcodeI::LB, instr);
        if (op == "lbu")    return parse_instruction_i_memory(OpcodeI::LBU, instr);
        if (op == "lh")     return parse_instruction_i_memory(OpcodeI::LH, instr);
        if (op == "lhu")    return parse_instruction_i_memory(OpcodeI::LHU, instr);
        if (op == "sw")     return parse_instruction_i_memory(OpcodeI::SW, instr);
        if (op == "sb")     return parse_instruction_i_memory(OpcodeI::SB, instr);
        if (op == "sh")     return parse_instruction_i_memory(OpcodeI::SH, instr);
        if (op == "lwl")    return parse_instruction_i_memory(OpcodeI::LWL, instr);
        if (op == "lwr")    return parse_instruction_i_memory(OpcodeI::LWR, instr);

        if (op == "j")      return parse_instruction_j(OpcodeJ::J, instr);
        if (op == "jal")    return parse_instruction_j(OpcodeJ::JAL, instr);

        throw std::invalid_argument("Could not parse opcode " + op);
    }

    std::vector<Instruction> Parser::parse_instructions(const std::string& assembly) const
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

    InstructionR Parser::parse_nop(const std::string& instr) const
    {
        static const auto parser = generate_parser<std::string>(R"((\w+))");
        auto _ = parser.evaluate(instr);

        return InstructionR
        {
            .op = OpcodeR::SLL,
            .rd = Register::zero,
            .rs = Register::zero,
            .rt = Register::zero,
            .sa = 0
        };
    }

    InstructionR Parser::parse_jalr(const std::string& instr) const
    {
        static const auto parser1 = generate_parser<Register, Register>(R"(\w+ ??, ??)");
        static const auto parser2 = generate_parser<Register>(R"(\w+ ??)");

        Register dst;
        Register link;

        try
        {
            std::tie(link, dst) = parser1.evaluate(instr);
        }
        catch (const std::invalid_argument&)
        {
            std::tie(dst) = parser2.evaluate(instr);
            link = Register::ra;
        }

        return InstructionR
        {
            .op = OpcodeR::JALR,
            .rd = link,
            .rs = dst,
            .rt = Register::zero,
            .sa = 0
        };
    }

    InstructionR Parser::parse_instruction_r(OpcodeR opcode, const std::string& instr) const
    {
        static const auto parser = generate_parser<Register, Register, Register>(R"(\w+ ??, ??, ??)");
        const auto [dst, src1, src2] = parser.evaluate(instr);

        return InstructionR
        {
            .op = opcode,
            .rd = dst,
            .rs = src1,
            .rt = src2,
            .sa = 0
        };
    }

    InstructionR Parser::parse_instruction_r_sa(OpcodeR opcode, const std::string& instr) const
    {
        static const auto parser = generate_parser<Register, Register, uint8_t>(R"(\w+ ??, ??, ??)");
        const auto [dst, src, sa] = parser.evaluate(instr);

        return InstructionR
        {
            .op = opcode,
            .rd = dst,
            .rs = src,
            .rt = Register::zero,
            .sa = sa
        };
    }

    InstructionR Parser::parse_instruction_r_no_dst(OpcodeR opcode, const std::string& instr) const
    {
        static const auto parser = generate_parser<Register, Register>(R"(\w+ ??, ??)");
        const auto [src1, src2] = parser.evaluate(instr);

        return InstructionR
        {
            .op = opcode,
            .rd = Register::zero,
            .rs = src1,
            .rt = src2,
            .sa = 0
        };
    }

    InstructionR Parser::parse_instruction_r_1_src(OpcodeR opcode, const std::string& instr) const
    {
        static const auto parser = generate_parser<Register>(R"(\w+ ??)");
        const auto [src] = parser.evaluate(instr);

        return InstructionR
        {
            .op = opcode,
            .rd = Register::zero,
            .rs = src,
            .rt = Register::zero,
            .sa = 0
        };
    }

    InstructionI Parser::parse_instruction_i(OpcodeI opcode, const std::string& instr) const
    {
        static const auto parser = generate_parser<Register, Register, int16_t>(R"(\w+ ??, ??, ??)");
        const auto [dst, src, constant] = parser.evaluate(instr);

        return InstructionI
        {
            .op = opcode,
            .rt = dst,
            .rs = src,
            .constant = constant
        };
    }

    InstructionI Parser::parse_instruction_i_1_src(OpcodeI opcode, const std::string& instr) const
    {
        static const auto parser = generate_parser<Register, int16_t>(R"(\w+ ??, ??)");
        const auto [dst, constant] = parser.evaluate(instr);

        return InstructionI
        {
            .op = opcode,
            .rt = dst,
            .rs = Register::zero,
            .constant = constant
        };
    }

    InstructionI Parser::parse_instruction_i_branch(OpcodeI opcode, const std::string& instr) const
    {
        static const auto parser = generate_parser<Register, Register, int32_t>(R"(\w+ ??, ??, ??)");
        const auto [dst, src, constant] = parser.evaluate(instr);

        return InstructionI
        {
            .op = opcode,
            .rt = dst,
            .rs = src,
            .constant = static_cast<int16_t>(constant >> 2)
        };
    }

    InstructionI Parser::parse_instruction_i_branch_no_dst(OpcodeI opcode, const std::string& instr) const
    {
        static const auto parser = generate_parser<Register, int32_t>(R"(\w+ ??, ??)");
        const auto [src, constant] = parser.evaluate(instr);

        return InstructionI
        {
            .op = opcode,
            .rt = Register::zero,
            .rs = src,
            .constant = static_cast<int16_t>(constant >> 2)
        };
    }

    InstructionI Parser::parse_instruction_i_memory(OpcodeI opcode, const std::string& instr) const
    {
        static const auto parser = generate_parser<Register, int16_t, Register>(R"(\w+ ??, ??\(??\))");
        const auto [rt, offset, base] = parser.evaluate(instr);

        return InstructionI
        {
            .op = opcode,
            .rt = rt,
            .rs = base,
            .constant = offset
        };
    }

    InstructionJ Parser::parse_instruction_j(OpcodeJ opcode, const std::string& instr) const
    {
        static const auto parser = generate_parser<uint32_t>(R"(\w+ ??)");
        const auto [target] = parser.evaluate(instr);

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
        try
        {
            if (value.starts_with("0x"))
                return std::stoul(&value[2], nullptr, 16);

            if (value.starts_with("0b"))
                return std::stoul(&value[2], nullptr, 2);

            return std::stoul(value, nullptr, 10);
        }
        catch (const std::invalid_argument&)
        {
            throw std::invalid_argument("Could not parse constant " + value);
        }
    }

    uint16_t Parser::parse_constant_16(const std::string& value)
    {
        return static_cast<uint16_t>(parse_constant_32(value));
    }

    uint8_t Parser::parse_constant_8(const std::string& value)
    {
        return static_cast<uint8_t>(parse_constant_32(value));
    }

    template<> Register Parser::Inner::parse(const std::string& raw) { return parse_register(raw); }
    template<> uint32_t Parser::Inner::parse(const std::string& raw) { return parse_constant_32(raw); }
    template<> uint16_t Parser::Inner::parse(const std::string& raw) { return parse_constant_16(raw); }
    template<> uint8_t Parser::Inner::parse(const std::string& raw) { return parse_constant_8(raw); }
    template<> int32_t Parser::Inner::parse(const std::string& raw) { return parse_constant_32(raw); }
    template<> int16_t Parser::Inner::parse(const std::string& raw) { return parse_constant_16(raw); }
    template<> int8_t Parser::Inner::parse(const std::string& raw) { return parse_constant_8(raw); }
    template<> std::string Parser::Inner::parse(const std::string& raw) { return raw; }
}
