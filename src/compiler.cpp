#include "compiler.hpp"

#include <config.hpp>
#include <mips/utils.hpp>
#include <utils/functional.hpp>

Compiler::Compiler(mips::RegisterFile& regs, mips::MemoryMap& mem, Allocator& allocator)
    : _regs(regs)
    , _mem(mem)
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
    using namespace x86;
    switch (instr.op)
    {
        case mips::OpcodeR::ADDU: compile<Opcode::ADD>(instr); break;
        case mips::OpcodeR::ADD:  compile<Opcode::ADD>(instr); break;
        case mips::OpcodeR::SUBU: compile<Opcode::SUB>(instr); break;
        case mips::OpcodeR::SUB:  compile<Opcode::SUB>(instr); break;
        case mips::OpcodeR::AND:  compile<Opcode::AND>(instr); break;
        case mips::OpcodeR::OR:   compile<Opcode::OR>(instr); break;
        case mips::OpcodeR::XOR:  compile<Opcode::XOR>(instr); break;
        case mips::OpcodeR::NOR:  compile_nor(instr); break;
        case mips::OpcodeR::JR:   compile_jump(instr.rs); break;
        case mips::OpcodeR::SLT:  compile_compare<CondCode::L>(instr); break;
        case mips::OpcodeR::SLTU: compile_compare<CondCode::B>(instr); break;
        case mips::OpcodeR::SLL:  compile_shift_imm<Opcode::SHL_I, OpcodeExt::SHL_I>(instr); break;
        case mips::OpcodeR::SRA:  compile_shift_imm<Opcode::SAR_I, OpcodeExt::SAR_I>(instr); break;
        case mips::OpcodeR::SRL:  compile_shift_imm<Opcode::SHR_I, OpcodeExt::SHR_I>(instr); break;
        default: throw_invalid_instr(instr);
    }
}

template <x86::Opcode Op>
void Compiler::compile(const mips::InstructionR instr)
{
    if (instr.rd == mips::Register::$zero) return;

    if (instr.rs == instr.rd)
    {
        compile_reg_load(acc1_reg, instr.rt);
        compile_reg_write<Op>(instr.rd, acc1_reg);
    }
    else
    {
        compile_reg_load(acc1_reg, instr.rs);
        compile_reg_load<Op>(acc1_reg, instr.rt);
        compile_reg_write(instr.rd, acc1_reg);
    }
}

void Compiler::compile(const mips::InstructionI instr, const uint32_t addr)
{
    using namespace x86;
    switch (instr.op)
    {
        case mips::OpcodeI::ADDIU:  compile<Opcode::ADD_I, OpcodeExt::ADD_I>(instr); break;
        case mips::OpcodeI::ADDI:   compile<Opcode::ADD_I, OpcodeExt::ADD_I>(instr); break;
        case mips::OpcodeI::ANDI:   compile<Opcode::AND_I, OpcodeExt::AND_I>(instr); break;
        case mips::OpcodeI::ORI:    compile<Opcode::OR_I,  OpcodeExt::OR_I>(instr); break;
        case mips::OpcodeI::XORI:   compile<Opcode::XOR_I, OpcodeExt::XOR_I>(instr); break;
        case mips::OpcodeI::SLTI:   compile_compare<CondCode::L>(instr); break;
        case mips::OpcodeI::SLTIU:  compile_compare<CondCode::B>(instr); break;
        case mips::OpcodeI::BEQ:    compile_branch<CondCode::E>(instr, addr); break;
        case mips::OpcodeI::BNE:    compile_branch<CondCode::NE>(instr, addr); break;
        case mips::OpcodeI::BGTZ:   compile_branch<CondCode::G>(instr, addr); break;
        case mips::OpcodeI::BLEZ:   compile_branch<CondCode::LE>(instr, addr); break;
        case mips::OpcodeI::BGEZ:   compile_branch<CondCode::GE>(instr, addr); break;
        case mips::OpcodeI::BLTZ:   compile_branch<CondCode::L>(instr, addr); break;
        case mips::OpcodeI::BGEZAL: compile_branch_and_link<CondCode::GE>(instr, addr); break;
        case mips::OpcodeI::BLTZAL: compile_branch_and_link<CondCode::L>(instr, addr); break;
        case mips::OpcodeI::LW:     compile_mem_read<uint32_t>(instr); break;
        case mips::OpcodeI::LB:     compile_mem_read<int8_t>(instr); break;
        case mips::OpcodeI::LBU:    compile_mem_read<uint8_t>(instr); break;
        case mips::OpcodeI::LH:     compile_mem_read<int16_t>(instr); break;
        case mips::OpcodeI::LHU:    compile_mem_read<uint16_t>(instr); break;
        case mips::OpcodeI::SW:     compile_mem_write<uint32_t>(instr); break;
        case mips::OpcodeI::SB:     compile_mem_write<uint8_t>(instr); break;
        case mips::OpcodeI::SH:     compile_mem_write<uint16_t>(instr); break;
        case mips::OpcodeI::LUI:    compile_reg_write(instr.rt, instr.constant << 16); break;
        default: throw_invalid_instr(instr);
    }
}

