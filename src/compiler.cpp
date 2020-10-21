#include "compiler.hpp"

#include <utils/functional.hpp>
#include <iostream>

Compiler::Compiler(mips::RegisterFile& regs, Allocator& allocator)
	: _regs(regs)
	, _allocator(allocator)
	, _jump_handler_obj(0)
{
}

Compiler::Result Compiler::compile(const std::vector<mips::Instruction>& block, const Config config)
{
	if constexpr (debug)
	{
		std::stringstream().swap(_debug_stream);
		_debug_stream << strtools::catf("Compiling basic block with -O%d\n", config.o_level);

		for (const auto instr : block)
		{
			_debug_stream << instr << "\n";
		}

		_debug_stream << "\n";
	}
	
	const auto reg_file = reinterpret_cast<uint32_t>(_regs.data());
	const auto addr = x86::Register::EDX;
	_assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(addr, reg_file);
	
	for (const auto instr : block)
	{
		compile(instr);
	}

	_assembler.instr<x86::Opcode::RET>();

	if constexpr (debug)
	{
		_debug_stream << "Generated x86 instructions\n" << _assembler.get_debug();
	}

	auto const size = _assembler.size();
	auto const buffer = _allocator.alloc(size);
	_assembler.copy(buffer);
	_linker.terminate_local(buffer);
	_allocator.commit(buffer, size);
	_assembler.reset();

	if constexpr (debug)
	{
		if (!_linker.global_map().empty())
		{
			_debug_stream << "\nGlobally resolved linker symbols\n";
			for (const auto& [label, ptr] : _linker.global_map())
			{
				_debug_stream << strtools::catf("%s: %p\n", label.c_str(), ptr);
			}
		}
	}

	return Result
	(
		reinterpret_cast<Result::func>(buffer),
		size,
		config
	);
}

std::string Compiler::get_debug() const
{
	return _debug_stream.str();
}

void Compiler::compile(const mips::Instruction instr)
{
	std::visit(functional::overload{
		[&](const auto& x) { compile(x); }
	}, instr);
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
	if (instr.dst == mips::Register::zero) return;
	
	const auto addr = x86::Register::EDX;
	const auto src1 = x86::Register::EAX;

	_assembler.instr<x86::Opcode::MOV, x86::InstrMode::MR>(src1, addr, static_cast<int32_t>(instr.src1) * 4);
	_assembler.instr<Op, x86::InstrMode::MR>(src1, addr, static_cast<int32_t>(instr.src2) * 4);
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
	if (instr.dst == mips::Register::zero) return;
	
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

void Compiler::compile_call(void (*const f)())
{
	const auto label = _label_generator.generate("func");
	_linker.label_global(label, f);
	_linker.resolve(label, [&] { return _assembler.size(); }, [&](const int32_t offset)
	{
		_assembler.call(offset);
	});
}

void Compiler::compile_jump(const uint32_t addr)
{
	if (!_jump_handler_obj)
		throw std::logic_error("Cannot compile a jump as no jump handler has been set");
	
	using namespace x86;
	_assembler.instr_imm<Opcode::MOV_I, OpcodeExt::MOV_I>(Register::ECX, _jump_handler_obj);
	_assembler.instr_imm<Opcode::MOV_I, OpcodeExt::MOV_I>(Register::EDX, addr);
	_linker.resolve(_jump_handler_label, [&] { return _assembler.size(); }, [&](const int32_t offset)
	{
		_assembler.call(offset);
	});
}
