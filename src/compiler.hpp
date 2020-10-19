#pragma once

#include <x86/assembler.hpp>
#include <mips/instruction.hpp>

class Compiler
{
public:
	void compile(const std::vector<mips::Instruction>& block);
	
private:
	x86::Assembler _assembler;

	void compile(const mips::InstructionR& instr);
	void compile(const mips::InstructionI& instr);
	void compile(const mips::InstructionJ& instr);
};

