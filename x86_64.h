#pragma once
#include <cstdint>

// ReSharper disable CppInconsistentNaming

enum Opcode16 : uint16_t
{
	
};

enum Opcode8 : uint8_t
{
	ADD = 0x00,
	ADC = 0x10,
	SUB = 0x29,
	MOV = 0x88,
	INC = 0xFE,
	DEC = 0xFE,
	NEG = 0xF6,
	
	AND = 0x20,
	OR = 0x08,
	XOR = 0x30,
	NOT = 0xF6,

	ADD_I_EAX = 0x50,
	
	MOV_I_8 = 0xB0,
	MOV_I_16 = 0xB8,
	MOV_I_32 = 0xB8,
	
	RET = 0xC3
};

enum class UnaryOpcodeExt : uint8_t
{
	INC = 0b00,
	DEC = 0b01,
	NEG = 0b11,
	NOT = 0b10
};

enum class RegisterSize : uint8_t
{
	Reg8 = 8,
	Reg16 = 16,
	Reg32 = 32
};

enum class OpcodePrefix : uint8_t
{
	Size16 = 0x66,
	Lock = 0xF0
};

enum class RegisterMode : uint8_t
{
	MemoryDisp0 = 0b00 << 6,
	MemoryDisp1 = 0b01 << 6,
	MemoryDisp4 = 0b10 << 6,
	Register = 0b11 << 6
};

enum Register32 : uint8_t
{
	EAX = 0b000,
	ECX = 0b001,
	EDX = 0b010,
	EBX = 0b011,
	ESP = 0b100,
	EBP = 0b101,
	ESI = 0b110,
	EDI = 0b111,

	R8D = 0b1000,
	R9D = 0b1001,
	R10D = 0b1010,
	R11D = 0b1011,
	R12D = 0b1100,
	R13D = 0b1101,
	R14D = 0b1110,
	R15D = 0b1111
};

enum class Register : uint8_t
{
	RAX = 0b000,
	RCX = 0b001,
	RDX = 0b010,
	RBX = 0b011,
	RSP = 0b100,
	RBP = 0b101,
	RSI = 0b110,
	RDI = 0b111,

	R8 = 0b1000,
	R9 = 0b1001,
	R10 = 0b1010,
	R11 = 0b1011,
	R12 = 0b1100,
	R13 = 0b1101,
	R14 = 0b1110,
	R15 = 0b1111,
	
	EAX = 0b000,
	ECX = 0b001,
	EDX = 0b010,
	EBX = 0b011,
	ESP = 0b100,
	EBP = 0b101,
	ESI = 0b110,
	EDI = 0b111,

	R8D = 0b1000,
	R9D = 0b1001,
	R10D = 0b1010,
	R11D = 0b1011,
	R12D = 0b1100,
	R13D = 0b1101,
	R14D = 0b1110,
	R15D = 0b1111,
	
	AX = 0b000,
	CX = 0b001,
	DX = 0b010,
	BX = 0b011,
	SP = 0b100,
	BP = 0b101,
	SI = 0b110,
	DI = 0b111,

	R8W = 0b1000,
	R9W = 0b1001,
	R10W = 0b1010,
	R11W = 0b1011,
	R12W = 0b1100,
	R13W = 0b1101,
	R14W = 0b1110,
	R15W = 0b1111,
	
	AL = 0b000,
	CL = 0b001,
	DL = 0b010,
	BL = 0b011,
	AH = 0b100,
	CH = 0b101,
	DH = 0b110,
	BH = 0b111,

	R8L = 0b1000,
	R9L = 0b1001,
	R10L = 0b1010,
	R11L = 0b1011,
	R12L = 0b1100,
	R13L = 0b1101,
	R14L = 0b1110,
	R15L = 0b1111
};