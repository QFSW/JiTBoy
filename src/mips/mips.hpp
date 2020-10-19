#pragma once
#include <cstdint>

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
		ADDI  = 0b001000,
		ADDIU = 0b001001,
		ANDI  = 0b001100,
		ORI   = 0b001101,
		XORI  = 0b001110,
		SLTI  = 0b001010,
		SLTIU = 0b001011,
		LUI   = 0b001111,
		LW    = 0b100011,
		LB    = 0b100000,
		LBU   = 0b100100,
		LH    = 0b100001,
		LHU   = 0b100101,
		SW    = 0b101011,
		SB    = 0b101000,
		SH    = 0b101001,
		BEQ   = 0b000100,
		BGTZ  = 0b000111,
		BLEZ  = 0b000110,
		BNE   = 0b000101,
		LWL   = 0b100010,
		LWR   = 0b100110,
	};
	
	enum class Register : uint8_t
	{
		r0 = 0,
		r1 = 1,
		r2 = 2,
		r3 = 3,
		r4 = 4,
		r5 = 5,
		r6 = 6,
		r7 = 7,
		r8 = 8,
		r9 = 9,
		r10 = 10,
		r11 = 11,
		r12 = 12,
		r13 = 13,
		r14 = 14,
		r15 = 15,
		r16 = 16,
		r17 = 17,
		r18 = 18,
		r19 = 19,
		r20 = 20,
		r21 = 21,
		r22 = 22,
		r23 = 23,
		r24 = 24,
		r25 = 25,
		r26 = 26,
		r27 = 27,
		r28 = 28,
		r29 = 29,
		r30 = 30,
		r31 = 31,

		zero = r0,
		at = r1,
		
		v0 = r2,
		v1 = r3,
		
		a0 = r4,
		a1 = r5,
		a2 = r6,
		a3 = r7,

		t0 = r8,
		t1 = r9,
		t2 = r10,
		t3 = r11,
		t4 = r12,
		t5 = r13,
		t6 = r14,
		t7 = r15,
		t8 = r24,
		t9 = r25,

		s0 = r16,
		s1 = r17,
		s2 = r18,
		s3 = r19,
		s4 = r20,
		s5 = r21,
		s6 = r22,
		s7 = r23,

		k0 = r26,
		k1 = r27,

		gp = r28,
		sp = r29,
		fp = r30,
		ra = r31
	};
}