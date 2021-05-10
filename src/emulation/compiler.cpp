#include "compiler.hpp"

#include <x86/utils.hpp>
#include <mips/utils.hpp>
#include <utils/functional.hpp>
#include <utils/utils.hpp>

namespace emulation
{
    Compiler::Compiler(mips::RegisterFile& regs, mips::MemoryMap& mem)
        : _regs(regs)
        , _mem(mem)
    { }

    CompiledBlock Compiler::compile(const SourceBlock& block, const CompilerConfig config)
    {
        reset();

        _state.source = &block;
        _state.config = config;

        if constexpr (debug)
        {
            std::stringstream().swap(_debug_stream);
            _debug_stream << strtools::catf("Compiling block 0x%x with %s\n", block.addr, strtools::cat(config).c_str());

            for (const auto& instr : block.code)
            {
                _debug_stream << instr << "\n";
            }

            _debug_stream << "\n";
        }

        const auto reg_file = reinterpret_cast<uint32_t>(_regs.data());
        _assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(addr_reg, reg_file);

        uint32_t pc = block.addr;
        for (uint32_t i = 0; i < block.code.size(); i++)
        {
            compile(block.code[i], pc);
            pc += 4;

            if (_state.terminated)
                break;
        }

        if (!_state.terminated)
            compile_jump(pc, pc);

        CompiledBlock output;
        output.size = _assembler.size();
        output.host_instr_count = _assembler.instr_count();
        output.source_instr_count = block.code.size();
        output.config = config;

        if constexpr (debug)
        {
            _debug_stream << "Generated " << output.host_instr_count << " x86 instructions\n" << _assembler.get_debug();
        }

        uint8_t* buffer = _allocator.alloc(output.size);
        _assembler.copy(buffer);
        _linker.terminate_local(buffer);
        output.code = reinterpret_cast<CompiledBlock::func>(buffer);

        if (_state.config.direct_linking)
        {
            for (const auto& [offset, target] : _state.unresolved_jumps)
            {
                uint8_t* src = reinterpret_cast<uint8_t*>(output.code) + offset;
                output.unresolved_jumps.emplace_back(src, target);
            }

            for (const auto& [offset, cond, target_true, target_false] : _state.unresolved_cond_jumps)
            {
                uint8_t* src = reinterpret_cast<uint8_t*>(output.code) + offset;
                output.unresolved_cond_jumps.emplace_back(src, cond, target_true, target_false);
            }
        }

        if constexpr (debug)
        {
            const auto usage = 100 * _allocator.get_used() / static_cast<float>(_allocator.get_total());
            _debug_stream << strtools::catf("\nAllocated %db of executable memory - %f%% used\n", output.size, usage);

            for (size_t i = 0; i < output.size; ++i)
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

        return output;
    }

    void Compiler::reset()
    {
        _assembler.reset();
        _state = State();
    }

    std::string Compiler::get_debug() const
    {
        return _debug_stream.str();
    }

    std::optional<mips::Instruction> Compiler::get_instr_at_addr(const uint32_t addr)
    {
        const size_t index = (addr - _state.source->addr) / 4;
        if (index >= 0 && index < _state.source->code.size())
            return _state.source->code[index];

        return std::nullopt;
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
            case mips::OpcodeR::ADDU:  compile<Opcode::ADD>(instr); break;
            case mips::OpcodeR::ADD:   compile<Opcode::ADD>(instr); break;
            case mips::OpcodeR::SUBU:  compile<Opcode::SUB>(instr); break;
            case mips::OpcodeR::SUB:   compile<Opcode::SUB>(instr); break;
            case mips::OpcodeR::AND:   compile<Opcode::AND>(instr); break;
            case mips::OpcodeR::OR:    compile<Opcode::OR>(instr); break;
            case mips::OpcodeR::XOR:   compile<Opcode::XOR>(instr); break;
            case mips::OpcodeR::NOR:   compile_nor(instr); break;
            case mips::OpcodeR::JR:    compile_jump(instr.rs, addr); break;
            case mips::OpcodeR::JALR:  compile_jump_and_link(instr.rs, instr.rd, addr); break;
            case mips::OpcodeR::SLT:   compile_compare<CondCode::L>(instr); break;
            case mips::OpcodeR::SLTU:  compile_compare<CondCode::B>(instr); break;
            case mips::OpcodeR::SLL:   compile_shift_imm<Opcode::SHL_I, OpcodeExt::SHL_I>(instr); break;
            case mips::OpcodeR::SRA:   compile_shift_imm<Opcode::SAR_I, OpcodeExt::SAR_I>(instr); break;
            case mips::OpcodeR::SRL:   compile_shift_imm<Opcode::SHR_I, OpcodeExt::SHR_I>(instr); break;
            case mips::OpcodeR::SLLV:  compile_shift<Opcode::SHL, OpcodeExt::SHL>(instr); break;
            case mips::OpcodeR::SRAV:  compile_shift<Opcode::SAR, OpcodeExt::SAR>(instr); break;
            case mips::OpcodeR::SRLV:  compile_shift<Opcode::SHR, OpcodeExt::SHR>(instr); break;
            case mips::OpcodeR::MFHI:  compile_mfhi(instr); break;
            case mips::OpcodeR::MFLO:  compile_mflo(instr); break;
            case mips::OpcodeR::MTHI:  compile_mthi(instr); break;
            case mips::OpcodeR::MTLO:  compile_mtlo(instr); break;
            case mips::OpcodeR::MULT:  compile_mul<Opcode::IMUL, OpcodeExt::IMUL>(instr); break;
            case mips::OpcodeR::MULTU: compile_mul<Opcode::MUL, OpcodeExt::MUL>(instr); break;
            case mips::OpcodeR::DIV:   compile_div(instr); break;
            case mips::OpcodeR::DIVU:  compile_divu(instr); break;
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
            case mips::OpcodeI::ORI:    compile<Opcode::OR_I, OpcodeExt::OR_I>(instr); break;
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
            case mips::OpcodeI::LWL:    compile_lwl(instr); break;
            case mips::OpcodeI::LWR:    compile_lwr(instr); break;
            case mips::OpcodeI::LUI:    compile_reg_write(instr.rt, instr.constant << 16); break;
            default:                    throw_invalid_instr(instr);
        }
    }

