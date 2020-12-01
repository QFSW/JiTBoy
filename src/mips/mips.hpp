#pragma once
#include <cstdint>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable IdentifierTypo

namespace mips
{
    enum class OpcodeR : uint8_t
    {
        ADD   = 0b100000,
        ADDU  = 0b100001,
        SUB   = 0b100010,
        SUBU  = 0b100011,
        AND   = 0b100100,
        OR    = 0b100101,
        NOR   = 0b100111,
        XOR   = 0b100110,
        JR    = 0b001000,
        JALR  = 0b001001,
        MULT  = 0b011000,
        MULTU = 0b011001,
        DIV   = 0b011010,
        DIVU  = 0b011011,
        MFHI  = 0b010000,
        MFLO  = 0b010010,
        MTHI  = 0b010001,
        MTLO  = 0b010011,
        SLT   = 0b101010,
        SLTU  = 0b101011,
        SLL   = 0b000000,
        SLLV  = 0b000100,
        SRA   = 0b000011,
        SRAV  = 0b000111,
        SRL   = 0b000010,
        SRLV  = 0b000110,
    };

    enum class OpcodeI : uint8_t
    {
        ADDI   = 0b001000,
        ADDIU  = 0b001001,
        ANDI   = 0b001100,
        ORI    = 0b001101,
        XORI   = 0b001110,
        SLTI   = 0b001010,
        SLTIU  = 0b001011,
        LUI    = 0b001111,
        LW     = 0b100011,
        LB     = 0b100000,
        LBU    = 0b100100,
        LH     = 0b100001,
        LHU    = 0b100101,
        SW     = 0b101011,
        SB     = 0b101000,
        SH     = 0b101001,
        BEQ    = 0b000100,
        BGTZ   = 0b000111,
        BLEZ   = 0b000110,
        BNE    = 0b000101,
        LWL    = 0b100010,
        LWR    = 0b100110,
        BGEZ   = 0b000001,
        BGEZAL = 0b010001,
        BLTZ   = 0b000000,
        BLTZAL = 0b010000
    };

    enum class OpcodeJ : uint8_t
    {
        J     = 0b000010,
        JAL   = 0b000011
    };
    
    enum class Register : uint8_t
    {
        $0 = 0,
        $1 = 1,
        $2 = 2,
        $3 = 3,
        $4 = 4,
        $5 = 5,
        $6 = 6,
        $7 = 7,
        $8 = 8,
        $9 = 9,
        $10 = 10,
        $11 = 11,
        $12 = 12,
        $13 = 13,
        $14 = 14,
        $15 = 15,
        $16 = 16,
        $17 = 17,
        $18 = 18,
        $19 = 19,
        $20 = 20,
        $21 = 21,
        $22 = 22,
        $23 = 23,
        $24 = 24,
        $25 = 25,
        $26 = 26,
        $27 = 27,
        $28 = 28,
        $29 = 29,
        $30 = 30,
        $31 = 31,

        $zero = $0,
        $at = $1,
        
        $v0 = $2,
        $v1 = $3,
        
        $a0 = $4,
        $a1 = $5,
        $a2 = $6,
        $a3 = $7,

        $t0 = $8,
        $t1 = $9,
        $t2 = $10,
        $t3 = $11,
        $t4 = $12,
        $t5 = $13,
        $t6 = $14,
        $t7 = $15,
        $t8 = $24,
        $t9 = $25,

        $s0 = $16,
        $s1 = $17,
        $s2 = $18,
        $s3 = $19,
        $s4 = $20,
        $s5 = $21,
        $s6 = $22,
        $s7 = $23,

        $k0 = $26,
        $k1 = $27,

        $gp = $28,
        $sp = $29,
        $fp = $30,
        $ra = $31
    };

    const char* opcode_to_string(OpcodeR op);
    const char* opcode_to_string(OpcodeI op);
    const char* opcode_to_string(OpcodeJ op);
    std::string reg_to_string(Register reg);
}