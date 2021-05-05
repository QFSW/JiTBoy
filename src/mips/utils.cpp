#include "utils.hpp"

#include <utils/functional.hpp>

namespace mips::utils
{
    bool is_branch_instr(const Instruction& instr)
    {
        return std::visit(functional::overload{
            [](const InstructionR& x)
            {
                switch (x.op)
                {
                    case OpcodeR::JR:
                    case OpcodeR::JALR: return true;
                    default: return false;
                }
            },
            [](const InstructionI& x)
            {
                switch (x.op)
                {
                    case OpcodeI::BEQ:
                    case OpcodeI::BNE:
                    case OpcodeI::BGTZ:
                    case OpcodeI::BLEZ:
                    case OpcodeI::BGEZ:
                    case OpcodeI::BGEZAL:
                    case OpcodeI::BLTZ:
                    case OpcodeI::BLTZAL: return true;
                    default: return false;
                }
            },
            [](const InstructionJ&) { return true; }
        }, instr);
    }

    bool is_nop(const Instruction& instr)
    {
        return writes_reg(instr, Register::$zero);
    }

    bool writes_reg(const Instruction& instr, Register reg)
    {
        return std::visit(functional::overload{
            [reg](const InstructionR& x)
            {
                switch (x.op)
                {
                    case OpcodeR::ADD:
                    case OpcodeR::ADDU:
                    case OpcodeR::SUB:
                    case OpcodeR::SUBU:
                    case OpcodeR::AND:
                    case OpcodeR::OR:
                    case OpcodeR::NOR:
                    case OpcodeR::XOR:
                    case OpcodeR::JALR:
                    case OpcodeR::MFHI:
                    case OpcodeR::MFLO:
                    case OpcodeR::SLT:
                    case OpcodeR::SLTU:
                    case OpcodeR::SLL:
                    case OpcodeR::SLLV:
                    case OpcodeR::SRA:
                    case OpcodeR::SRAV:
                    case OpcodeR::SRL:
                    case OpcodeR::SRLV:
                        return reg == x.rd;

                    case OpcodeR::MULT:
                    case OpcodeR::MULTU:
                    case OpcodeR::DIV:
                    case OpcodeR::DIVU:
                    case OpcodeR::MTHI:
                    case OpcodeR::MTLO:
                    case OpcodeR::JR:
                    default:
                        return false;
                }
            },
            [reg](const InstructionI& x)
            {
                switch (x.op)
                {
                    case OpcodeI::ADDI:
                    case OpcodeI::ADDIU:
                    case OpcodeI::ANDI:
                    case OpcodeI::ORI:
                    case OpcodeI::XORI:
                    case OpcodeI::SLTI:
                    case OpcodeI::SLTIU:
                    case OpcodeI::LUI:
                    case OpcodeI::LW:
                    case OpcodeI::LB:
                    case OpcodeI::LBU:
                    case OpcodeI::LH:
                    case OpcodeI::LHU:
                    case OpcodeI::SW:
                    case OpcodeI::SB:
                    case OpcodeI::SH:
                    case OpcodeI::LWL:
                    case OpcodeI::LWR:
                        return reg == x.rt;

                    case OpcodeI::BGEZAL:
                    case OpcodeI::BLTZAL:
                        return reg == Register::$ra;

                    case OpcodeI::BEQ:
                    case OpcodeI::BGTZ:
                    case OpcodeI::BLEZ:
                    case OpcodeI::BNE:
                    case OpcodeI::BGEZ:
                    case OpcodeI::BLTZ:
                    default:
                        return false;
                }
            },
            [reg](const InstructionJ& x)
            {
                switch (x.op)
                {
                    case OpcodeJ::JAL:
                        return reg == Register::$ra;

                    default:
                        return false;
                }
            }
        }, instr);
    }

    bool reads_reg(const Instruction& instr, const Register reg)
    {
        return std::visit(functional::overload{
            [reg](const InstructionR& x)
            {
                switch (x.op)
                {
                    case OpcodeR::ADD:
                    case OpcodeR::ADDU:
                    case OpcodeR::SUB:
                    case OpcodeR::SUBU:
                    case OpcodeR::AND:
                    case OpcodeR::OR:
                    case OpcodeR::NOR:
                    case OpcodeR::XOR:
                    case OpcodeR::MULT:
                    case OpcodeR::MULTU:
                    case OpcodeR::DIV:
                    case OpcodeR::DIVU:
                    case OpcodeR::SLT:
                    case OpcodeR::SLTU:
                    case OpcodeR::SLLV:
                    case OpcodeR::SRAV:
                    case OpcodeR::SRLV:
                        return reg == x.rt || reg == x.rs;

                    case OpcodeR::SLL:
                    case OpcodeR::SRA:
                    case OpcodeR::SRL:
                        return reg == x.rt;

                    case OpcodeR::MTHI:
                    case OpcodeR::MTLO:
                    case OpcodeR::JR:
                    case OpcodeR::JALR:
                        return reg == x.rs;

                    case OpcodeR::MFHI:
                    case OpcodeR::MFLO:
                    default:
                        return false;
                }
            },
            [reg](const InstructionI& x)
            {
                switch (x.op)
                {
                    case OpcodeI::ADDI:
                    case OpcodeI::ADDIU:
                    case OpcodeI::ANDI:
                    case OpcodeI::ORI:
                    case OpcodeI::XORI:
                    case OpcodeI::SLTI:
                    case OpcodeI::SLTIU:
                    case OpcodeI::LW:
                    case OpcodeI::LB:
                    case OpcodeI::LBU:
                    case OpcodeI::LH:
                    case OpcodeI::LHU:
                    case OpcodeI::SW:
                    case OpcodeI::SB:
                    case OpcodeI::SH:
                    case OpcodeI::LWL:
                    case OpcodeI::LWR:
                    case OpcodeI::BGTZ:
                    case OpcodeI::BLEZ:
                    case OpcodeI::BGEZ:
                    case OpcodeI::BGEZAL:
                    case OpcodeI::BLTZ:
                    case OpcodeI::BLTZAL:
                        return reg == x.rs;

                    case OpcodeI::BEQ:
                    case OpcodeI::BNE:
                        return reg == x.rs || reg == x.rt;

                    case OpcodeI::LUI:
                    default:
                        return false;
                }
            },
            [](const InstructionJ&) { return false; }
        }, instr);
    }
}