    template <x86::Opcode Op, x86::OpcodeExt Ext>
    void Compiler::compile(const mips::InstructionI instr)
    {
        const uint32_t constant =
            mips::utils::sign_extends_imm(instr.op)
                ? instr.constant
                : static_cast<uint16_t>(instr.constant);

        compile_imm<Op, Ext>(instr.rt, instr.rs, constant);
    }

    void Compiler::compile(const mips::InstructionJ instr, const uint32_t addr)
    {
        switch (instr.op)
        {
            case mips::OpcodeJ::JAL:
            {
                const uint32_t link = addr + 8;
                compile_reg_write(mips::Register::$ra, link);
                [[fallthrough]];
            }
            case mips::OpcodeJ::J:
            {
                const uint32_t target = (0xF0000000 & addr) | (instr.target << 2);
                compile_jump(target, addr);
                break;
            }
            default: throw_invalid_instr(instr);
        }
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
        std::optional<mips::Instruction> delay_slot = get_instr_at_addr(addr + 4);
        const bool delay_slot_exists = delay_slot && !mips::utils::is_nop(delay_slot.value());

        const uint32_t target_true = addr + (instr.constant << 2);
        const uint32_t target_false = delay_slot_exists
            ? addr + 8
            : addr + 4;

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

        if (delay_slot_exists)
        {
            _assembler.instr<x86::Opcode::PUSHF>();
            compile_delay_slot(delay_slot.value(), addr + 4);
            _assembler.instr<x86::Opcode::POPF>();
        }

        if (_state.config.direct_linking)
            _state.unresolved_cond_jumps.emplace_back(_assembler.size(), Cond, target_true, target_false);

        _assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(acc2_reg, target_true);
        _assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(return_reg, target_false);
        _assembler.move_cond<Cond>(return_reg, acc2_reg);
        _assembler.instr<x86::Opcode::RET>();
        _state.terminated = true;
    }

    template <x86::CondCode Cond>
    void Compiler::compile_branch_and_link(const mips::InstructionI instr, const uint32_t addr)
    {
        const uint32_t link = addr + 8;
        compile_reg_write(mips::Register::$ra, link);
        compile_branch<Cond>(instr, addr);
    }

    template <x86::Opcode Op, x86::OpcodeExt Ext>
    void Compiler::compile_shift_imm(const mips::InstructionR instr)
    {
        compile_imm<Op, Ext>(instr.rd, instr.rt, instr.sa);
    }

    template <x86::Opcode Op, x86::OpcodeExt Ext>
    void Compiler::compile_shift(const mips::InstructionR instr)
    {
        using namespace x86;

        if (instr.rt != instr.rd)
        {
            compile_reg_load(acc1_reg, instr.rt);
            compile_reg_write(instr.rd, acc1_reg);
        }

        _assembler.instr<Opcode::MOV>(acc1_reg, addr_reg);
        compile_reg_load(Register::ECX, instr.rs);
        _assembler.instr_imm<Opcode::AND_I, OpcodeExt::AND_I>(Register::ECX, 0b11111);
        _assembler.instr<Op, Ext, InstrMode::RM>(acc1_reg, calc_reg_offset(instr.rd));
        _assembler.instr<Opcode::MOV>(addr_reg, acc1_reg);
    }

