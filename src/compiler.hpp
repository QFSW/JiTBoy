#pragma once

#include <config.hpp>
#include <executable_allocator.hpp>
#include <x86/assembler.hpp>
#include <mips/instruction.hpp>
#include <mips/register_file.hpp>
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

	template <typename T, void(T::* F)(int)>
	void set_jump_handler(T& obj);
	
private:
	LabelGenerator _label_generator;
	Linker _linker;
	x86::Assembler _assembler;
	mips::RegisterFile& _regs;
	Allocator& _allocator;

	std::string _jump_handler_label;
	uint32_t _jump_handler_obj;

	void compile(mips::Instruction instr);
	void compile(mips::InstructionR instr);
	void compile(mips::InstructionI instr);
	void compile(mips::InstructionJ instr);

	template <x86::Opcode Op>
	void compile(mips::InstructionR instr);

	template <x86::Opcode Op, x86::OpcodeExt Ext>
	void compile(mips::InstructionI instr);

	void compile_call(void (*f)());
	void compile_jump(uint32_t addr);

	template <typename T, void(T::* F)()>
	void compile_call(T& obj);

	template <typename T, void(T::* F)()>
	static void __fastcall proxy_call(T* obj);

	template <typename T, typename A, void(T::* F)(A)>
	static void __fastcall proxy_call_arg(T* obj, A a);

	static constexpr bool debug = config::debug;
	std::stringstream _debug_stream;
};

template <typename T, void(T::* F)(int)>
void Compiler::set_jump_handler(T& obj)
{
	_jump_handler_obj = reinterpret_cast<uint32_t>(&obj);
	_jump_handler_label = _label_generator.generate("jump_handler");
	_linker.label_global(_jump_handler_label, &proxy_call_arg<T, int, F>);
}

template <typename T, void(T::* F)()>
void Compiler::compile_call(T& obj)
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
void __fastcall Compiler::proxy_call(T* obj)
{
	return (obj->*F)();
}

template <typename T, typename A, void(T::* F)(A)>
void __fastcall Compiler::proxy_call_arg(T* obj, A a)
{
	return (obj->*F)(a);
}
