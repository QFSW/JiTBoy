#pragma once
#include <cstdint>

// ReSharper disable CppInconsistentNaming
// ReSharper disable IdentifierTypo

namespace mips
{
	enum class OpcodeR : uint8_t
	{
		ADD = 0b100000
	};

	enum class OpcodeI : uint8_t
	{
		ADD = 0b001000
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