    template <x86::Opcode Op, x86::OpcodeExt Ext>
    void Compiler::compile_mul(const mips::InstructionR instr)
    {
        using namespace x86;

        compile_reg_load(Register::EAX, instr.rs);
        _assembler.instr<Op, Ext, InstrMode::RM>(addr_reg, calc_reg_offset(instr.rt));
        compile_hi_lo_writeback();
    }

    void Compiler::compile_div(const mips::InstructionR instr)
    {
        using namespace x86;

        compile_reg_load(Register::EAX, instr.rs);
        _assembler.instr<Opcode::MOV>(Register::EDX, Register::EAX);
        _assembler.instr_imm<Opcode::SAR_I, OpcodeExt::SAR_I>(Register::EDX, 31);
        _assembler.instr<Opcode::IDIV, OpcodeExt::IDIV, InstrMode::RM>(addr_reg, calc_reg_offset(instr.rt));
        compile_hi_lo_writeback();
    }

    void Compiler::compile_divu(const mips::InstructionR instr)
    {
        using namespace x86;

        compile_reg_load(Register::EAX, instr.rs);
        _assembler.instr<Opcode::XOR>(Register::EDX, Register::EDX);
        _assembler.instr<Opcode::DIV, OpcodeExt::DIV, InstrMode::RM>(addr_reg, calc_reg_offset(instr.rt));
        compile_hi_lo_writeback();
    }

    void Compiler::compile_hi_lo_writeback()
    {
        compile_reg_write(mips::RegisterFile::hi_reg, x86::Register::EDX);
        compile_reg_write(mips::RegisterFile::lo_reg, x86::Register::EAX);
    }

    void Compiler::compile_delay_slot(mips::Instruction delay_instr, uint32_t delay_addr)
    {
        if (mips::utils::is_branch_instr(delay_instr)) [[unlikely]]
            throw std::runtime_error("Cannot compile a branch instruction inside of a branch delay slot");

        compile(delay_instr, delay_addr);
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
        if (dst == mips::Register::$zero) [[unlikely]]
            throw std::logic_error("Cannot write to $zero");

        _assembler.instr<Op, x86::InstrMode::RM>(addr_reg, src, calc_reg_offset(dst));
    }

