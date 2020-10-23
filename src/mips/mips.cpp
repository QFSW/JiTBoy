#include "mips.hpp"
#include <utils/strtools.hpp>

// ReSharper disable StringLiteralTypo

namespace mips
{
    const char* opcode_to_string(const OpcodeR op)
    {
        switch (op)
        {
            case OpcodeR::ADD:   return "add";
            case OpcodeR::ADDU:  return "addu";
            case OpcodeR::SUB:   return "sub";
            case OpcodeR::SUBU:  return "subu";
            case OpcodeR::AND:   return "and";
            case OpcodeR::OR:    return "or";
            case OpcodeR::NOR:   return "nor";
            case OpcodeR::XOR:   return "xor";
            case OpcodeR::JR:    return "jr";
            case OpcodeR::JALR:  return "jalr";
            case OpcodeR::MULT:  return "mult";
            case OpcodeR::MULTU: return "multu";
            case OpcodeR::DIV:   return "div";
            case OpcodeR::DIVU:  return "divu";
            case OpcodeR::MFHI:  return "mfhi";
            case OpcodeR::MFLO:  return "mflo";
            case OpcodeR::MTHI:  return "mthi";
            case OpcodeR::MTLO:  return "mtlo";
            case OpcodeR::SLT:   return "slt";
            case OpcodeR::SLTU:  return "sltu";
            case OpcodeR::SLL:   return "sll";
            case OpcodeR::SLLV:  return "sllv";
            case OpcodeR::SRA:   return "sra";
            case OpcodeR::SRAV:  return "srav";
            case OpcodeR::SRL:   return "srl";
            case OpcodeR::SRLV:  return "srvl";
            default:             return "???";
        }
    }
    
    const char* opcode_to_string(const OpcodeI op)
    {
        switch (op)
        {
            case OpcodeI::ADDI:  return "addi";
            case OpcodeI::ADDIU: return "addiu";
            case OpcodeI::ANDI:  return "andi";
            case OpcodeI::ORI:   return "ori";
            case OpcodeI::XORI:  return "xori";
            case OpcodeI::SLTI:  return "slti";
            case OpcodeI::SLTIU: return "sltiu";
            case OpcodeI::LUI:   return "lui";
            case OpcodeI::LW:    return "lw";
            case OpcodeI::LB:    return "lb";
            case OpcodeI::LBU:   return "lbu";
            case OpcodeI::LH:    return "lh";
            case OpcodeI::LHU:   return "lhu";
            case OpcodeI::SW:    return "sw";
            case OpcodeI::SB:    return "sb";
            case OpcodeI::SH:    return "sh";
            case OpcodeI::BEQ:   return "beq";
            case OpcodeI::BGTZ:  return "bgtz";
            case OpcodeI::BLEZ:  return "blez";
            case OpcodeI::BNE:   return "bne";
            case OpcodeI::LWL:   return "lwl";
            case OpcodeI::LWR:   return "lwr";
            default:             return "???";
        }
    }

    const char* opcode_to_string(const OpcodeJ op)
    {
        switch (op)
        {
            case OpcodeJ::J:     return "j";
            case OpcodeJ::JAL:   return "jal";
            default:             return "???";
        }
    }
    
    std::string reg_to_string(const Register reg)
    {
        return strtools::catf("$%d", reg);
    }
}