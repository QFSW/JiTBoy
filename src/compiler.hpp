#pragma once

#include <executable_allocator.hpp>
#include <x86/assembler.hpp>
#include <mips/instruction.hpp>
#include <mips/register_file.hpp>

class Compiler
{	
public:
	using Allocator = ExecutableAllocator<4096>;
	typedef void(*func)();
	
	Compiler(mips::RegisterFile& regs, Allocator& allocator);
	
	func compile(const std::vector<mips::Instruction>& block);
	[[nodiscard]] std::string get_debug() const;
	
private:
	x86::Assembler _assembler;
	mips::RegisterFile& _regs;
	Allocator& _allocator;
	
	void compile(mips::InstructionR instr);
	void compile(mips::InstructionI instr);
	void compile(mips::InstructionJ instr);

	template <x86::Opcode Op>
	void compile(mips::InstructionR instr);

	template <x86::Opcode Op, x86::OpcodeExt Ext>
	void compile(mips::InstructionI instr);

	static constexpr bool debug = true;
	std::stringstream _debug_stream;
};

