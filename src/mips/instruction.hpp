#pragma once

#include <variant>

#include <mips/mips.hpp>

namespace mips
{
	struct InstructionR
	{
		Opcode op;
		Register dst;
		Register src1;
		Register src2;
		uint8_t shamt;
	};

	struct InstructionI
	{
		Opcode op;
		Register dst;
		Register src;
		uint16_t constant;
	};

	struct InstructionJ
	{
		
	};

	using Instruction = std::variant<InstructionR, InstructionI, InstructionJ>;
}
