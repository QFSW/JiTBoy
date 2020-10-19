#pragma once

#include <config.hpp>
#include <executable_allocator.hpp>
#include <x86/assembler.hpp>
#include <mips/instruction.hpp>
#include <mips/register_file.hpp>
#include <iostream>
#include <linker.hpp>
#include "label_generator.hpp"

class Compiler
{	
public:
	using Allocator = ExecutableAllocator<4096>;
	typedef void(*func)();
	
	Compiler(mips::RegisterFile& regs, Allocator& allocator);
	
	func compile(const std::vector<mips::Instruction>& block);
	[[nodiscard]] std::string get_debug() const;
	
private:
	LabelGenerator _label_generator;
	Linker _linker;
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

	void compile_call(void (*f)());

	template <typename T, void(T::* F)()>
	void compile_call(T& obj)
	{
		using namespace x86;
		const auto addr = reinterpret_cast<uint32_t>(&obj);
		_assembler.instr_imm<Opcode::MOV_I, OpcodeExt::MOV_I>(Register::ECX, addr);
		
		const auto label = _label_generator.generate("member_func");
		_linker.label_global(label, &proxy_call<T, F>);
		_linker.resolve(label, [&] { return _assembler.size(); }, [&](const int32_t offset)
		{
			_assembler.call(offset);
		});
	}

	template <typename T, void(T::* F)()>
	static void proxy_call(T* obj)
	{
		return (obj->*F)();
	}

	static constexpr bool debug = config::debug;
	std::stringstream _debug_stream;
};

