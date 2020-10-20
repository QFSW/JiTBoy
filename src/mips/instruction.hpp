#pragma once

#include <variant>

#include <mips/mips.hpp>

namespace mips
{
	struct InstructionR
	{
		OpcodeR op;
		Register dst;
		Register src1;
		Register src2;
		uint8_t shamt;
	};

	struct InstructionI
	{
		OpcodeI op;
		Register dst;
		Register src;
		uint16_t constant;
	};

	struct InstructionJ
	{
		OpcodeJ op;
		uint32_t target;
	};

	using Instruction = std::variant<InstructionR, InstructionI, InstructionJ>;

	std::ostream& operator<<(std::ostream& os, const InstructionR& instr);
	std::ostream& operator<<(std::ostream& os, const InstructionI& instr);
	std::ostream& operator<<(std::ostream& os, const InstructionJ& instr);
	std::ostream& operator<<(std::ostream& os, const Instruction& instr);
}
