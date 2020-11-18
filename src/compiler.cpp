#include "compiler.hpp"

#include <mips/utils.hpp>
#include <utils/functional.hpp>

Compiler::Compiler(mips::RegisterFile& regs, Allocator& allocator)
    : _regs(regs)
    , _allocator(allocator)
{ }

CompiledBlock Compiler::compile(const SourceBlock& block, const CompilerConfig config)
{
    if constexpr (debug)
    {
        std::stringstream().swap(_debug_stream);
        _debug_stream << strtools::catf("Compiling block 0x%x with -O%d\n", block.addr, config.o_level);

        for (const auto& instr : block.code)
        {
            _debug_stream << instr << "\n";
        }

        _debug_stream << "\n";
    }
    
    const auto reg_file = reinterpret_cast<uint32_t>(_regs.data());
    _assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(addr_reg, reg_file);

    for (uint32_t i = 0; i < block.code.size(); i++)
    {
        compile(block.code[i], block.addr + i * 4);
    }

    if (!mips::utils::is_branch_instr(block.code.back()))
    {
        const uint32_t target = block.addr + block.code.size() * 4;
        compile_jump(target);
    }

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
        const auto usage = 100 * _allocator.get_used() / static_cast<float>(_allocator.get_total());
        _debug_stream << strtools::catf("\nAllocated %db of executable memory - %f%% used\n", size, usage);

        for (size_t i = 0; i < size; ++i)
        {
            _debug_stream << strtools::catf("%02x ", buffer[i]);
        }
        _debug_stream << "\n";
        
        if (!_linker.global_map().empty())
        {
            _debug_stream << "\nGlobally resolved linker symbols\n";
            for (const auto& [label, ptr] : _linker.global_map())
            {
                _debug_stream << strtools::catf("%s: 0x%p\n", label.c_str(), ptr);
            }
        }
    }

    return CompiledBlock
    (
        reinterpret_cast<CompiledBlock::func>(buffer),
        size,
        config
    );
}

std::string Compiler::get_debug() const
{
    return _debug_stream.str();
}

void Compiler::compile(const mips::Instruction instr, const uint32_t addr)
{
    std::visit(functional::overload{
        [&](const auto& x) { compile(x, addr); }
    }, instr);
}

void Compiler::compile(const mips::InstructionR instr, const uint32_t addr)
{
    switch (instr.op)
    {
        case mips::OpcodeR::ADDU: compile<x86::Opcode::ADD>(instr); break;
        case mips::OpcodeR::ADD:  compile<x86::Opcode::ADD>(instr); break;
        case mips::OpcodeR::SUBU: compile<x86::Opcode::SUB>(instr); break;
        case mips::OpcodeR::SUB:  compile<x86::Opcode::SUB>(instr); break;
        case mips::OpcodeR::AND:  compile<x86::Opcode::AND>(instr); break;
        case mips::OpcodeR::OR:   compile<x86::Opcode::OR>(instr); break;
        case mips::OpcodeR::XOR:  compile<x86::Opcode::XOR>(instr); break;
        case mips::OpcodeR::JR:   compile_jump(instr.src1); break;
        case mips::OpcodeR::SLT:
        {
            using namespace x86;
            compile_reg_load(acc1_reg, instr.src1);
            compile_reg_load<Opcode::CMP>(acc1_reg, instr.src2);
            _assembler.instr_imm<Opcode::MOV_I, OpcodeExt::MOV_I>(acc1_reg, 1);
            _assembler.instr_imm<Opcode::MOV_I, OpcodeExt::MOV_I>(acc2_reg, 0);
            _assembler.move_cond<CondCode::L>(acc2_reg, acc1_reg);
            compile_reg_write(instr.dst, acc2_reg);
            break;
        }
        default: throw_invalid_instr(instr);
    }
}

template <x86::Opcode Op>
void Compiler::compile(const mips::InstructionR instr)
{
    if (instr.dst == mips::Register::zero) return;
    
    compile_reg_load(acc1_reg, instr.src1);
    compile_reg_load<Op>(acc1_reg, instr.src2);
    compile_reg_write(instr.dst, acc1_reg);
}

void Compiler::compile(const mips::InstructionI instr, const uint32_t addr)
{
    switch (instr.op)
    {
        case mips::OpcodeI::ADDIU: compile<x86::Opcode::ADD_I, x86::OpcodeExt::ADD_I>(instr); break;
        case mips::OpcodeI::ADDI:  compile<x86::Opcode::ADD_I, x86::OpcodeExt::ADD_I>(instr); break;
        case mips::OpcodeI::ANDI:  compile<x86::Opcode::AND_I, x86::OpcodeExt::AND_I>(instr); break;
        case mips::OpcodeI::ORI:   compile<x86::Opcode::OR_I, x86::OpcodeExt::OR_I>(instr); break;
        case mips::OpcodeI::XORI:  compile<x86::Opcode::XOR_I, x86::OpcodeExt::XOR_I>(instr); break;
        default: throw_invalid_instr(instr);
    }
}

template <x86::Opcode Op, x86::OpcodeExt Ext>
void Compiler::compile(const mips::InstructionI instr)
{
    if (instr.dst == mips::Register::zero) return;

    if (instr.dst == instr.src)
    {
        compile_reg_write<Op, Ext>(instr.dst, instr.constant);
    }
    else
    {
        compile_reg_load(acc1_reg, instr.src);
        _assembler.instr_imm<Op, Ext>(acc1_reg, instr.constant);
        compile_reg_write(instr.dst, acc1_reg);
    }
}

void Compiler::compile(const mips::InstructionJ instr, const uint32_t addr)
{
    switch (instr.op)
    {
        case mips::OpcodeJ::JAL:
        {
            const uint32_t link = addr + 4;
            compile_reg_write(mips::Register::ra, link);
            [[fallthrough]];
        }
        case mips::OpcodeJ::J:
        {
            const uint32_t target = (0xF0000000 & addr) | (instr.target << 2);
            compile_jump(target);
            break;
        }
        default: throw_invalid_instr(instr);
    }
}

void Compiler::throw_invalid_instr(mips::Instruction instr)
{
    std::visit(functional::overload{
        [&](const auto& x) { throw std::logic_error(std::string("Instruction ") + mips::opcode_to_string(x.op) + " is not supported"); }
    }, instr);
}

constexpr uint32_t Compiler::calc_reg_offset(mips::Register reg)
{
    return static_cast<int32_t>(reg) * sizeof(int32_t);
}

template <x86::Opcode Op>
void Compiler::compile_reg_load(const x86::Register dst, const mips::Register src)
{
    _assembler.instr<Op, x86::InstrMode::MR>(dst, addr_reg, calc_reg_offset(src));
}

template <x86::Opcode Op>
void Compiler::compile_reg_write(const mips::Register dst, const x86::Register src)
{
    _assembler.instr<Op, x86::InstrMode::RM>(addr_reg, src, calc_reg_offset(dst));
}

template <x86::Opcode Op, x86::OpcodeExt Ext>
void Compiler::compile_reg_write(const mips::Register dst, const uint32_t imm)
{
    _assembler.instr_imm<Op, Ext, x86::InstrMode::IM>(addr_reg, imm, calc_reg_offset(dst));
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

void Compiler::compile_jump(const uint32_t target)
{
    _assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(return_reg, target);
    _assembler.instr<x86::Opcode::RET>();
}

void Compiler::compile_jump(const mips::Register target)
{
    compile_reg_load(return_reg, target);
    _assembler.instr<x86::Opcode::RET>();
}
