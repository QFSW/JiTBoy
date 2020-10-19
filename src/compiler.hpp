#pragma once

#include <executable_allocator.hpp>
#include <x86/assembler.hpp>
#include <mips/instruction.hpp>
#include <mips/register_file.hpp>

class Compiler
{
	using Allocator = ExecutableAllocator<4096>;
	
public:
	Compiler(mips::RegisterFile& regs, Allocator& allocator);
	void compile(const std::vector<mips::Instruction>& block);
	
private:
	x86::Assembler _assembler;
	mips::RegisterFile& _regs;
	Allocator& _allocator;
	
	void compile(mips::InstructionR instr);
	void compile(mips::InstructionI instr);
	void compile(mips::InstructionJ instr);

	template <x86::Opcode Op>
	void compile(mips::InstructionR instr);
};

