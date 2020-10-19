#include "compiler.hpp"

#include <utils/functional.hpp>

void Compiler::compile(const std::vector<mips::Instruction>& block)
{
	for (const auto& instr : block)
	{
		std::visit(functional::overload {
			[this](const auto& x) { compile(x); }
		}, instr);
	}

	_assembler.instr<x86::Opcode::RET>();
	// TODO: allocate executable memory and commit instructions

	_assembler.reset();
}

void Compiler::compile(const mips::InstructionR& instr)
{
	// TODO: implement
}

void Compiler::compile(const mips::InstructionI& instr)
{
	// TODO: implement
}

void Compiler::compile(const mips::InstructionJ& instr)
{
	// TODO: implement
}
