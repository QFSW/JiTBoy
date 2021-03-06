#include "instruction.hpp"

#include <ostream>

#include <utils/strtools.hpp>
#include <utils/functional.hpp>

namespace mips
{
    std::ostream& operator<<(std::ostream& os, const InstructionR& instr)
    {
        switch (instr.op)
        {
            case OpcodeR::JALR:
            {
                if (instr.rd == Register::$ra)
                {
                    os << opcode_to_string(instr.op)
                    << " " << reg_to_string(instr.rs);
                }
                else
                {
                    os << opcode_to_string(instr.op)
                    << " " << reg_to_string(instr.rd)
                    << ", " << reg_to_string(instr.rs);
                }
                break;
            }
            case OpcodeR::JR:
            case OpcodeR::MFHI:
            case OpcodeR::MFLO:
            case OpcodeR::MTHI:
            case OpcodeR::MTLO:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.rd);
                break;
            }
            case OpcodeR::MULT:
            case OpcodeR::MULTU:
            case OpcodeR::DIV:
            case OpcodeR::DIVU:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.rs)
                << ", " << reg_to_string(instr.rt);
                break;
            }
            case OpcodeR::SLL:
            case OpcodeR::SRA:
            case OpcodeR::SRL:
            {
                if (
                    instr.op == OpcodeR::SLL
                    && instr.rd == Register::$zero
                    && instr.rt == Register::$zero
                    && instr.rs == Register::$zero
                    && instr.sa == 0
                )
                {
                    os << "nop";
                }
                else
                {
                    os << opcode_to_string(instr.op)
                    << " " << reg_to_string(instr.rd)
                    << ", " << reg_to_string(instr.rt)
                    << ", " << static_cast<uint32_t>(instr.sa);
                }
                break;
            }
            case OpcodeR::SLLV:
            case OpcodeR::SRAV:
            case OpcodeR::SRLV:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.rd)
                << ", " << reg_to_string(instr.rt)
                << ", " << reg_to_string(instr.rs);
                break;
            }
            default:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.rd)
                << ", " << reg_to_string(instr.rs)
                << ", " << reg_to_string(instr.rt);
                break;
            }
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const InstructionI& instr)
    {
        switch (instr.op)
        {
            case OpcodeI::LUI:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.rt)
                << ", " << instr.constant;
                break;
            }
            case OpcodeI::BEQ:
            case OpcodeI::BNE:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.rt)
                << ", " << reg_to_string(instr.rs)
                << ", " << (instr.constant << 2);
                break;
            }
            case OpcodeI::BGEZ:
            case OpcodeI::BGTZ:
            case OpcodeI::BLTZ:
            case OpcodeI::BLEZ:
            case OpcodeI::BGEZAL:
            case OpcodeI::BLTZAL:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.rs)
                << ", " << (instr.constant << 2);
                break;
            }
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
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.rt)
                << ", " << instr.constant
                << "(" << reg_to_string(instr.rs) << ")";
                break;
            }
            default:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.rt)
                << ", " << reg_to_string(instr.rs)
                << ", " << instr.constant;
                break;
            }
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const InstructionJ& instr)
    {
        os << opcode_to_string(instr.op)
        << strtools::catf(" 0x%x", instr.target << 2);

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Instruction& instr)
    {
        std::visit(functional::overload{
            [&](const auto& x) { os << x; }
        }, instr);

        return os;
    }
}
