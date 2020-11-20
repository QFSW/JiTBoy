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
            case OpcodeR::JR:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.src1);
                break;
            }
            default:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.dst)
                << " " << reg_to_string(instr.src1)
                << " " << reg_to_string(instr.src2);
                break;
            }
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const InstructionI& instr)
    {
        switch (instr.op)
        {
            case OpcodeI::BEQ:
            case OpcodeI::BGTZ:
            case OpcodeI::BLEZ:
            case OpcodeI::BNE:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.dst)
                << " " << reg_to_string(instr.src)
                << " " << (instr.constant << 2);
                break;
            }
            default:
            {
                os << opcode_to_string(instr.op)
                << " " << reg_to_string(instr.dst)
                << " " << reg_to_string(instr.src)
                << " " << instr.constant;
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
