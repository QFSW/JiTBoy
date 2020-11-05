#include "encoding.hpp"
#include <utils/functional.hpp>

namespace mips
{
    Instruction decode_instruction(const uint32_t binary)
    {
        uint8_t opcode = 0b111111 & (binary >> (32 - 6));

        if (opcode == 0)
            return decode_instruction_r(binary);

        if (opcode == 0b10 || opcode == 0b11)
            return decode_instruction_j(binary);

        return decode_instruction_i(binary);
    }

    InstructionR decode_instruction_r(const uint32_t binary)
    {
        uint8_t rs = 0b11111 & (binary >> (32 - (6 + 5)));
        uint8_t rt = 0b11111 & (binary >> (32 - (6 + 5 + 5)));
        uint8_t rd = 0b11111 & (binary >> (32 - (6 + 5 + 5 + 5)));
        uint8_t shamt = 0b11111 & (binary >> (32 - (6 + 5 + 5 + 5 + 5)));
        uint8_t funct = 0b111111 & binary;

        return InstructionR
        {
            .op = static_cast<OpcodeR>(funct),
            .dst = static_cast<Register>(rd),
            .src1 = static_cast<Register>(rs),
            .src2 = static_cast<Register>(rt),
            .shamt = shamt
        };
    }

    InstructionI decode_instruction_i(const uint32_t binary)
    {
        uint8_t opcode = 0b111111 & (binary >> (32 - 6));
        uint8_t rs = 0b11111 & (binary >> (32 - (6 + 5)));
        uint8_t rt = 0b11111 & (binary >> (32 - (6 + 5 + 5)));
        uint16_t constant = 0xFFFF & binary;

        if (opcode == 0b000001)
            opcode = rt;

        return InstructionI
        {
            .op = static_cast<OpcodeI>(opcode),
            .dst = static_cast<Register>(rt),
            .src = static_cast<Register>(rs),
            .constant = constant
        };
    }

    InstructionJ decode_instruction_j(const uint32_t binary)
    {
        uint8_t opcode = 0b111111 & (binary >> (32 - 6));
        uint32_t target = 0x03FFFFFF & binary;

        return InstructionJ
        {
            .op = static_cast<OpcodeJ>(opcode),
            .target = target
        };
    }

    uint32_t encode_instruction(Instruction instr)
    {
        return std::visit(functional::overload{
            [](const auto& x) { return encode_instruction(x); }
        }, instr);
    }

    uint32_t encode_instruction(InstructionR instr)
    {
        const uint8_t op = 0;
        const uint8_t funct = static_cast<uint8_t>(instr.op);
        const uint8_t rs = static_cast<uint8_t>(instr.src1);
        const uint8_t rt = static_cast<uint8_t>(instr.src2);
        const uint8_t rd = static_cast<uint8_t>(instr.dst);
        const uint8_t shamt = instr.shamt;

        uint32_t binary = 0;
        binary |= funct;
        binary |= op << (32 - 6);
        binary |= rs << (32 - (6 + 5));
        binary |= rt << (32 - (6 + 5 + 5));
        binary |= shamt << (32 - (6 + 5 + 5 + 5 + 5));
        return binary;
    }

    uint32_t encode_instruction(const InstructionI instr)
    {
        const uint16_t constant = instr.constant;
        const uint8_t op = static_cast<uint8_t>(instr.op);
        const uint8_t rs = static_cast<uint8_t>(instr.src);
        const uint8_t rt = static_cast<uint8_t>(instr.dst);

        uint32_t binary = 0;
        binary |= constant;
        binary |= op << (32 - 6);
        binary |= rs << (32 - (6 + 5));
        binary |= rt << (32 - (6 + 5 + 5));
        return binary;
    }

    uint32_t encode_instruction(const InstructionJ instr)
    {
        const uint32_t target = instr.target;
        const uint8_t op = static_cast<uint8_t>(instr.op);

        uint32_t binary = 0;
        binary |= target;
        binary |= op << (32 - 6);
        return binary;
    }
}