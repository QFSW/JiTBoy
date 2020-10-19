#include "compiler.hpp"

#include <utils/functional.hpp>
#include <iostream>

Compiler::Compiler(mips::RegisterFile& regs, Allocator& allocator)
	: _regs(regs)
	, _allocator(allocator)
{
}

Compiler::func Compiler::compile(const std::vector<mips::Instruction>& block)
{
	if constexpr (debug)
	{
		std::stringstream().swap(_debug_stream);
		_debug_stream << "Compiling basic block\n";

		for (const auto instr : block)
		{
			std::visit(functional::overload {
				[&](const auto& x) { _debug_stream << x << "\n"; }
			}, instr);
		}

		_debug_stream << "\n";
	}
	
	const auto reg_file = reinterpret_cast<uint32_t>(_regs.data());
	const auto addr = x86::Register::EDX;
	_assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(addr, reg_file);
	
	for (const auto instr : block)
	{
		std::visit(functional::overload {
			[&](const auto& x) { compile(x); }
		}, instr);
	}

	_assembler.instr<x86::Opcode::RET>();

	if constexpr (debug)
	{
		_debug_stream << "Generated x86 instructions\n" << _assembler.get_debug();
	}

	auto const buffer = _allocator.alloc(_assembler.size());
	_assembler.copy(buffer);
	_allocator.commit(buffer, _assembler.size());
	_assembler.reset();

	return reinterpret_cast<func>(buffer);
}

std::string Compiler::get_debug() const
{
	return _debug_stream.str();
}

void Compiler::compile(const mips::InstructionR instr)
{
	switch (instr.op)
	{
		case mips::OpcodeR::ADD: compile<x86::Opcode::ADD>(instr); break;
		case mips::OpcodeR::SUB: compile<x86::Opcode::SUB>(instr); break;
		case mips::OpcodeR::AND: compile<x86::Opcode::AND>(instr); break;
		case mips::OpcodeR::OR: compile<x86::Opcode::OR>(instr); break;
		case mips::OpcodeR::XOR: compile<x86::Opcode::XOR>(instr); break;
		default: throw std::logic_error(std::string("Instruction ") + mips::opcode_to_string(instr.op) + " is not supported");
	}
}

template <x86::Opcode Op>
void Compiler::compile(mips::InstructionR instr)
{
	const auto addr = x86::Register::EDX;
	const auto src1 = x86::Register::EAX;
	const auto src2 = x86::Register::ECX;

	_assembler.instr<x86::Opcode::MOV, x86::InstrMode::MR>(src1, addr, static_cast<int32_t>(instr.src1) * 4);
	_assembler.instr<x86::Opcode::MOV, x86::InstrMode::MR>(src2, addr, static_cast<int32_t>(instr.src2) * 4);
	_assembler.instr<Op>(src1, src2);
	_assembler.instr<x86::Opcode::MOV, x86::InstrMode::RM>(addr, src1, static_cast<int32_t>(instr.dst) * 4);
}

void Compiler::compile(const mips::InstructionI instr)
{
	switch (instr.op)
	{
		case mips::OpcodeI::ADDI: compile<x86::Opcode::ADD_I, x86::OpcodeExt::ADD_I>(instr); break;
		case mips::OpcodeI::ANDI: compile<x86::Opcode::AND_I, x86::OpcodeExt::AND_I>(instr); break;
		default: throw std::logic_error(std::string("Instruction ") + mips::opcode_to_string(instr.op) + " is not supported");
	}
}

template <x86::Opcode Op, x86::OpcodeExt Ext>
void Compiler::compile(mips::InstructionI instr)
{
	const auto addr = x86::Register::EDX;
	const auto src = x86::Register::EAX;

	_assembler.instr<x86::Opcode::MOV, x86::InstrMode::MR>(src, addr, static_cast<int32_t>(instr.src) * 4);
	_assembler.instr_imm<Op, Ext>(src, instr.constant);
	_assembler.instr<x86::Opcode::MOV, x86::InstrMode::RM>(addr, src, static_cast<int32_t>(instr.dst) * 4);
}

void Compiler::compile(const mips::InstructionJ instr)
{
	// TODO: implement
}