template <x86::Opcode Op, x86::OpcodeExt Ext>
void Compiler::compile(const mips::InstructionI instr)
{
    compile_imm<Op, Ext>(instr.rt, instr.rs, instr.constant);
}

void Compiler::compile(const mips::InstructionJ instr, const uint32_t addr)
{
    switch (instr.op)
    {
        case mips::OpcodeJ::JAL:
        {
            const uint32_t link = addr + 4;
            compile_reg_write(mips::Register::$ra, link);
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

template <x86::Opcode Op, x86::OpcodeExt Ext>
void Compiler::compile_shift_imm(const mips::InstructionR instr)
{
    compile_imm<Op, Ext>(instr.rd, instr.rt, instr.sa);
}

template <x86::Opcode Op, x86::OpcodeExt Ext>
void Compiler::compile_imm(const mips::Register dst, const mips::Register src, const uint32_t imm)
{
    if (dst == mips::Register::$zero) return;

    if (dst == src)
    {
        compile_reg_write<Op, Ext>(dst, imm);
    }
    else
    {
        compile_reg_load(acc1_reg, src);
        _assembler.instr_imm<Op, Ext>(acc1_reg, imm);
        compile_reg_write(dst, acc1_reg);
    }
}

template <x86::CondCode Cond>
void Compiler::compile_compare(const mips::InstructionR instr)
{
    if (instr.rd == mips::Register::$zero) return;

    compile_reg_load(acc1_reg, instr.rs);
    compile_reg_load<x86::Opcode::CMP>(acc1_reg, instr.rt);
    compile_reg_write(instr.rd, 0);
    _assembler.set_cond<Cond, x86::InstrMode::RM>(addr_reg, calc_reg_offset(instr.rd));
}

template <x86::CondCode Cond>
void Compiler::compile_compare(const mips::InstructionI instr)
{
    if (instr.rt == mips::Register::$zero) return;

    compile_reg_read<x86::Opcode::CMP_I, x86::OpcodeExt::CMP_I>(instr.rs, instr.constant);
    compile_reg_write(instr.rt, 0);
    _assembler.set_cond<Cond, x86::InstrMode::RM>(addr_reg, calc_reg_offset(instr.rt));
}

template <x86::CondCode Cond>
void Compiler::compile_branch(const mips::InstructionI instr, const uint32_t addr)
{
    const uint32_t target_true = addr + (instr.constant << 2);
    const uint32_t target_false = addr + 4;

    if (instr.rs == mips::Register::$zero)
    {
        compile_reg_read<x86::Opcode::CMP_I, x86::OpcodeExt::CMP_I>(instr.rt, 0);
    }
    else if (instr.rt == mips::Register::$zero)
    {
        compile_reg_read<x86::Opcode::CMP_I, x86::OpcodeExt::CMP_I>(instr.rs, 0);
    }
    else
    {
        compile_reg_load(acc2_reg, instr.rs);
        compile_reg_load<x86::Opcode::CMP>(acc2_reg, instr.rt);
    }

    _assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(acc2_reg, target_true);
    _assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(return_reg, target_false);
    _assembler.move_cond<Cond>(return_reg, acc2_reg);
    _assembler.instr<x86::Opcode::RET>();
}

template <x86::CondCode Cond>
void Compiler::compile_branch_and_link(const mips::InstructionI instr, const uint32_t addr)
{
    const uint32_t link = addr + 4;
    compile_reg_write(mips::Register::$ra, link);
    compile_branch<Cond>(instr, addr);
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

template <x86::Opcode Op, x86::OpcodeExt Ext>
void Compiler::compile_reg_read(const mips::Register src, const uint32_t imm)
{
    _assembler.instr_imm<Op, Ext, x86::InstrMode::IM>(addr_reg, imm, calc_reg_offset(src));
}

template <x86::Opcode Op>
void Compiler::compile_reg_write(const mips::Register dst, const x86::Register src)
{
    if (dst == mips::Register::$zero)
        throw std::logic_error("Cannot write to $zero");

    _assembler.instr<Op, x86::InstrMode::RM>(addr_reg, src, calc_reg_offset(dst));
}

template <x86::Opcode Op, x86::OpcodeExt Ext>
void Compiler::compile_reg_write(const mips::Register dst, const uint32_t imm)
{
    if (dst == mips::Register::$zero)
        throw std::logic_error("Cannot write to $zero");

    _assembler.instr_imm<Op, Ext, x86::InstrMode::IM>(addr_reg, imm, calc_reg_offset(dst));
}

void Compiler::compile_nor(const mips::InstructionR instr)
{
    using namespace x86;
    if (instr.rd == mips::Register::$zero) return;

    compile<Opcode::OR>(instr);
    _assembler.instr<Opcode::NOT, OpcodeExt::NOT, InstrMode::RM>(addr_reg, calc_reg_offset(instr.rd));
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

void Compiler::compile_compute_mem_addr(const x86::Register dst, const mips::InstructionI instr)
{
    using namespace x86;

    if (instr.constant)
    {
        if (dst == addr_reg)
        {
            compile_reg_load(acc1_reg, instr.rs);
            _assembler.instr_imm<Opcode::ADD_I, OpcodeExt::ADD_I>(acc1_reg, instr.constant);
            _assembler.instr<Opcode::MOV>(dst, acc1_reg);
        }
        else
        {
            compile_reg_load(dst, instr.rs);
            _assembler.instr_imm<Opcode::ADD_I, OpcodeExt::ADD_I>(dst, instr.constant);
        }
    }
    else
    {
        compile_reg_load(dst, instr.rs);
    }
}

template <typename T>
void Compiler::compile_mem_write(const mips::InstructionI instr)
{
    using namespace x86;
    const static auto name = strtools::catf("mem_write<%s>", utils::nameof<T>());
    const static auto label = _label_generator.generate(name);
    const static auto func = utils::instance_proxy<uint32_t, uint32_t>::call<mips::MemoryMap, void, &mips::MemoryMap::write<T>>;

    _assembler.instr_imm<Opcode::MOV_I, OpcodeExt::MOV_I>(Register::ECX, reinterpret_cast<uint32_t>(&_mem));
    _assembler.instr<Opcode::PUSH, OpcodeExt::PUSH>(addr_reg);
    _assembler.instr<Opcode::PUSH, OpcodeExt::PUSH, InstrMode::RM>(addr_reg, calc_reg_offset(instr.rt));

    compile_compute_mem_addr(Register::EDX, instr);

    _linker.label_global(label, func);
    _linker.resolve(label, [&] { return _assembler.size(); }, [&](const int32_t offset)
    {
        _assembler.call(offset);
    });

    _assembler.instr<Opcode::POP, OpcodeExt::POP>(addr_reg);
}

template <typename T>
void Compiler::compile_mem_read(const mips::InstructionI instr)
{
    using namespace x86;
    const static auto name = strtools::catf("mem_read<%s>", utils::nameof<T>());
    const static auto label = _label_generator.generate(name);
    const static auto func = utils::instance_proxy<uint32_t>::call<mips::MemoryMap, uint32_t, &mips::MemoryMap::read<T>>;

    _assembler.instr_imm<Opcode::MOV_I, OpcodeExt::MOV_I>(Register::ECX, reinterpret_cast<uint32_t>(&_mem));
    _assembler.instr<Opcode::PUSH, OpcodeExt::PUSH>(addr_reg);

    compile_compute_mem_addr(Register::EDX, instr);

    _linker.label_global(label, func);
    _linker.resolve(label, [&] { return _assembler.size(); }, [&](const int32_t offset)
    {
        _assembler.call(offset);
    });

    _assembler.instr<Opcode::POP, OpcodeExt::POP>(addr_reg);
    compile_reg_write(instr.rt, Register::EAX);
}
