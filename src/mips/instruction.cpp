#include "instruction.hpp"

#include <ostream>
#include <utils/functional.hpp>

namespace mips
{
	std::ostream& operator<<(std::ostream& os, const InstructionR& instr)
	{
		os << opcode_to_string(instr.op)
		<< " " << reg_to_string(instr.dst)
		<< " " << reg_to_string(instr.src1)
		<< " " << reg_to_string(instr.src2);

		return os;
	}

	std::ostream& operator<<(std::ostream& os, const InstructionI& instr)
	{
		os << opcode_to_string(instr.op)
		<< " " << reg_to_string(instr.dst)
		<< " " << reg_to_string(instr.src)
		<< " " << instr.constant;

		return os;
	}

	std::ostream& operator<<(std::ostream& os, const InstructionJ& instr)
	{
		os << opcode_to_string(instr.op)
		<< " " << instr.target;

		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Instruction& instr)
	{
		std::visit(functional::overload{
			[&](const auto& x) { os << x; }
		}, instr);

		return os;
	}
}