    template <x86::Opcode Op, x86::OpcodeExt Ext>
    void Compiler::compile_reg_write(const mips::Register dst, const uint32_t imm)
    {
        if (dst == mips::Register::$zero) [[unlikely]]
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

    void Compiler::compile_mfhi(const mips::InstructionR instr)
    {
        if (instr.rd == mips::Register::$zero) return;

        compile_reg_load(acc1_reg, mips::RegisterFile::hi_reg);
        compile_reg_write(instr.rd, acc1_reg);
    }

    void Compiler::compile_mflo(const mips::InstructionR instr)
    {
        if (instr.rd == mips::Register::$zero) return;

        compile_reg_load(acc1_reg, mips::RegisterFile::lo_reg);
        compile_reg_write(instr.rd, acc1_reg);
    }

    void Compiler::compile_mthi(const mips::InstructionR instr)
    {
        if (instr.rs == mips::Register::$zero) return;

        compile_reg_load(acc1_reg, instr.rs);
        compile_reg_write(mips::RegisterFile::hi_reg, acc1_reg);
    }

    void Compiler::compile_mtlo(const mips::InstructionR instr)
    {
        if (instr.rs == mips::Register::$zero) return;

        compile_reg_load(acc1_reg, instr.rs);
        compile_reg_write(mips::RegisterFile::lo_reg, acc1_reg);
    }


    void Compiler::compile_call(void (* const f)())
    {
        const auto label = _label_generator.generate("func");
        _linker.label_global(label, f);
        _linker.resolve(label, [&] { return _assembler.size(); }, [&](const int32_t offset)
        {
            _assembler.call(offset);
        });
    }

    void Compiler::compile_jump(const uint32_t target, const uint32_t addr)
    {
        if (_state.config.direct_linking)
            _state.unresolved_jumps.emplace_back(_assembler.size(), target);

        if (auto delay_slot = get_instr_at_addr(addr + 4))
        {
            compile_delay_slot(delay_slot.value(), addr + 4);
        }

        _assembler.instr_imm<x86::Opcode::MOV_I, x86::OpcodeExt::MOV_I>(return_reg, target);
        _assembler.instr<x86::Opcode::RET>();
        _state.terminated = true;
    }

    void Compiler::compile_jump(const mips::Register target, const uint32_t addr)
    {
        bool use_stack = false;

        if (auto delay_slot = get_instr_at_addr(addr + 4))
        {
            use_stack = mips::utils::writes_reg(delay_slot.value(), target);

            if (use_stack)
            {
                compile_reg_load(return_reg, target);
                _assembler.instr<x86::Opcode::PUSH, x86::OpcodeExt::PUSH>(return_reg);
            }

            compile_delay_slot(delay_slot.value(), addr + 4);
        }

        if (use_stack)
            _assembler.instr<x86::Opcode::POP, x86::OpcodeExt::POP>(return_reg);
        else
            compile_reg_load(return_reg, target);

        _assembler.instr<x86::Opcode::RET>();
        _state.terminated = true;
    }

    void Compiler::compile_jump_and_link(const mips::Register target, const mips::Register link_reg, const uint32_t addr)
    {
        const uint32_t link = addr + 8;
        compile_reg_write(link_reg, link);
        compile_jump(target, addr);
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
        const static auto name = strtools::catf("__mem_write<%s>", utils::nameof<T>());
        const static auto func = utils::instance_proxy<uint32_t, uint32_t>::call<mips::MemoryMap, void, &mips::MemoryMap::write<T>>;
        compile_mem_instr(instr, func, name, true, false);
    }

    template <typename T>
    void Compiler::compile_mem_read(const mips::InstructionI instr)
    {
        const static auto name = strtools::catf("__mem_read<%s>", utils::nameof<T>());
        const static auto func = ::utils::instance_proxy<uint32_t>::call<mips::MemoryMap, uint32_t, &mips::MemoryMap::read<T>>;
        compile_mem_instr(instr, func, name, false, true);
    }

    void Compiler::compile_lwl(const mips::InstructionI instr)
    {
        const static auto name = "__mem_lwl";
        const static auto func = utils::instance_proxy<uint32_t, uint32_t>::call<mips::MemoryMap, uint32_t, &mips::MemoryMap::load_word_left>;
        compile_mem_instr(instr, func, name, true, true);
    }

    void Compiler::compile_lwr(const mips::InstructionI instr)
    {
        const static auto name = "__mem_lwr";
        const static auto func = utils::instance_proxy<uint32_t, uint32_t>::call<mips::MemoryMap, uint32_t, &mips::MemoryMap::load_word_right>;
        compile_mem_instr(instr, func, name, true, true);
    }

    template <typename F>
    void Compiler::compile_mem_instr(mips::InstructionI instr, F func, const std::string& name, bool has_arg, bool has_return)
    {
        using namespace x86;

        compile_compute_mem_addr(Register::EDX, instr);
        compile_call_instance(func, name, _mem, [&]
        {
            if (has_arg)
            {
                _assembler.instr<Opcode::PUSH, OpcodeExt::PUSH, InstrMode::RM>(addr_reg, calc_reg_offset(instr.rt));
            }
        });

        if (has_return && instr.rt != mips::Register::$zero)
            compile_reg_write(instr.rt, Register::EAX);
    }

    template <typename F, typename T, typename Pre>
    void Compiler::compile_call_instance(F func, const std::string& label, T& instance, Pre pre_call)
    {
        using namespace x86;

        _assembler.instr<Opcode::PUSH, OpcodeExt::PUSH>(addr_reg);
        pre_call();
        _assembler.instr_imm<Opcode::MOV_I, OpcodeExt::MOV_I>(Register::ECX, reinterpret_cast<uint32_t>(&instance));

        compile_call(func, label);

        _assembler.instr<Opcode::POP, OpcodeExt::POP>(addr_reg);
    }

    template <typename F, typename T>
    void Compiler::compile_call_instance(F func, const std::string& label, T& instance)
    {
        compile_call_instance(func, label, instance, []{});
    }

    template <typename F>
    void Compiler::compile_call(F func, const std::string& label)
    {
        _linker.label_global(label, func);
        _linker.resolve(label, [&] { return _assembler.size(); }, [&](const int32_t offset)
        {
            _assembler.call(offset);
        });
    }

    template <typename T, void(T::* F)()>
    void Compiler::compile_call(T& obj)
    {
        using namespace x86;
        const auto addr = reinterpret_cast<uint32_t>(&obj);
        _assembler.instr_imm<Opcode::MOV_I, OpcodeExt::MOV_I>(Register::ECX, addr);

        const auto label = _label_generator.generate("member_func");
        _linker.label_global(label, &::utils::instance_proxy<>::call<T, void, F>);
        _linker.resolve(label, [&] { return _assembler.size(); }, [&](const int32_t offset)
        {
            _assembler.call(offset);
        });
    }
}
