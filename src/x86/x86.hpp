#pragma once
#include <cstdint>

// ReSharper disable CppInconsistentNaming
// ReSharper disable IdentifierTypo

namespace x86
{
    enum class Opcode : uint8_t
    {
        ADD = 0x00,
        ADC = 0x10,
        SUB = 0x29,
        MOV = 0x88,
        INC = 0xFE,
        DEC = 0xFE,
        NEG = 0xF6,
        CMP = 0x38,
        MUL = 0xF6,
        DIV = 0xF6,

        AND = 0x20,
        OR  = 0x08,
        XOR = 0x30,
        NOT = 0xF6,

        PUSH = 0xFF,
        POP  = 0x8F,

        NOP = 0x90,

        ADD_I = 0x80,
        ADC_I = 0x80,
        SUB_I = 0x80,
        MOV_I = 0xC6,
        AND_I = 0x80,
        OR_I  = 0x80,
        XOR_I = 0x80,
        CMP_I = 0x80,
        SAL_I = 0xC0,
        SAR_I = 0xC0,
        SHL_I = 0xC0,
        SHR_I = 0xC0,

        ADD_I_EAX = 0x50,

        RET   = 0xC3,
        ENTER = 0xC8,
        LEAVE = 0xC9,

        JMP_8  = 0xEB,
        JMP_32 = 0xE9,
        CALL   = 0xE8,

        CMOVcc = 0x40,
        SETcc  = 0x90,
        Jcc_8  = 0x70,
        Jcc_32 = 0x80
    };

    enum class OpcodeExt : uint8_t
    {
        INC  = 0,
        DEC  = 1,
        NEG  = 3,
        NOT  = 2,
        PUSH = 6,
        POP  = 0,
        MUL  = 4,
        DIV  = 6,

        ADD_I = 0,
        ADC_I = 2,
        SUB_I = 5,
        MOV_I = 0,
        AND_I = 4,
        OR_I  = 1,
        XOR_I = 6,
        CMP_I = 7,
        SAL_I = 4,
        SAR_I = 7,
        SHL_I = 4,
        SHR_I = 5
    };

    enum class OpcodePrefix : uint8_t
    {
        Size16 = 0x66,
        Size64 = 0x48,
        Lock   = 0xF0,
        CMOVcc = 0x0F,
        SETcc  = 0x0F,
        Jcc_32 = 0x0F
    };

    enum class RegisterSize : uint8_t
    {
        Reg8  = 8,
        Reg16 = 16,
        Reg32 = 32,
        Reg64 = 64
    };

    enum class RegisterMode : uint8_t
    {
        MemoryDisp0 = 0b00 << 6,
        MemoryDisp1 = 0b01 << 6,
        MemoryDisp4 = 0b10 << 6,
        Register    = 0b11 << 6
    };

    enum class CondCode : uint8_t
    {
        A   = 0x07,
        AE  = 0x03,
        B   = 0x02,
        BE  = 0x06,
        C   = 0x02,
        E   = 0x04,
        G   = 0x0F,
        GE  = 0x0D,
        L   = 0x0C,
        LE  = 0x0E,
        NA  = 0x06,
        NAE = 0x02,
        NB  = 0x03,
        NBE = 0x07,
        NC  = 0x03,
        NE  = 0x05,
        NG  = 0x0E,
        NGE = 0x0C,
        NL  = 0x0D,
        NLE = 0x0F,
        NO  = 0x01,
        NP  = 0x0B,
        NS  = 0x09,
        NZ  = 0x05,
        O   = 0x00,
        P   = 0x0A,
        PE  = 0x0A,
        PO  = 0x0B,
        S   = 0x08,
        Z   = 0x04
    };

    enum class Register : uint8_t
    {
        RAX  = 0b000,
        RCX  = 0b001,
        RDX  = 0b010,
        RBX  = 0b011,
        RSP  = 0b100,
        RBP  = 0b101,
        RSI  = 0b110,
        RDI  = 0b111,

        R8   = 0b1000,
        R9   = 0b1001,
        R10  = 0b1010,
        R11  = 0b1011,
        R12  = 0b1100,
        R13  = 0b1101,
        R14  = 0b1110,
        R15  = 0b1111,

        EAX  = 0b000,
        ECX  = 0b001,
        EDX  = 0b010,
        EBX  = 0b011,
        ESP  = 0b100,
        EBP  = 0b101,
        ESI  = 0b110,
        EDI  = 0b111,

        R8D  = 0b1000,
        R9D  = 0b1001,
        R10D = 0b1010,
        R11D = 0b1011,
        R12D = 0b1100,
        R13D = 0b1101,
        R14D = 0b1110,
        R15D = 0b1111,

        AX   = 0b000,
        CX   = 0b001,
        DX   = 0b010,
        BX   = 0b011,
        SP   = 0b100,
        BP   = 0b101,
        SI   = 0b110,
        DI   = 0b111,

        R8W  = 0b1000,
        R9W  = 0b1001,
        R10W = 0b1010,
        R11W = 0b1011,
        R12W = 0b1100,
        R13W = 0b1101,
        R14W = 0b1110,
        R15W = 0b1111,

        AL   = 0b000,
        CL   = 0b001,
        DL   = 0b010,
        BL   = 0b011,
        AH   = 0b100,
        CH   = 0b101,
        DH   = 0b110,
        BH   = 0b111,

        R8L  = 0b1000,
        R9L  = 0b1001,
        R10L = 0b1010,
        R11L = 0b1011,
        R12L = 0b1100,
        R13L = 0b1101,
        R14L = 0b1110,
        R15L = 0b1111
    };

    const char* opcode_to_string(Opcode op);
    const char* opcode_imm_to_string(Opcode op, OpcodeExt ext);
    const char* reg_to_string(Register reg, RegisterSize size);
    const char* cond_to_string(CondCode cond);
}