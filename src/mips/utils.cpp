#include "utils.hpp"

#include <utils/functional.hpp>

namespace mips::utils
{
    constexpr bool is_branch_instr(const Instruction& instr)
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
                    case OpcodeI::BGTZ:
                    case OpcodeI::BLEZ:
                    case OpcodeI::BGEZ:
                    case OpcodeI::BGEZAL:
                    case OpcodeI::BLTZ:
                    case OpcodeI::BLTZAL: return true;
                    default: return false;
                }
            },
            [](const InstructionJ& x) { return true; }
        }, instr);
    }
}